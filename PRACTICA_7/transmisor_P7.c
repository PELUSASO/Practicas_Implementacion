
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

/* Para cambiar a 115200 baudios
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        6,                                     // BRDIV = 6
        8,                                       // UCxBRF = 8
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};
*/
// Variables
static uint8_t CipherKey [32] = {0x00, 0x01, 0x02, 0x03,
                                 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B,
                                 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x00, 0x01, 0x02, 0x03,
                                 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B,
                                 0x0C, 0x0D, 0x0E, 0x0F};

static uint8_t Dato[16]={0x01, 0x02, 0x03, 0x04,
                         0x05, 0x06, 0x07, 0x08,
                         0x09, 0x0A, 0x0B, 0x0C,
                         0x0D, 0x0E, 0x0F, 0x02};

static uint8_t DatoAESencrip [16];
static uint8_t DatoAESdesencrip [16];
int j=0,j2;
bool aux = false;
bool comp = true;

void LED_init(void){
    P1->SEL0 &= ~0X01;  //Configuración de entradas y salidas
    P1->SEL1 &= ~0X01;
    P1->DIR |= 0x01;    
    P1->OUT &= ~0x01;
}

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /*Desencriptacion de DatoAESencrip y almacenado en DatoAESdesencrip */
    AES256_setDecipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);

    //Inicializar LED
    LED_init();

    /*Configruación UART0*/
    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);


    /*Configruación UART 2*/
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    /* Habilitamos UART */
    UART_enableModule(EUSCI_A2_BASE);
    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA2);


    while(1)
    {
        if(aux==true){  // Se comprueba de que se terminaron de pasar datos para poder comparar

            /*DesEncriptacion de Dato y almacenado en DatoAESdesencrip*/
            AES256_decryptData(AES256_BASE, DatoAESencrip, DatoAESdesencrip);
            aux=false; // Cambio de estado de bandera auxiliar
            for(j2=0;j2<=15;j2++){
                UART_transmitData(EUSCI_A0_BASE, DatoAESdesencrip[j2]); // Se transmite el dato para ver que funcione en el monitor serie de Arduino

                if(Dato[j2]!=DatoAESdesencrip[j2]){    //si un dato es distinto se cambia el estado de la bandera comp
                    comp=false;                    
                }
            }
            if(comp==true){                 //Si todo es correcto se prende un led
                P1->OUT ^= 0x01;            
            }
            comp=true;                      
        }
    }
}

// Servicio interrupción EUSCIA_2 UART2 
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    /* Se envia dato recibido por Tx */
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        DatoAESencrip[j]=UART_receiveData(EUSCI_A2_BASE);   //se guardan los datos recibidos en DatoAESencrip
        j++;
        if(j>15){           
            j=0;            
            aux = true;     //Cambio de estado de auxiliar
        }
    }
}
