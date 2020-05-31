#include <xc.h>
#include "RTC_I2C.h"
#include "I2C.h"
#include "Usart.h"

void Escribir_Hora_Rtc(void) {
    I2C_Inicio();
    I2C_Escribir(0b11010000); /*Elegimos la dirección del RTC (slave) que viene dato por el fabricante*/
    /*
     * Nota: El ultimo bit es referente a la modalidad de escritura y de lectura en 0 se escribe
     */
    I2C_Escribir(0b00000000); /*Enviamos la primera dirección para configurar el RTC que es la de los segundos*/
    /*
     * El primer bit corresponde a CH  = 0
     * Segun el fabricante:
     * The CH bit in the seconds register will be set to a 1. The clock can be halted
     * whenever the timekeeping functions are not required, which minimizes current (IBATDR).
     */
    I2C_Escribir(segundos); /*Escribimos el valor de los segundos almacenado en la variable correspondiente*/
    I2C_Escribir(minutos); /*Escribimos el valor de los minutos almacenado en la variable correspondiente*/
    I2C_Escribir(horas); /*Escribimos el valor de los minutos almacenado en la variable correspondiente*/

    I2C_Detener(); /*Damos el comando de parada para detener la transferencia de datos por I2C*/
}

void Leer_Hora_Rtc(void) {
    I2C_Inicio(); /*Damos el comando de START para iniciar la transferencia de datos por I2C*/
    I2C_Escribir(0b11010000); /*Escojemos la dirección del RTC slave*/
    I2C_Escribir(0b00000000); /*Enviamos el dato para ubicarnos en la dirección desde la cual se va a comenzar a leer los datos*/
    /*
     * Como deseamos leer desde los segundos vamos a la dirección 0x00
     */
    I2C_Reinicio(); /*Enviamos el comando de RESTART para preparar el siguiente envio o recepción de datos*/
    I2C_Escribir(0b11010001); /*Se elige la dirección del esclavo y se manda el ultimo bit en 1 para poder indicar que queremos leer los datos del RTC*/
    segundos = I2C_Leer(); /*Leemos primer dato que retorna el RTC que es referente a los segundos*/
    I2C_ack(); /*Enviamos el comando de reconocimiento para poder dar pase a la lectura del siguiente dato del RTC*/
    minutos = I2C_Leer(); /*Leemos el segundo dato del RTC relativo a los minutos*/
    I2C_ack(); /*Enviamos el comando de reconocimiento para poder dar pase a la lectura del siguiente dato del RTC*/
    horas = I2C_Leer(); /*Leemos el tercer dato que corresponde a las horas que da el RTC*/
    I2C_nack(); /*Enviamos el comando de no reconocimiento de datos para detener la lectura del RTC*/
    I2C_Detener(); /*Damos por terminada la recepción de datos del RTC*/
}

void Poner_Fecha_Rtc(void) {
    I2C_Inicio(); /*Enviamos el comando de START del RTC para iniciar la transmisión de los datos*/
    I2C_Escribir(0b11010000); /*Enviamos la dirección del RTC para iniciar la comunicación*/
    I2C_Escribir(0b00000100); /*Enviamos la direccion que corresponde al dato de almacenado de días*/
    I2C_Escribir(dias); /*Enviamos el dato de los días guardado en la variable del mismo nombre*/
    I2C_Escribir(meses); /*Enviamos el dato de los meses guardado en la variable del mismo nombre*/
    I2C_Escribir(years); /*Enviamos el dato de los años guardado en la variable "years */
    I2C_Detener(); /*Detenemos la transmisión de datos por el bus I2C*/
}

