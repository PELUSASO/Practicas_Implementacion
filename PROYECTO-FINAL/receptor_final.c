
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

volatile uint8_t j=0;
volatile uint8_t j2=0;
volatile uint8_t j3=0;

volatile uint8_t dataencripReady=0;
volatile uint8_t cont=0;

//Variables

const static uint8_t Dato_Resuelto[16]={0x31, 0x32, 0x31, 0x33,
                         0x31, 0x39, 0x38,
                         0x39, 0x30, 0x37,
                         0x31, 0x39, 0x38,
                         0x31, 0x36, 0x39};
static uint8_t CipherKey[32]={0x00, 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F,
                              0x00, 0x01, 0x02, 0x03,
                              0x04, 0x05, 0x06, 0x07,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F
                             };
static uint8_t Dato [16];
const static uint8_t DatoAESdesencrip [16];


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

void configUART(){
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

    // MAP_WDT_A_holdTimer();
    WDT_A -> CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN4);;
    P10-> DIR |= BIT4|BIT5;
    P10-> OUT &= ~(BIT4|BIT5);
    P10-> SEL0 |=BIT4|BIT5;

    configUART();

    // Habilitamos acceso a esctritura al registro CS

        CS -> KEY = CS_KEY_VAL;
    // Configura el divisor de señal SMLCK/8
        CS -> CTL1 |= CS_CTL1_DIVA_3;

        // Deshabilitamos acceso a esctritura al registro CS
        CS -> KEY = 0;
        //Timer A3 de modo ascendente, fuente de reloj SMCLK ) 3MHz
        //Limpieza del TAR
    TIMER_A3 ->CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR ;
        //Cargar base de tiempo en CCR0
    TIMER_A3->CCR[0] = 60000;
    //Modo toggle registro captura comparación
    TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_4;
    TIMER_A3 -> CCTL [1] = TIMER_A_CCTLN_OUTMOD_7;

    TIMER_A3->CCR[1] = 18000;

    while(1)
    {
        if(dataencripReady==1){



            /*Desencriptación de DATOASencript y almancenado en DATOASencript */
            AES256_setDecipherKey(AES256_BASE, CipherKey, AES256_KEYLENGTH_256BIT);
            /*Desencriptación de DATOASencript y almancenado en DATOASdesencript */
            AES256_decryptData(AES256_BASE, Dato, DatoAESdesencrip);

            for(j2=0;j2<16;j2++){
                if(DatoAESdesencrip[j2]==Dato_Resuelto[j2]){
                    j3++;
                }
            }
            if(!(P1IN & BIT4)){
                j=0;
                dataencripReady=0;
                while(!(P1IN & BIT4));
            }

            if(j3==16){
                TIMER_A3->CCR[1] = 1000;
                UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
                GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN7);
                GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN6);
                j3=0;
            }
                   else
                   {
                       TIMER_A3->CCR[1] = 18000;
                       GPIO_setDriveStrengthHigh(GPIO_PORT_P2, GPIO_PIN0);
                       GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN6);
                       GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN7);
                       j3=0;
                       //UART_clearInterruptFlag(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
                   }
                   cont=0;
                    j=0;
                   dataencripReady=0;
               }
        }

}
void EUSCIA0_IRQHandler(void)
{

    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    /* Se envia dato recibido por Tx */
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        if(j<=15){
            Dato[j]=UART_receiveData(EUSCI_A0_BASE);
            j++;
            if(j==15){
                dataencripReady = 1;
            }
        }
        
    }

}


