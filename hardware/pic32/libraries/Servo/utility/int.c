/************************************************************************/
/*																		*/
/*	int.c	--	Handles timer interrupts for PIC32      				*/
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu                                             */
/*                          											*/
/*	Copyright 2011, Digilent Inc.										*/
/*  This library is free software; you can redistribute it and/or       */
/*  modify it under the terms of the GNU Lesser General Public          */
/*  License as published by the Free Software Foundation; either        */
/*  version 2.1 of the License, or (at your option) any later version.  */
/*                                                                      */
/*  This library is distributed in the hope that it will be useful,     */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   */
/*  Lesser General Public License for more details.                     */
/*                                                                      */
/*  You should have received a copy of the GNU Lesser General Public    */
/*  License along with this library; if not, write to the Free Software */
/*  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA           */
/*  02110-1301 USA                                                      */
/************************************************************************/
/*  File Description:													*/
/*	This file declares functions that handle timer interrupts for       */
/*  chipKIT PIC32 boards.												*/
//************************************************************************
//*	Edit History
//************************************************************************
//*	Aug 18,	2011	<MichelleY> file header comment block reformatted
//*	Sep  5,	2011	<GeneA> Added include of p32xxxx.h and plib.h to fix
//*						compile errors intoduced when include of plib.h was removed
//*						HardwareSerial.h
//* Feb  7, 2013	<GeneApperson> Removed dependency on Microchip plib library
//************************************************************************

#define OPT_SYSTEM_INTERNAL
#define OPT_BOARD_INTERNAL
#include <p32xxxx.h>
#include <sys/attribs.h>
#include <pins_arduino.h>

#include "int.h"

// M00BUG: This is hard coded for specific registers for interrupt priority
//	flag bits and enable bits. This code happens to work correctly for all
//  currently existing PIC32 devices, but this needs to be rewritten to be
//	more generic.

#if defined(__PIC32MZXX__)
void __attribute__((nomips16,vector(_TIMER_3_VECTOR),interrupt(_T3_IPL_ISR))) T3_IntHandler(void)
#else
void __attribute__((interrupt(),nomips16)) T3_IntHandler (void)
#endif
{
 	handle_interrupts(TIMER3, &TMR3, &PR3); 
	IFS0bits.T3IF = 0; // Clear timer interrupt status flag
}


#if defined(__PIC32MZXX__)
void __attribute__((nomips16,vector(_TIMER_3_VECTOR),interrupt(_T3_IPL_ISR))) T3_IntHandler(void)
#else
void __attribute__((interrupt(),nomips16)) T4_IntHandler (void)
#endif
{
 	handle_interrupts(TIMER4, &TMR4, &PR4); 
	IFS0bits.T4IF = 0; // Clear timer interrupt status flag
}

#if defined(__PIC32MZXX__)
void __attribute__((nomips16,vector(_TIMER_3_VECTOR),interrupt(_T3_IPL_ISR))) T3_IntHandler(void)
#else
void __attribute__((interrupt(),nomips16)) T5_IntHandler (void)
#endif
{
 	handle_interrupts(TIMER5, &TMR5, &PR5); 
	IFS0bits.T5IF = 0; // Clear timer interrupt status flag
}

void initISR(int timer)
{
    if(timer == TIMER3)
    {
        // set the vector up
        setIntVector(_TIMER_3_VECTOR, T3_IntHandler);

         //timer 3 set clock period 20ms
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
        T3CONbits.TCKPS = 0b101; // set prescalar 1:32
#else
        T3CONbits.TCKPS = 0b110; // set prescalar 1:64
#endif
        TMR3 = 0;
        PR3 = 0x61A8;        
           
	    IFS0bits.T3IF = 0;// Clear the T3 interrupt flag 
	    IEC0bits.T3IE = 1;// Enable T3 interrupt 
     
		IPC3CLR = 0x0000001F;
	    IPC3SET = (_T3_IPL_IPC << 2) | _T3_SPL_IPC;
       
	    T3CONbits.ON = 1;// Enable Timer3
    }
    if(timer == TIMER4)
    {
        // set the vector up
        setIntVector(_TIMER_4_VECTOR, T4_IntHandler);
 
        //timer 4 set clock period 20ms 
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
        T4CONbits.TCKPS = 0b101; // set prescalar 1:32
#else
        T4CONbits.TCKPS = 0b110; // set prescalar 1:64
#endif
        TMR4 = 0;
        PR4 = 0x61A8;        
           
	    IFS0bits.T4IF = 0;// Clear the T4 interrupt flag 
	    IEC0bits.T4IE = 1;// Enable T4 interrupt 
     
		IPC4CLR = 0x0000001F;
	    IPC4SET = (_T4_IPL_IPC << 2) | _T4_SPL_IPC;   
       
	    T4CONbits.ON = 1;// Enable Timer4	 
    }
    if(timer == TIMER5)
    {
        // set the vector up
        setIntVector(_TIMER_5_VECTOR, T5_IntHandler);

        //timer 5 set clock period 20ms 
#if defined(__PIC32MX1XX__) || defined(__PIC32MX2XX__)
        T5CONbits.TCKPS = 0b101; // set prescalar 1:32
#else
        T5CONbits.TCKPS = 0b110; // set prescalar 1:64
#endif
        TMR5 = 0;
        PR5 = 0x61A8;        
           
	    IFS0bits.T5IF = 0;// Clear the T5 interrupt flag 
	    IEC0bits.T5IE = 1;// Enable T5 interrupt 
     
		IPC5CLR = 0x0000001F;
	    IPC5SET = (_T5_IPL_IPC << 2) | _T5_SPL_IPC;
       
	    T5CONbits.ON = 1;// Enable Timer5
    }
    
} 

