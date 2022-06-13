
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

      int i=0x00;
      int cont=0x00;
      int aux=0x00;

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();


// PIN DE SALIDA LED DE PLACA
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);


    while(1)
    {

        for (i=0; i<1000; i++){ //contador de 1000
         for(cont=0; cont<300000; cont++); //Delay de aprox 1 seg
         if((i==2) | (i==3)| (i==5)| (i==7)){ // Ver si es divisible entre 2,3,5 o 7 para observar si es primo,
          GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
         }
         else if((i%2==0)|(i%3==0)|(i%5==0)|(i%7==0)){ //En caso de que sea divisible y no es el mismo numero se apaga el led por que no es primo
             GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
         }
         else
         {
             GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); //De otro modo se prende
         }

        }
        
    }
}
