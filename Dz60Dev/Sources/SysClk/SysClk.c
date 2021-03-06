/* for EnableInterrupts macro */
#include <hidef.h> 
/* include peripheral declarations */
#include "derivative.h" 

/*
 * Coding by Grey at home. 2017.10.23 in Beijing.
 * Config system clock.
 * Use external OSR as the system clock.
 */
void SysClkInit(void) 
{
	/*
		Select external clock with a range between 1MHz~16Mhz.
		Configure external oscillator for high gain operation.
		Oscillator requested.
		MCGERCLK active.
	*/
	MCGC2=0x36;
	/*
		Stay here if OSC is not ok.
	*/
	while(!MCGSC_OSCINIT);
	/*
		- CLKS(bit7,6) set to 0b10 in order to select external 
			reference clock as system clock source.
		- RDIV(bit 5-3) set to 0b111, or divide-by-128 because 
		4 MHz / 128 = 31.25kHz which is in the 31.25kHz to 39.0625kHz 
		range by FLL.
		- IREFS(bit 2)cleard to 0, selecting the external reference clock.
	*/
	MCGC1=0xb8;
	/*
		Stay here if the reference clock is not the external clock.
	*/
	while(MCGSC_IREFST);
	/*
		Stay here if the external reference clock in not selected.
	*/
	while(MCGSC_CLKST!=0b10);
	/*
		FLL(or PLL) is disabled when in bypass mode(lower power).
	*/
	MCGC2_LP = 1;
	/*
		RDIV (bits 5-3) set to %010, or divide-by-4 because 
		4 MHz / 4 = 1 MHz which is in the 1 MHz to 2 MHz range required
		by the PLL. In BLPE mode, the configuration of the RDIV
		does not matter because both the FLL and PLL are disabled. 
		Changing them only sets up the the dividers for PLL usage in PBE mode.
	*/
	MCGC1 = 0x90;
	/*
		�C PLLS (bit 6) set to 1, selects the PLL. In BLPE mode, 
		changing this bit only prepares the MCG for PLL usage in PBE mode.
		�C VDIV (bits 3-0) set to %1000, or multiply-by-32 because 1 MHz 
		reference * 32 = 32 MHz.(40MHz most when MUL 40)
		In BLPE mode, the configuration of the VDIV bits does not matter 
		because the PLL is disabled. Changing them only sets up the multiply 
		value for PLL usage in PBE mode.
	*/
	MCGC3 = 0x48;
	/*
		FLL(or PLL) is not disabled when in bypass mode(lower power).
		In other words: enable FLL(or PLL) when in bypass mode.
	*/
	MCGC2_LP = 0;
	/*
		Stay here if the source of PLLS is FLL clock.
		This can also be achieved by checking if the souce if PLLS is
		PLL clock.
	*/
	while(!MCGSC_PLLST);
	/*
		Check if the PLL is locked.
	*/
	while(!MCGSC_LOCK);
	/*
		�C CLKS (bits7 and 6) in MCGSC1 set to 0b00 in order to select 
		the output of the PLL as the system clock source.
	*/
	MCGC1 = 0x10;
	/*
		�C Loop until CLKST (bits 3 and 2) in MCGSC are 0b11, indicating 
		that the PLL output is selected to feed MCGOUT in the current
		clock mode
	*/
	while(MCGSC_CLKST!=0b11);

	/*
		Calculation for system clock:
		4MHz / 4 * 32 = 32MHz. The bus clock then would be 16MHz.
	*/
} 

