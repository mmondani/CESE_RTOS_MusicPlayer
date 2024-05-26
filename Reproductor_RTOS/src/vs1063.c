#include "vs1063.h"
#include "vs10xx_uc.h"
#include "vs1063a-patches.h"

void LoadPlugin(const u_int16 *d, u_int16 len);


extern GPIO xCS_VS1063, xDCS_VS1063, DREQ_VS1063, RST_VS1063;

unsigned char dataBuffer[32];

extern const unsigned short plugin[];

int VS1063Init(void)
{
  /* Start initialization with a dummy read, which makes sure our
     microcontoller chips selects and everything are where they
     are supposed to be and that VS10xx's SCI bus is in a known state. */
	VS1063ReadSCI(SCI_MODE);

  /* First real operation is a software reset. After the software
     reset we know what the status of the IC is. You need, depending
     on your application, either set or not set SM_SDISHARE. See the
     Datasheet for details. */
	//VS1063WriteSCI(SCI_MODE, SM_SDINEW|SM_SDISHARE|SM_TESTS|SM_RESET);
	VS1063WriteSCI(SCI_MODE, SM_SDINEW|SM_RESET);

  /* A quick sanity check: write to two registers, then test if we
     get the same results. Note that if you use a too high SPI
     speed, the MSB is the most likely to fail when read again. */
	VS1063WriteSCI(SCI_HDAT0, 0xABAD);
	VS1063WriteSCI(SCI_HDAT1, 0x1DEA);
	if (VS1063ReadSCI(SCI_HDAT0) != 0xABAD || VS1063ReadSCI(SCI_HDAT1) != 0x1DEA)
	{
		printf("There is something wrong with VS10xx\n");
		return 1;
	}

  /* Set the clock. Until this point we need to run SPI slow so that
     we do not exceed the maximum speeds mentioned in
     Chapter SPI Timing Diagram in the Datasheet. */
	//VS1063WriteSCI(SCI_CLOCKF, HZ_TO_SC_FREQ(12288000) | SC_MULT_53_40X | SC_ADD_53_15X);
	VS1063WriteSCI(SCI_CLOCKF, 0 | SC_MULT_53_40X | SC_ADD_53_15X);

  /* Now when we have upped the VS10xx clock speed, the microcontroller
     SPI bus can run faster. Do that before you start playing or
     recording files. */

  /* Set up other parameters. */
	VS1063WriteMem(PAR_CONFIG1, PAR_CONFIG1_AAC_SBR_SELECTIVE_UPSAMPLE);

  /* Set volume level at -30 dB of maximum */
	VS1063WriteSCI(SCI_VOL, 0x3C3C);


	LoadPlugin(plugin, sizeof(plugin)/sizeof(plugin[0]));

	return 0;
}

unsigned int VS1063isBusy (void)
{
	return (GPIOestaActivo(&DREQ_VS1063) == 0);
}

/*
  Write 16-bit value to given VS10xx address
*/
void VS1063WriteMem(unsigned int addr, unsigned int data)
{
	VS1063WriteSCI(SCI_WRAMADDR, addr);
	VS1063WriteSCI(SCI_WRAM, data);
}

/*
  Read 16-bit value from addr.
*/
unsigned int VS1063ReadMem(unsigned int addr)
{
	VS1063WriteSCI(SCI_WRAMADDR, addr);
	return VS1063ReadSCI(SCI_WRAM);
}

void VS1063WriteSCI(unsigned char addr, unsigned int data)
{
	unsigned int i;

	while (GPIOestaActivo(&DREQ_VS1063) == 0)		// Wait until DREQ is high
	{
		i ++;
		if (i > 10000)
			break;
	}

	GPIOpasivarPin(&xCS_VS1063); // Activate xCS
	SSPSendRecieveByte(SPI_VS1063, 2); // Write command code
	SSPSendRecieveByte(SPI_VS1063, addr); // SCI register number
	SSPSendRecieveByte(SPI_VS1063, (unsigned char)(data >> 8));
	SSPSendRecieveByte(SPI_VS1063, (unsigned char)(data & 0xFF));
	GPIOactivarPin(&xCS_VS1063); // Deactivate xCS
}

unsigned int VS1063ReadSCI(unsigned char addr)
{
	unsigned int res;
	unsigned int i;

	while (GPIOestaActivo(&DREQ_VS1063) == 0)		// Wait until DREQ is high
	{
		i ++;
		if (i > 10000)
			break;
	}

	GPIOpasivarPin(&xCS_VS1063); // Activate xCS
	SSPSendRecieveByte(SPI_VS1063, 3); // Read command code
	SSPSendRecieveByte(SPI_VS1063,addr); // SCI register number
	res = (unsigned int)SSPSendRecieveByte(SPI_VS1063, 0x00) << 8;
	res |= SSPSendRecieveByte(SPI_VS1063, 0x00);
	GPIOactivarPin(&xCS_VS1063); // Deactivate xCS

	return res;
}

int VS1063WriteSDI(const unsigned char *data, unsigned char bytes)
{
	unsigned char i;
	if (bytes > 32)
		return -1; // Error: Too many bytes to transfer!

	while (GPIOestaActivo(&DREQ_VS1063) == 0); // Wait until DREQ is high

	GPIOpasivarPin(&xDCS_VS1063); // Activate xDCS
	for (i=0; i<bytes; i++)
	{
		SSPSendRecieveByte(SPI_VS1063, *data++);
	}
	GPIOpasivarPin(&xDCS_VS1063); // Dectivate xDCS

	return 0; // Ok
}

void VS1063SineTest (void)
{
	unsigned int register_bk;

	while (GPIOestaActivo(&DREQ_VS1063) == 0); // Wait until DREQ is high

	register_bk = VS1063ReadSCI (SCI_MODE);
	register_bk |= SM_TESTS;
	VS1063WriteSCI (SCI_MODE, register_bk);

	dataBuffer[0] = 0x53;
	dataBuffer[1] = 0xEF;
	dataBuffer[2] = 0x6E;
	dataBuffer[3] = 0x7E;
	dataBuffer[4] = 0x00;
	dataBuffer[5] = 0x00;
	dataBuffer[6] = 0x00;
	dataBuffer[7] = 0x00;
	dataBuffer[8] = 0x00;
	dataBuffer[9] = 0x00;
	dataBuffer[10] = 0x00;
	dataBuffer[11] = 0x00;
	dataBuffer[12] = 0x00;
	dataBuffer[13] = 0x00;
	dataBuffer[14] = 0x00;
	dataBuffer[15] = 0x00;

	VS1063WriteSDI (dataBuffer, 16);

}

void LoadPlugin(const u_int16 *d, u_int16 len)
{
  int i = 0;

  while (i<len)
  {
	unsigned short addr, n, val;
	addr = d[i++];
	n = d[i++];
	if (n & 0x8000U)
	{ /* RLE run, replicate n samples */
	  n &= 0x7FFF;
	  val = d[i++];
	  while (n--) {
		  VS1063WriteSCI(addr, val);
	  }
	}
	else
	{           /* Copy run, copy n samples */
	  while (n--) {
		val = d[i++];
		VS1063WriteSCI(addr, val);
	  }
	}
  }
}
