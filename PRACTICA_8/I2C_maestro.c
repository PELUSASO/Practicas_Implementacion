

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Definicion de variables
volatile uint8_t RxData[5]={0};
volatile uint8_t RxDataPointer;


int main(void)
{
    volatile uint32_t i;
    // detenemos watch dog
    WDT_A -> CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    // Configuracion de gpioS
    P1 -> OUT &= ~BIT0;
    P1 -> DIR |= BIT0;
    // Confuguracion de pull ups
    P1 -> REN |= BIT6 + BIT7;
    P1 -> OUT |= BIT6 + BIT7;

    // Seleccionamos funcion periferica I2C
    P1-> SEL0 |= BIT6 + BIT7;
    // Inicializamos al puntero del buffer
    RxDataPointer =0;

    // Interrupcion Global
    __enable_irq();
    // Vector de interrupcion de EUSCIB0
    NVIC -> ISER[0] = 1 << ((EUSCIB0_IRQn)& 31);
    // Configuracion del modulo EUSCIB0 en el modo I2C receptor
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST; // Reset de los estados
    EUSCI_B0 -> CTLW0 = EUSCI_B_CTLW0_SWRST+
            EUSCI_B_CTLW0_MODE_3 + //Modo I2C
            EUSCI_B_CTLW0_MST + // Modo maestro
            EUSCI_B_CTLW0_SYNC + // Modo sincrono
            EUSCI_B_CTLW0_SSEL__SMCLK; // Fuente de reloj

    // Genera una condicion de stop en forma automatica
    EUSCI_B0 -> CTLW1 |= EUSCI_B_CTLW1_ASTP_2;
    // Razon de baudios =smclk /30
    EUSCI_B0 -> BRW = 30;
    // Numero de bytes a recibir
    EUSCI_B0 -> TBCNT =5;
    // Direccion del esclavo
    EUSCI_B0 -> I2CSA = 0X048;
    // libera estados de máquina
    EUSCI_B0 -> CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
    // interrupcion por recepcion de datos, por no reconocimiento de nack y por conteo de bites recibidos
    EUSCI_B0 -> IE |= EUSCI_B_IE_RXIE +
            EUSCI_B_IE_NACKIE +
            EUSCI_B_IE_BCNTIE;

    while(1)
    {
        // Procesador dormido al salir de la rutina de interrupcinon
        SCB -> SCR |= SCB_SCR_SLEEPONEXIT_Msk;
        // aseguramos que tenga efecto inmediato la funcion anterior
        __DSB();
        // Retardo crudo
        for (i=2000; i>0; i--);
        // Garantizamos una condicion de stop para enviar
        while(EUSCI_B0 -> CTLW0 & EUSCI_B_CTLW0_TXSTP);
        // Inicia la condicion de start
        EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTT;
        // modo LPM0
        __sleep();
        __no_operation();

    }
}

// Sevicio de interrupcion EUSCIB_0
void EUSCIB0_IRQHandler(void)
{
    // Verificamos si hay reconocimiento de nack
    if(EUSCI_B0 -> IFG & EUSCI_B_IFG_NACKIFG){
        // Limpieza interrupcion
        EUSCI_B0 -> IFG &= ~EUSCI_B_IFG_NACKIFG;
        // inicia condicion de start
        EUSCI_B0 -> CTLW0 |=EUSCI_B_CTLW0_TXSTT;
    }
// Verifica si llego dato nuevo
    else if(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG0){
        // Limpiamos bandera de interrupcion
        EUSCI_B0 -> IFG &= ~EUSCI_B_IFG_RXIFG0;
        // Leer y guardar el siguiente dato recibido
        RxData [RxDataPointer++]=EUSCI_B0 -> RXBUF;
        // Verificamos si se lleno el buffer de recepcion
        if(RxDataPointer > sizeof (RxData)){
            RxDataPointer =0; // limpiamos el puntero
        }
        // -- Inicio de codiog opcional
        // Dspierta al salir de la rutina de servicio de interrupcion
        SCB -> SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
        // Se garantiza que la orden anterior tenga efecto inmediato
        __DSB();
        // --- Fin de condigo opcional
    }
    // Verificamos si ya se completo el numero de butes a recibir
    else if(EUSCI_B0 -> IFG & EUSCI_B_IFG_BCNTIFG){
        EUSCI_B0 -> IFG &= ~EUSCI_B_IFG_BCNTIFG; // limpiamos bandera de interrupcion
    }
}

