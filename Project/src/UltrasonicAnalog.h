#ifndef ULTRASONIC_ANALOG_H_
#define ULTRASONIC_ANALOG_H_

#include <WPILib.h>

class UltrasonicAnalog
{
private:
	AnalogInput analogInput;

	double inchesPerVolt;

public:
	UltrasonicAnalog(int port, double inchesPerVolt);

	double GetDistanceInches();

};

#endif
