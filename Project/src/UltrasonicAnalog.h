#ifndef ULTRASONIC_ANALOG_H_
#define ULTRASONIC_ANALOG_H_

#include <WPILib.h>

const int NUM_VALUES_AVERAGE = 5;

class UltrasonicAnalog
{
private:
	AnalogInput analogInput;
	double distanceValues[NUM_VALUES_AVERAGE] = {};

	double inchesPerVolt;
	double minDistance;

public:
	UltrasonicAnalog(int port, double inchesPerVolt, double minDistance);

	double GetDistanceInches();

};

#endif
