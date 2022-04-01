

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
/* Contastes que se utilizaron */
int cont =0x00;
int btn =0x00;
int btn2 =0x00;
int aux1= 0x00;
bool est = true;


int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();
    /* Configuración de los GPIOs */

    /* Semaforo 1 */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);//verde
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);//amarrillo
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);//rojo
    /* Semaforo 2*/
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);//verde
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);//amarrillo
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);//rojo
    /* Semaforo 3*/
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0);//verde
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN1);//amarrillo
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN2);//rojo
    /* Semaforo 4*/
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN5);//verde
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN6);//amarrillo
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);//rojo

    //Botones para secuencia/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);//boton 1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);//boton 2


    //Configuración para el Timer A0 que sirive como reloj de los semaforos/
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_OUTMOD_4 + TIMER_A_CCTLN_CCIE;
    TIMER_A0 ->CCR[0]=32768;//Para obtener una frecuencia de 0.5 Hz Y un tiempo de 2 seg/
    TIMER_A0 ->CTL = TIMER_A_CTL_SSEL__ACLK + TIMER_A_CTL_MC__CONTINUOUS + TIMER_A_CTL_CLR + TIMER_A_CTL_IE;

    __enable_irq();// se activa la interrupción global
    NVIC -> ISER[0] |= 1 << ((TA0_0_IRQn) & 31);//SI SE COMPARA

    while(1){
    }
}
void TA0_0_IRQHandler(){
    TIMER_A0 -> CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; //Limpieza a la bandera de interrupcion
    TIMER_A0 -> CCR[0] +=32768; //Sumamos el offset para CCR0
    btn++;
    cont++;
    btn2++;
    if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW) && (btn>=2)){ //se verifica el estado del botón 1.1 y que sea presionado 2 segundos
        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)== GPIO_INPUT_PIN_LOW) && (btn2>=2)){ //se verifica el estado del botón 1.4 y que sea presionado 2 segundos
                    aux1++;//aumenta el auxiliar par verificar la vez que fue presionado los botones
                    if(aux1==1){
                        est=false; //se activa el modo precaución
                        btn=0; //se reinicializa el conteo
                        btn2=0; //se riinicializa el conteo
                    }
                    else if (aux1==2){ // se presiono una segunda vez
                        est=true; //se activa el modo normal
                        aux1=0; //se reinicializa el conteo
                        btn=0; //se reinicializa el conteo
                        btn2=0; //se reinicializa el conteo
                        cont=0; //se reinicializa el conteo
                    }
        }
    }
    if (est==true){ //se verifica el estado del semaforo, se activa el estado normal
        if(cont==1){
             /* Semaforo 1 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
         }
        else if(cont==3){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==4){
            /* Semaforo 1 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==5){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==6){
            /* Semaforo 1 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==7){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6); //Se activa el Amarrillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==10){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==12){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==13){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==14){
           /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if(cont==15){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==16){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);//Se activa el amarillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==19){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==22){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==23){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==24){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==25){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==26){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN1);//se activa amarrillo
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==29){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==32){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==33){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==34){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==35){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==36){
            /* Semaforo 1 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7);
            /* Semaforo 2 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
            GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6);
            GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
            /* Semaforo 3 */
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN1);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN2);
            /* Semaforo 4*/
            GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
            GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
            GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
        }
        else if (cont==39){
            cont=0;
        }

    }
    else if (est==false){ //se activa el estado precaución
        P2->OUT^=BIT6;
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
        P3->OUT^=BIT6;
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
        P4->OUT^=BIT1;
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);
        P4->OUT^=BIT6;
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN5);
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    }
}
