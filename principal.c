/*
 * File:   principal.c
 * Author: HP
 *
 * Created on 5 de abril de 2020, 06:12 PM
 */


#include <xc.h>
#include "Config.h"
#include "I2C.h"
#include "Usart.h"
#include "RTC_I2C.h"

#define _XTAL_FREQ 20000000UL

/*
 * Principal
 */

void main(void) {
    Confi_Blue(32); /*Confuguramos los registros correspondientes a la salida de datos por la terminar serial con un valor de 32 para el registro SPBRG relativo al cristal de 20Mhz*/
    I2C_Inicializar(); /*Confuguramos los registros correspondientes a la salida de datos por protocolo I2C*/

    /*************************************************************************************/
    /********************ENVIAMOS LA HORA HACIA EL RTC********************************/
    /*
     * PARA COLOCAR LA HORA Y LA FECHA SE TIENE QUE IR A LA FUNCIÓN RTC_I2C 
     * Y CAMBIAR LOS SIGUIENTES VALORES DE LOS ARREGLOS 
     * char Hora_Del_Usuario[] = "022612"; 02 HORAS 26 MINUTOS 12 SEGUNDOS
     *char Fecha_Del_Usuario[] = "120912"; 12 DIAS 09 MES 12 AÑO
     */

    Transformar_Hora_Para_Envio(); /*Transformamos las horas en sus equivalentes den BCD para ser enviados al RTC*/
    Escribir_Hora_Rtc(); /*Se envia la hora ateriormente transformada*/

    Transformar_Fecha_Para_Envio(); /*Transformamos las horas en sus equivalentes den BCD para ser enviados al RTC*/
    Poner_Fecha_Rtc(); /*Se envia la hora ateriormente transformada*/

    /**************************************************************************************/

    while (1) { /*Bucle para la muestra de hora y fecha procedente del RTC*/
        UART0_Tx_String("Hora: "); /*Añadimos un mensaje de "HORA" que antesede a la hora a mostrar por el terminal serial*/
        Leer_Hora_Rtc(); /*Leemos la hora puesta en las variables correspondientes*/
        Hora_Por_Terminal_Serial(); /*Enviamos la hora a través del terminal serial*/

        UART0_Tx_String("Fecha: "); /*Añadimos un mensaje de "FECHA" que antesede a la fecha a mostrar por el terminal serial*/
        Leer_Fecha_Rtc(); /*Leemos la hora puesta en las variables correspondientes*/
        Fecha_Por_Terminal_Serial(); /*Enviamos la fecha a través del terminal serial*/
        __delay_ms(1000); /*Damos un pequeño retardo por software*/
    }
    return;
}