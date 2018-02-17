#ifndef BULLDOZER_H
#define BULLDOZER_H

#include <WPILib.h>

class Bulldozer
{
public:
	Bulldozer(int extendPort, int retractPort, double extensionTime);

	void Extend();
	void Retract();
	void Stop();
	void Pulse(double pauseTime);

private:
	enum State { RETRACT, EXTEND };

	DoubleSolenoid solenoid;
	Timer delay;
	bool pulsing;
	State lastState;

	double extensionTime;

};

#endif
