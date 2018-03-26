#ifndef BULLDOZER_H
#define BULLDOZER_H

#include <WPILib.h>
#include "Utility.h"

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

	DoubleSolenoid bulldozerSolenoid;
	DoubleSolenoid kickerSolenoid;

	Timer bulldozerDelay;
	bool bulldozerPulsing;
	Utility::PneumaticState bulldozerLastState;

	Timer kickerDelay;
	bool kicking;
	Utility::PneumaticState kickerLastState;

	double extensionTime;

};

#endif
