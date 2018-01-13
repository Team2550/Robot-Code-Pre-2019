#include "UltrasonicAnalog.h"

UltrasonicAnalog::UltrasonicAnalog(int port, double inchesPerVolt) :
		analogInput(port)
{
	analogInput.SetAverageBits(3);

	this->inchesPerVolt = inchesPerVolt;
}

double UltrasonicAnalog::GetDistanceInches()
{
	double voltage = analogInput.GetAverageVoltage();
	double distance = voltage * inchesPerVolt;

	return distance;
}
