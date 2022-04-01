#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
   int cont=0x00;
    int i=0x00;
    bool aut=true;
    bool botonbajo=true;
    bool botonbajo2=true;
    bool aux=true;
   int aux2=0x00;
   int aux3=0x00;

/**
 * main.c
 * Genere una señal PWM simetric ad e 20kHZ
 * Fuente de reloj SMCLK
 * Señal de salida P10.4 (TA3.0)
 * DCOCLK = 3MHz -> SMCLK -> Timer Clock
 * CCR = 75
 *
 */
int estado=0;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
 //Configuracion de ssalida p10.4
	P10->DIR |= BIT5;
	//Seleccion de funcion periferica primaria
	P10 -> SEL0 |= BIT5;

	//Confuguracion entrada P1.1
	P1REN=0b0000000010;
	P1OUT=0b0000000010;
	// Habilitamos acceso a esctritura al registro CS

	CS -> KEY = CS_KEY_VAL;


	// Deshabilitamos acceso a esctritura al registro CS
	CS -> KEY = 0;
	//Timer A3 de modo ascendente, fuente de reloj SMCLK ) 3MHz
	//Limpieza del TAR
TIMER_A3 ->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR ;
	//Cargar base de tiempo en CCR0
TIMER_A3->CCR[0] = 12000;

//Modo toggle registro captura comparación
TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_4;
TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_7;
//





for(;;){
if()
    if(cont==1)
}

}
