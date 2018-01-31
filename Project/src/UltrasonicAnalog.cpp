#include "UltrasonicAnalog.h"

UltrasonicAnalog::UltrasonicAnalog(int port, double inchesPerVolt, double minDistance) :
		analogInput(port)
{
	analogInput.SetAverageBits(2); // Uses 2^2 samples for average

	this->inchesPerVolt = inchesPerVolt;
	this->minDistance = minDistance;
}

double UltrasonicAnalog::GetDistanceInches()
{
	double voltage = analogInput.GetAverageVoltage();
	double distance = voltage * inchesPerVolt;

	for (int i = NUM_VALUES_AVERAGE - 1; i > 1; i--)
	{
		distanceValues[i] = distanceValues[i - 1];
	}

	if (distance >= minDistance)
		distanceValues[0] = distance;
	else
		distanceValues[0] = 0;

	double sumDistance = 0;
	int numValidValues = 0;

	for (int i = 0; i < NUM_VALUES_AVERAGE; i++)
	{
		if (distanceValues[i] >= minDistance)
		{
			sumDistance += distanceValues[i];
			numValidValues++;
		}
	}

	if (numValidValues > 0)
		return sumDistance / numValidValues;
	else
		return 0;
}
