
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Declaración de variables auxilares */
int cont=0x01;
int cont2=0x05;
int i=0x01;
bool botonbajo=true;

int main(void)
{
    /* Configuiración de GPIOs */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);


    /* Inicializando GPIOs */
    GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN2);

    /* Configuración de interrupciones P1.4 */
     GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
     GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

     Interrupt_enableInterrupt(INT_PORT1);
     Interrupt_enableMaster();


    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    while(1)
    {
        
    }
}
void PORT1_IRQHandler(void){
    uint32_t status;
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    if(status & GPIO_PIN1){
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
        if (cont2==5){ // Estado A
           P2->OUT= cont2;
           cont2++;
        }
        else if (cont2==6){ // Estado B
            P2->OUT= cont2;
             cont2++;
        }
        else if (cont2==7){ //Estado C
            P2->OUT= cont2;
             cont2=1;
        }
        else if (cont2==1){ // Estado D
            P2->OUT= cont2;
             cont2=5;
       }
       else{

       }
    }
}
