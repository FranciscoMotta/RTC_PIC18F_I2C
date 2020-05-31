/* 
 * File:   RTC_I2C.h
 * Author: HP
 *
 * Created on 6 de abril de 2020, 01:53 AM
 */

#ifndef RTC_I2C_H
#define	RTC_I2C_H

/*
 * Macros
 */

#include <xc.h>
#include <stdint.h>
#define DATO_CONVERSOR_A_ASCII 0x30

/*
 * Variables
 */

uint8_t segundos, minutos, horas;
uint8_t dias, meses, years;
uint8_t variable_de_conteo = 0;

char Hora_Del_Usuario[] = "022612";
char Fecha_Del_Usuario[] = "120912";

/*
 * Funciones
 */

void Escribir_Hora_Rtc(void);

void Leer_Hora_Rtc(void);

void Poner_Fecha_Rtc(void);

void Leer_Fecha_Rtc(void);

void Hora_Por_Terminal_Serial(void);

void Fecha_Por_Terminal_Serial(void);

void Transformar_Hora_Para_Envio(void);

void Transformar_Fecha_Para_Envio(void);

#endif	/* RTC_I2C_H */

