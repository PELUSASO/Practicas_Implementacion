
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

int cont=0x00;
int cont2=0x00;

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* GPIOs configuración */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);

    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);
    Interrupt_enableMaster();

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    while(1){
    }
}
void PORT1_IRQHandler(void){
    uint32_t status;
    uint32_t final;
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    final= GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    if(status & GPIO_PIN1){
        cont++;
        if (cont == 9 ){
            cont2++;
            P1->OUT= cont;
            cont=0;
            if (cont2 == 9){
                cont2=0;
                P1->OUT= cont2;
                if (final & status){
                    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
                }
            }
    }
}
