
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Declaración de variables auxilares */
int cont=0x01;
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



    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    while(1)
    {
        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1))==GPIO_INPUT_PIN_HIGH){
            botonbajo=true;
        }
        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_LOW){
            for (i=0;i<50000;i++){
            }

        if ((cont==1)&&(botonbajo==true)){ // Estado A
           P2->OUT= cont;
           botonbajo=false;
           cont++;
        }
        else if ((cont==2)&&(botonbajo==true)){ // Estado B
            P2->OUT= cont;
             botonbajo=false;
             cont++;
        }
        else if ((cont==3)&&(botonbajo==true)){ //Estado C
            P2->OUT= cont;
             botonbajo=false;
             cont++;
        }
        else if ((cont==4)&&(botonbajo==true)){ // Estado D
            P2->OUT= cont;
             botonbajo=false;
             cont=1;
       }
       else{

       }
    }
  }
}