void Leer_Fecha_Rtc(void) {
    I2C_Inicio(); /*Damos el comando de START para iniciar la transferencia de datos por I2C*/
    I2C_Escribir(0b11010000); /*Escojemos la dirección del RTC slave*/
    I2C_Escribir(0b00000100); /*Enviamos el dato para ubicarnos en la dirección desde la cual se va a comenzar a leer los datos*/
    /*
     * Como deseamos leer desde los dias vamos a la dirección 0x04
     */
    I2C_Reinicio(); /*Enviamos el comando de RESTART para preparar el siguiente envio o recepción de datos*/
    I2C_Escribir(0b11010001); /*Se elige la dirección del esclavo y se manda el ultimo bit en 1 para poder indicar que queremos leer los datos del RTC*/
    dias = I2C_Leer(); /*Leemos primer dato que retorna el RTC que es referente a los segundos*/
    I2C_ack(); /*Enviamos el comando de reconocimiento para poder dar pase a la lectura del siguiente dato del RTC*/
    meses = I2C_Leer(); /*Leemos el segundo dato del RTC relativo a los minutos*/
    I2C_ack(); /*Enviamos el comando de reconocimiento para poder dar pase a la lectura del siguiente dato del RTC*/
    years = I2C_Leer(); /*Leemos el tercer dato que corresponde a las horas que da el RTC*/
    I2C_nack(); /*Enviamos el comando de no reconocimiento de datos para detener la lectura del RTC*/
    I2C_Detener(); /*Damos por terminada la recepción de datos del RTC*/
}

void Hora_Por_Terminal_Serial(void) {
    Tx_str((horas >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial la Hora y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de las horas del RTC
     */
    Tx_str((horas & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    Tx_str(':'); /*Visualizamos un separador por el terminal serial*/

    Tx_str((minutos >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial los minutos y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de los minutos del RTC
     */
    Tx_str((minutos & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    Tx_str(':'); /*Visualizamos un separador por el terminal serial*/

    Tx_str((segundos >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial los segundos y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de los segundos del RTC
     */
    Tx_str((segundos & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    UART0_Tx_String(" \r\n"); /*Mostramos un fin de linea y saltamos un espacio*/
}

void Fecha_Por_Terminal_Serial(void) {
    Tx_str((years >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial la Hora y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de las horas del RTC
     */
    Tx_str((years & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    Tx_str('-'); /*Visualizamos un separador por el terminal serial*/

    Tx_str((meses >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial los minutos y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de los minutos del RTC
     */
    Tx_str((meses & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    Tx_str('-'); /*Visualizamos un separador por el terminal serial*/

    Tx_str((dias >> 4) + DATO_CONVERSOR_A_ASCII); /*Mostramos por el ternimal serial los segundos y añadimos el conversor a ASCII*/
    /*
     * El corrimiento se hace para mostrar primero los bits mas significativos de los segundos del RTC
     */
    Tx_str((dias & 0x0F) + DATO_CONVERSOR_A_ASCII);
    /*
     * El multiplicador se da con el fin de poner en 0000 los bits mas significativos del RTC y mostrar los menos significativos
     */
    UART0_Tx_String(" \r\n"); /*Mostramos un fin de linea y saltamos un espacio*/
}

void Transformar_Hora_Para_Envio(void) {
    for (variable_de_conteo = 0; variable_de_conteo < 7; variable_de_conteo++) { /*Creamos un conteo para garantizar que los bits más significativos sean 0*/
        Hora_Del_Usuario[variable_de_conteo]  &= 0x0F; /*Multiplicamos por 0x0F para garantizar lo anterior*/
    }

    horas = ((Hora_Del_Usuario[0] << 4) + Hora_Del_Usuario[1]); /*Guardamos la hora en la vairiable determinada para ser enviada al RTC*/
    minutos = ((Hora_Del_Usuario[2] << 4) + Hora_Del_Usuario[3]); /*Guardamos los minutos en la vairiable determinada para ser enviada al RTC*/
    segundos = ((Hora_Del_Usuario[4] << 4) + Hora_Del_Usuario[5]); /*Guardamos los segundos en la vairiable determinada para ser enviada al RTC*/

}

void Transformar_Fecha_Para_Envio(void) {
    for (variable_de_conteo = 0; variable_de_conteo < 7; variable_de_conteo++) {/*Creamos un conteo para garantizar que los bits más significativos sean 0*/
        Fecha_Del_Usuario[variable_de_conteo] &= 0x0F; /*Multiplicamos por 0x0F para garantizar lo anterior*/
    }

    years = ((Fecha_Del_Usuario[0] << 4) + Fecha_Del_Usuario[1]); /*Guardamos los años en la vairiable determinada para ser enviada al RTC*/
    meses = ((Fecha_Del_Usuario[2] << 4) + Fecha_Del_Usuario[3]); /*Guardamos los meses en la vairiable determinada para ser enviada al RTC*/
    dias = ((Fecha_Del_Usuario[4] << 4) + Fecha_Del_Usuario[5]); /*Guardamos los dias en la vairiable determinada para ser enviada al RTC*/

}