#ifndef BULLDOZER_H
#define BULLDOZER_H

#include <WPILib.h>

class Bulldozer
{
public:
	Bulldozer(int retractPort, int extendPort, int kickInPort, int kickOutPort, double extensionTime);

	void Reset();
	void Extend();
	void Retract();
	void Stop();
	bool Pulse(double pauseTime); // Returns true if complete
	bool Kick(double pauseTime); // Returns true if complete

private:
	enum State { UNKNOWN, RETRACT, EXTEND };

	DoubleSolenoid bulldozerSolenoid;
	DoubleSolenoid kickerSolenoid;

	Timer bulldozerDelay;
	bool bulldozerPulsing;
	State bulldozerLastState;

	Timer kickerDelay;
	bool kicking;
	State kickerLastState;

	double extensionTime;

};

#endif
