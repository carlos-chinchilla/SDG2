/*
 * reloj.c
 *
 *  Created on:
 *      Author:
 */


#include "reloj.h"

fsm_trans_t g_fsmTransReloj[] = {{WAIT_TIC, CompruebaTic, WAIT_TIC, ActualizaReloj},
									{-1, NULL, -1, NULL},};

static TipoRelojShared g_relojSharedVars;

int DIAS_MESES[0]={31,29,31,30,31,30,31,31,30,31,30,31};
int DIAS_MESES[1]={31,28,31,30,31,30,31,31,30,31,30,31};

void ResetReloj(TipoReloj *p_reloj){
	p_reloj ->calendario.MM=DEFAULT_DAY;
	p_reloj ->calendario.dd=DEFAULT_MONTH;
	p_reloj ->calendario.yyyy=DEFAULT_YEAR;
	p_reloj ->hora.hh=DEFAULT_HOUR;
	p_reloj ->hora.mm=DEFAULT_MIN;
	p_reloj ->hora.ss=DEFAULT_SEC;
	p_reloj ->timestamp = 0;
	p_reloj ->hora.formato = DEFAULT_TIME_FORMAT;

	piLock(RELOJ_KEY);
	g_relojSharedVars.flags = 0;
	piUnlock(RELOJ_KEY);
}
int ConfiguraInicializaReloj (TipoReloj *p_reloj) {
	ResetReloj(p_reloj);
	tmr_t* tmr1 = tmr_new (tmr_actualiza_reloj_isr);
	p_reloj -> tmrTic = tmr1;
	tmr_startms_periodic(tmr1, PRECISION_RELOJ_MS);
	return 0;
}

//Comprueba el bit FLAG_ACTUALIZA_RELOJ de la variable flags
int CompruebaTic (fsm_t* p_this){
	piLock(RELOJ_KEY);
	resultado = (g_relojSharedVars.flags & FLAG_ACTUALIZA_RELOJ);
	piUnlock(RELOJ_KEY);

	return resultado;
}

void ActualizaReloj (fsm_t* p_this){
	//Convertimos el argumento a un TipoReloj y lo guardamos en un puntero
	TipoReloj *p_mireloj = (TipoReloj*)(p_this->user_data);

	//Aumentamos el timestap del reloj recibido en 1
	p_mireloj ->timestamp += 1;

	//Actualizamos la hora del reloj recibido
	ActualizaHora(p_mireloj ->hora);

	//Si tras actualizar la hora estan los valores a 0, actualizamos la fecha
	if((p_mireloj ->hora.hh == 0) && (p_mireloj ->hora.mm == 0) && (p_mireloj ->hora.ss == 0)){
		ActualizaFecha(p_mireloj ->calendario);
	}

	//Poner a 0 el bit FLAG_ACTUALIZA_RELOJ de la variable flags y a 1 el FLAG_TIME_ACTUALIZADO
	piLock(RELOJ_KEY);
	g_relojSharedVars.flags &= FLAG_ACTUALIZA_RELOJ;
	g_relojSharedVars.flags |= FLAG_TIME_ACTUALIZADO;
	piUnlock(RELOJ_KEY);

}

//Poner a 1 el bit FLAG_ACTUALIZA_RELOJ de la variable flags
void tmr_actualiza_reloj_isr (union sigval value){
	piLock(RELOJ_KEY);
	g_relojSharedVars.flags |= FLAG_ACTUALIZA_RELOJ;
	piUnlock(RELOJ_KEY);
}

void ActualizaFecha (TipoCalendario *p_fecha){
	int diasMes = CalculaDiasMes(p_fecha ->MM , p_fecha ->yyyy);
	int diaActual = p_fecha ->dd;
	int modulo = (diaActual+1)%(diasMes+1);
	int nuevoDia = MAX(1,modulo);
	p_fecha ->dd = nuevoDia;

	if (nuevoDia==1){
		int mesActual = p_fecha ->MM;
		int modulo1 = (mesActual+1)%(MAX_MONTH+1);
		int nuevoMes = MAX(1,modulo1);
		p_fecha ->dd = nuevomes;

		if (nuevoMes==1){
			int añoActual = p_fecha ->yyyy;
			int nuevoAño = añoActual + 1;
			p_fecha ->yyyy = nuevoAño;
		}
	}


}

void ActualizaHora (TipoHora *p_hora){
	int seg = p_hora ->ss;
	int seg2 = seg +1;
	int modulo = seg2 % 60;
	p_hora ->ss = modulo;

	if (modulo==0){
		int min = p_hora ->mm;
		int min2 = min + 1;
		int modulo1 = min2 % 60;
		p_hora ->mm = modulo1;

		if (modulo1==0){
			int horaActual = p_hora ->hh;
			int horaNueva = horaActual +1;
			int modulo1 = horaNueva % (DEFAULT_TIME_FORMAT+1);
			p_hora ->hh = modulo1;
		}

	}
}

int CalculaDiasMes(int month, int year){
	int bisiesto = EsBisiesto(year);
	if (month==2 && bisiesto==1){
		return 29;
	} else if (month==2 && bisiesto==0) {
		return  28;
	} else if ((month==1|3|5|7|8|10|12)){
		return 31;
	} else {
		return 30;
	}

}

int EsBisiesto(int year){
	if ((year%4)==0){
		if((year%100)==0){
			if((year%400)==0){
				return 1;
			} else return 0;
		}else return 1;
	}else return 0;

}

int SetHora(int horaInt, TipoHora *p_hora){
	if (horaInt<0) return 1;


}

TipoRelojShared GetRelojSharedVar(){

}

void SetRelojSharedVar(TipoRelojShared value){
}

