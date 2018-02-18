#ifndef BULLDOZER_H
#define BULLDOZER_H

#include <WPILib.h>

class Bulldozer
{
public:
	Bulldozer(int extendPort, int retractPort, double extensionTime);

	void Reset();
	void Extend();
	void Retract();
	void Stop();
	bool Pulse(double pauseTime); // Returns true if complete

private:
	enum State { UNKNOWN, RETRACT, EXTEND };

	DoubleSolenoid solenoid;
	Timer delay;
	bool pulsing;
	State lastState;

	double extensionTime;

};

#endif