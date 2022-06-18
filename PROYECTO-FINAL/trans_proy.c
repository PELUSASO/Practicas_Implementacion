
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//Variables
volatile uint8_t j=0;
volatile uint8_t dataencripReady=0;
volatile uint8_t cont=0;
 uint8_t con2=0;
// Clave de cifrado
static uint8_t CipherKey[32]={0x00, 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F,
                              0x00, 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F
                             };
volatile uint8_t Dato [16];
volatile uint8_t DatoAESencrip [16];

// Configuración de UART para 9600 baudios
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

void configUART(){ // Funcion para configurar UART
    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );

    /* DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableMaster();

}
int main(void)


{
    /* Stop Watchdog  */

    MAP_WDT_A_holdTimer();

// Se inicializan los pines de los LED
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
// Se inicializa la configuración del UART
    configUART();

    while(1)
    {
        //Verifica que la contraseña este completamente leido
        if(dataencripReady==1){
            // Verifica si el pin 4 del puerto 1, se presiono y reinicia el contador j para ingresar una nueva contraseña
            if(!(P1IN & BIT4)){
                j=0;
                dataencripReady = 0;
                while(!(P1IN & BIT4));

            }
            // Verifica si el pin 1 del puerto 1, se presiono y codifica el dato y transmite la codificación
            if(!(P1IN & BIT1)){
                AES256_setCipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);
                /*Encriptacion de Dato y almacenado en DATOAESencript*/
                AES256_encryptData(AES256_BASE, Dato, DatoAESencrip);

                   for (j=0;j<16;j++){
                       // Transmisión de dato encriptado dato por dato
                       UART_transmitData(EUSCI_A0_BASE, DatoAESencrip[j]);
                   }
                   while(!(P1IN & BIT1));
               }
        }
    }
}
void EUSCIA0_IRQHandler(void) //Inrerrupcion de recepcion UART
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
       con2=con2++;
// Se reciben 16 datos por UART y se guardan en Dato
       if(j<=15){
            Dato[j]=UART_receiveData(EUSCI_A0_BASE);
            j++;
            if(j==15){
                dataencripReady = 1;
                UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
            }
        }
    }
}

