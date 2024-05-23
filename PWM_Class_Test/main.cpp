

/**
 * Testprogramm der PWM Klasse:
 * Dieses Programm soll die Funkionalit�t der PWM Klasse pr�fen, sodass diese im Hauptprogramm benutzt werden kann.
 *
 */
#include "PWM.h"
#include "System.h"
#include "Config.h"
#include <cmath>

void main(void)
{
	System sys;
	PWM motorRight, motorLeft;
	GPIO dirLeft, dirRight, brakeLeft, brakeRight, enable;

	// Initialisierung des Systemobjektes
	sys.init(40000000);

	//TODO: Put your code here -> A3.2

	while(1)
	{
	    //TODO: Put your code here -> A3.2
	}
}
