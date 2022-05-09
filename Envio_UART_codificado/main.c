
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};


//Dato a enviar
const static uint8_t Dato [16] ={0x00, 0x01, 0x02, 0x03,
                            0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0a, 0x0b,
                            0x0c, 0x0d, 0x0e, 0x0f};

// Llave de cifrado
const static uint8_t CipherKey [32] = {0x00, 0x01, 0x02, 0x03,
                                 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b,
                                 0x0c, 0x0d, 0x0e, 0x0f,
                                 0x00, 0x01, 0x02, 0x03,
                                 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0a, 0x0b,
                                 0x0c, 0x0d, 0x0e, 0x0f
                                };
// Variables para sostener dato encriptado y desencriptado
static uint8_t DatoAESencrip [16];
static uint8_t DatoAESdesencrip [16];



int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    /* DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    //Interrupt_enableSleepOnIsrExit();
    Interrupt_enableMaster();
    // Cargamos la llave al modulo AES con longitud 256
    AES256_setCipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);
    // Encriptacion de dato y almacenamiento en DatoAESencrip
    AES256_encryptData(AES256_BASE, Dato, DatoAESencrip);
    // Des encriptacion de DatoAESencrip y almacenado en DatoAESdesencrip
    AES256_setDecipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);
    AES256_decryptData(AES256_BASE, DatoAESencrip, DatoAESdesencrip);

GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

int n=0;

    while(1)
    {
       n=0;
      if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_LOW){
          __delay_cycles(100000);
                    while (n<15){
                     UART_transmitData(EUSCI_A0_BASE,DatoAESencrip[n]);

                     n++;
      }

       }
       // UART0_OutString((char*)Dato + Null)

       // UART_transmitData(EUSCI_A0_BASE, i);
        /* Modo bajo consumo */
      //  PCM_gotoLPM0();
    }
}

/*Funcion para envio de string */



void UART0_OutString(char *pt){
    while(*pt){
        UART0_OutChar(*pt);
        pt++;
    }
}
void UART0_OutChar(char letra){
    UART_transmitData(EUSCI_A0_BASE, letra);
}

/* Servicio interrupción EUSCIA_0 */
void EUSCIA0_IRQHandler(void)
{

    /* Se envia dato recibido por Tx */
    uint8_t i=0;
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        i=UART_receiveData(EUSCI_A0_BASE);
        UART_transmitData(EUSCI_A0_BASE, i);


    }
}

