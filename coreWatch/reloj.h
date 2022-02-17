/*
 * reloj.h
 *
 *  Created on: 10 de feb. de 2022
 *      Author: alumno
 */


#ifndef RELOJ_H_
#define RELOJ_H_

#include <systemConfig.h>
#include <util.h>
#include <coreWatch.h>

enum FSM_ESTADOS_RELOJ {
	WAIT_TIC=0
};
#define FLAG_ACTUALIZA_RELOJ 0x01
#define FLAG_TIME_ACTUALIZADO 0x02

typedef struct {
int dd;
int MM;
int yyyy ;
} TipoCalendario ;

typedef struct {
int hh;
int mm;
int ss;
int formato ;
} TipoHora ;

typedef struct {
int timestamp ;
TipoHora hora ;
TipoCalendario calendario ;
tmr_t * tmrTic ;
} TipoReloj ;

typedef struct {
int flags ;
} TipoRelojShared ;

#define MIN_DAY 1
#define MAX_MONTH 12
#define MIN_MONTH 1
#define MIN_YEAR 1970
#define TIME_FORMAT_24_H 24


#define MIN_HOUR 0
#define MAX_HOUR 24

#define MAX_MIN 59
#define MAX_SEG 59

#define PRECISION_RELOJ_MS 1000

extern fsm_trans_t g_fsmTransReloj [];
extern const int DIAS_MESES[2][MAX_MONTH];

int CompruebaTic ( fsm_t * p_this );
void ActualizaReloj ( fsm_t * p_this );
void tmr_actualiza_reloj_isr ( union sigval value );
void ActualizaFecha ( TipoCalendario * p_fecha );
void ActualizaHora ( TipoHora * p_hora );
int CalculaDiasMes (int month , int year );
int ConfiguraInicializaReloj ( TipoReloj * p_reloj );
int EsBisiesto (int year );
TipoRelojShared GetRelojSharedVar ();
void ResetReloj ( TipoReloj * p_reloj );
int SetHora (int horaInt , TipoHora * p_hora);
void SetRelojSharedVar ( TipoRelojShared value );


#endif /* RELOJ_H_ */
