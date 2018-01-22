#include "UltrasonicAnalog.h"

UltrasonicAnalog::UltrasonicAnalog(int port, double inchesPerVolt) :
		analogInput(port)
{
	analogInput.SetAverageBits(2); // Uses 2^2 samples for average

	this->inchesPerVolt = inchesPerVolt;
}

double UltrasonicAnalog::GetDistanceInches()
{
	double voltage = analogInput.GetAverageVoltage();
	double distance = voltage * inchesPerVolt;

	return distance;
}
