
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static uint8_t i = 0;
static uint8_t cont=0;
char msg[30];

static volatile uint16_t ValorADC;
static volatile float VoltajeNormalizado;
static volatile float y;
static volatile float y_n;
static volatile float apha = 0.5;
static volatile float k;
static volatile float fc;
static volatile float x;
static volatile float aux=0;
static volatile float suma=0;


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


int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();
    ValorADC=0;
    VoltajeNormalizado=0;

    /*Configura Flash estado de espera*/
    FlashCtl_setWaitState(FLASH_BANK0,1);
    FlashCtl_setWaitState(FLASH_BANK1,1);

    /*Habilitamos FPU*/
    FPU_enableModule();
    FPU_enableLazyStacking();

    /*Configuración ADC*/
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_32, ADC_DIVIDER_8, ADC_NOROUTE);

    /*GPIO Configuracion*/
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION);

    /*Configurar ADC Memoria*/
    ADC14_configureSingleSampleMode(ADC_MEM0, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, ADC_NONDIFFERENTIAL_INPUTS);

    /*Configuración timer en modo manual*/
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

    /* P1.2 and P1.3 en modo UART  */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* DCO to 12MHz */
    PCM_setPowerState(PCM_AM_LDO_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Configuración UART con base a la estructura de arriba */
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    /* Habilitamos UART */
    UART_enableModule(EUSCI_A0_BASE);

    /* Interrupciones */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);

    ADC14_enableInterrupt(ADC_INT0);
    Interrupt_enableInterrupt(INT_ADC14);

    Interrupt_enableMaster();



    while(1)
    {
        __delay_cycles(6000000);
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
}
}

void ADC14_IRQHandler(void){
    uint16_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(ADC_INT0);
    if(ADC_INT0 & status){
        ValorADC = ADC14_getResult(ADC_MEM0);
        VoltajeNormalizado = (ValorADC*3.3)/16383;
        // Filtro EMA
        y=(VoltajeNormalizado*apha)+((1-apha)*y_n);
        y_n=y;
        aux+=VoltajeNormalizado;
        // El otro filtro
        suma=aux/cont;
        cont++;
        k=1;
        fc=1200*k;
        if(CS_getDCOFrequency()<fc){
            x=1;
        }
        else{
            x=0;
        }

        // El otro del otro filtro

        sprintf(msg,"%0.3f,%0.3f\r\n",VoltajeNormalizado,y);
        // sprintf(msg,"%0.3f,%0.3f\r\n",VoltajeNormalizado,suma);//Descomentar si se quiere usar y comentar los demas
        // sprintf(msg,"%0.3f,%0.3f\r\n",VoltajeNormalizado,x); //Descomentar si se quiere usar y comentar los demas

        //transmision de datos
        for(cont=0;cont<=12;cont++){
            UART_transmitData(EUSCI_A0_BASE, msg[cont]);
        }
    }
}


