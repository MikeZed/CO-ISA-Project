#include "config.h"
#include "lcd.h"
#include "led.h"
#include "sim.h"
//#include "timed.h"
#include "utils.h"
#include "rgbled.h"
#include "btn.h"
#include <xc.h>  
#include <sys/attribs.h>
#include <string.h>
     
//==================================================
//              Global Configuration
//==================================================
// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	PRIPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

 #define _XTAL_FREQ 8000000




void Timer4Setup()
{
    static int fTimerInitialised = 0;
    if(!fTimerInitialised)
    {        
                                          //  setup peripheral
        PR4 = 10000;                        //             set period register, generates one interrupt every 1 ms
        TMR4 = 0;                           //             initialize count to 0
        T4CONbits.TCKPS = 3;                //            1:256 prescale value
        T4CONbits.TGATE = 0;                //             not gated input (the default)
        T4CONbits.TCS = 0;                  //             PCBLK input (the default)
        T4CONbits.ON = 1;                   //             turn on Timer1
        IPC4bits.T4IP = 2;                  //             priority
        IPC4bits.T4IS = 0;                  //             subpriority
        IFS0bits.T4IF = 0;                  //             clear interrupt flag
        IEC0bits.T4IE = 1;                  //             enable interrupt
        fTimerInitialised = 1;
    }
}

//==============================================================================
//Interrupt Handler- handled every 1msec
//==============================================================================
void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void)
{
    IFS0bits.T4IF = 0;                  // clear interrupt flag
}

//==============================================================================
//this function initializes the needed components
//==============================================================================
void init(){
    LCD_Init(); 
    LED_Init();
    BTN_Init();
    REG_Init();
    RGBLED_Timer5Setup();
    Timer4Setup();
    pc=0;
    counter=0;
    num_of_reg = 0;
    address_Init();
    IOREG_Init();
    
}






void main(){
    init(); //initialize all needed components
    
    if(SWT_GetValue(7))
    {
        IEC0bits.T4IE = 0; 
        sim(fib);        
    }
    else
    {
        sim(stopper);
    }
    
}






