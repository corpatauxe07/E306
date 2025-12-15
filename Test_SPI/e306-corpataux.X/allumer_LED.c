/*******************************************************************************
 * File:    allumer_LED.c
 * Author:  Dach Tim
 * Company: EMF (Ecole des Métiers de Fribourg) http://www.emf.ch
 * Desc.:   Programme qui allumer une led sur la platine didactique
 * Date :   20241205 Création (YYYYMMDD)            
 ******************************************************************************/

//----- Including libraries --
#include <xc.h>

// PIC16F1788 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config DEBUG = OFF      // In-Circuit Debugger Mode (In-Circuit Debugger disabled, ICSPCLK and ICSPDAT are general purpose I/O pins)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
//---- IO configuration --------------------------------
#define TRISA_CONFIG 0b00000000 //Tout des sortie de X3 et X4
#define TRISB_CONFIG 0b11000000 //RB7 et RB6 sont des pin ICSPDAT et ICSPCLK
                //les autres sont conecter avec les led
#define TRISC_CONFIG 0b00000111 //RC0 à RC2 est des switch

#define LATA_CONFIG 0b00000000  
#define LATB_CONFIG 0b00111100
#define LATC_CONFIG 0b00000000

#define ANSEL_CONFIG   0    //tout en digital

//----- Constant Definiton  ----------------------------
#define D4 LATAbits.LATA0
#define D5 LATAbits.LATA1
#define D6 LATAbits.LATA2
#define D7 LATAbits.LATA3

#define ENABLE LATAbits.LATA4
#define RS LATBbits.LATB0
#define R_W LATBbits.LATB1

#define LED2_GREEN LATBbits.LATB5
#define LED2_RED LATBbits.LATB4
#define LED1_GREEN LATBbits.LATB3
#define LED1_RED LATBbits.LATB2

#define SWITCH_S0 PORTCbits.RC0
#define SWITCH_S1 PORTCbits.RC1
#define SWITCH_S2 PORTCbits.RC2

#define ON 0
#define OFF 1

/* Global Variable Definitions ************************************************/

/* Function Declarations ******************************************************/
void PIC_Initialize(void);
void OSC_Initialize(void);


/* Initialize Function Implementations ****************************************/
void SYS_InitAll(void)
{
    PIC_Initialize();
    OSC_Initialize();
}

void PIC_Initialize(void)
{
    //configure tout des PORT travailer en mode binaire
    ANSELA = ANSEL_CONFIG;
    ANSELB = ANSEL_CONFIG;
    ANSELC = ANSEL_CONFIG;
    
    //protecter les PORT
    LATA = LATA_CONFIG;
    LATB = LATB_CONFIG;
    LATC = LATC_CONFIG;
    
    //fait le I/O sur des PORT
    TRISA = TRISA_CONFIG;
    TRISB = TRISB_CONFIG;
    TRISC = TRISC_CONFIG;
}

void OSC_Initialize(void)
{
    //IRCF<3:0>: Internal Oscillator Frequency Select bits
    //1111 = 16 MHz HF or 32 MHz HF(2)
    //1110 = 8 MHz or 32 MHz HF(2)
    //1101 = 4MHz HF
    //1100 = 2MHz HF
    //1011 = 1MHz HF
    //1010 = 500 kHz HF(1)
    //1001 = 250 kHz HF(1)
    //1000 = 125 kHz HF(1)
    //0111 = 500 kHz MF (default upon Reset)
    //0110 = 250 kHz MF
    //0101 = 125 kHz MF
    //0100 = 62.5 kHz MF
    //0011 = 31.25 kHz HF(1)
    //0010 = 31.25 kHz MF
    //000x = 31kHz LF
    OSCCONbits.IRCF = 0b1101;
    
    //SCS<1:0>: System Clock Select bits
    //1x = Internal oscillator block
    //01 = Timer1 oscillator
    //00 = Clock determined by FOSC<2:0> in Configuration Words.
    OSCCONbits.SCS = 0b10;
}


/* Function Implementations ***************************************************/


/* Main Function Implementation ***********************************************/
void main(void)         
{   
    /* Local Variable Definition*/
    
    /* Initialisation */
    SYS_InitAll();                      // Initialiser le système
    
    while(1)
    {
        if(SWITCH_S0 == ON)
        {
            LED1_GREEN = ON;
        }
        else
        {
            LED1_GREEN = OFF;
        }
        
        if(SWITCH_S1 == ON)
        {
            LED1_RED = ON;
        }
        else
        {
            LED1_RED = OFF;
        }
        
        if(SWITCH_S2 == ON)
        {
            LED2_GREEN = ON;
        }
        else
        {
            LED2_GREEN = OFF;
        }
        
        if((SWITCH_S1 == OFF)&&(SWITCH_S2 == OFF)&&(SWITCH_S0 == OFF))
        {
            LED2_RED = ON;
        }
        else
        {
            LED2_RED = OFF;
        }
    }
    /* Safeguard */ 
    while (1);                          // Garde-Fou  ->  Boucle infinie
}

/**************************************************************** End-Of-File */