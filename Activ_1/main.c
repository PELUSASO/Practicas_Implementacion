/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
   int cont=0x01;
    int i=0x00;
    bool aut=true;
    bool botonbajo=true;
    bool botonbajo2=true;
    bool aux=true;
   int aux2=0x00;
   int aux3=0x00;
int main(void)
{
//Variables locales

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /*Configuracion de GPIOs*/
   GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0);
   GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN1);
   GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN2);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);

   GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN0);
   GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN1);
   GPIO_setOutputLowOnPin(GPIO_PORT_P2 , GPIO_PIN2);

   while(1)
    {


        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_LOW)){
           aux2++;
            if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_LOW)&&(aux2>10)){
                aux3++;
                if(aux3>10){
                    aux=false;
                }
            }
        }

if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)){
    botonbajo=true;
    aux2=0;
}
if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)){
    botonbajo2=true;
    aux3=0;
}

        if (aux==false){
            if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==GPIO_INPUT_PIN_HIGH)){

                if((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_HIGH)){
                aut=!aut;
                aux=true;
               }
            }
        }


if (aut==false){
    if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==GPIO_INPUT_PIN_LOW))
       {
           for(i=0;i<50000;i++){
                }
           if ((cont==1)&&(botonbajo==true)){
GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
botonbajo=false;
cont++;
           }
           else if ((cont==2)&&(botonbajo==true)) {
               GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
               GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
               GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
               botonbajo=false;
               cont++;
           }
           else if ((cont==3)&&(botonbajo==true)) {
                           GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
                           GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                           GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                           botonbajo=false;
                           cont++;
                       }
           else if ((cont==4)&&(botonbajo==true)) {
                           GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
                           GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
                           GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                           botonbajo=false;
                           cont++;
                       }
           else if ((cont==5)&&(botonbajo==true)) {
                           GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                           GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
                           GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                           botonbajo=false;
                           cont++;
                              }
           else if ((cont==6)&&(botonbajo==true)) {
                                   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                                   GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                                   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                                   botonbajo=false;
                                   cont++;
                                        }
           else if ((cont==7)&&(botonbajo==true)) {
                                    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                                    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                                    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                                    botonbajo=false;
                                    cont=1;
                                                    }

           else{

           }
       }
}
else if(aut==true){
    for(i=0;i<50000;i++){
                    }
               if (cont==1){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);

    cont++;
               }
               else if (cont==2) {
                   GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
                   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
                   GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                   cont++;
               }
               else if (cont==3) {
                               GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
                               GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                               GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                               cont++;
                           }
               else if (cont==4) {
                               GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0);
                               GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
                               GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                               cont++;
                           }
               else if (cont==5) {
                                      GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                                      GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN1);
                                      GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                                      cont++;
                                  }
               else if (cont==6) {
                                       GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                                       GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                                       GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN2);
                                                cont++;
                                            }
               else if (cont==7) {
                                        GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN0);
                                        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN1);
                                        GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN2);
                                                            cont=1;
                                                        }

               else{

               }
}
else {
}

}

}
