#ifndef BULLDOZER_H
#define BULLDOZER_H

#include <WPILib.h>
#include <iostream>

class Bulldozer
{
public:
	Bulldozer(int retractPort, int extendPort, int kickInPort, int kickOutPort);

	void Extend();
	void Retract();

	bool GetIsExtended();

	bool Pulse(double pauseTime); // Returns true if complete
	bool Kick(double pauseTime); // Returns true if complete

private:
	enum State { UNKNOWN, RETRACT, EXTEND };

	DoubleSolenoid bulldozerSolenoid;
	DoubleSolenoid kickerSolenoid;

	Timer pulseTimer;
	Timer kickTimer;

	bool isPulsing;
	bool isKicking;
};

#endif
