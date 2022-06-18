
#include "msp.h"


/**
 * main.c
 * Configuracion de msp432 como esclavo
 * transmisor  de 5 datos al maestro
 */
uint8_t TXData;
uint8_t TXDataSize;
#define TX_DATA_SIZE    5
void main(void)
{
    volatile uint32_t i;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Seleccionamos funcion i2c
	P1 ->SEL0 |= BIT6 + BIT7;
	// Inicializamos
	TXData = 0;
	TXDataSize = 0;
	// Habilitamos interrupcion
	__enable_irq();
	// Habilitamos vector correcpondiente
	NVIC ->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);

	//Configuramos modulo
	EUSCI_B0 ->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Colocamos reset

	EUSCI_B0 ->CTLW0 = EUSCI_B_CTLW0_SWRST +
	        EUSCI_B_CTLW0_MODE_3 +          //Modo i2c
	        EUSCI_B_CTLW0_SYNC;           //modo sincrono

	EUSCI_B0 ->I2COA0 = 0X48 + EUSCI_B_I2COA0_OAEN; // indica y habilita direccion del esclavo

	EUSCI_B0 ->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;   // libera los estados de maquina

	EUSCI_B0 ->IE |= EUSCI_B_IE_TXIE0 +          // habilita interrupcion por Rx
	        EUSCI_B_IE_STPIE;
	SCB ->SCR |= SCB_SCR_SLEEPONEXIT_Msk;

    __DSB();
    __sleep();
    __no_operation();
}
void EUSCIB0_IRQHandler(void){

    if(EUSCI_B0 ->IFG & EUSCI_B_IFG_STPIFG){   // verifica que no hay reconocimiento
        EUSCI_B0 ->IFG &= ~EUSCI_B_IFG_STPIFG;    // limpiamos bandera
        TXDataSize = 0;          // reset al puntero
    }
    else if (EUSCI_B0 ->IFG & EUSCI_B_IFG_TXIFG0){ // Verifica si se envio el dato
        if(TXDataSize < TX_DATA_SIZE){
            EUSCI_B0 ->TXBUF = TXData++;    //envia el siguiente dato
        }
        else{
            EUSCI_B0 ->TXBUF =0;
        }
        TXDataSize++;
    }
}

