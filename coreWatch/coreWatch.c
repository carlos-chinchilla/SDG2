#include "coreWatch.h"

//------------------------------------------------------
// FUNCIONES PROPIAS
//------------------------------------------------------
// Wait until next_activation (absolute time)
// Necesita de la función "delay" de WiringPi.
void DelayUntil(unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay(next - now);
	}
}


//------------------------------------------------------
// MAIN
//------------------------------------------------------
int main() {
	unsigned int next;

	next = millis();
	while (1) {
		next += CLK_MS;
		DelayUntil(next);
	}
}
