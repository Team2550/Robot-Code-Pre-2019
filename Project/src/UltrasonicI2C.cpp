#include <UltrasonicI2C.h>

UltrasonicI2C::UltrasonicI2C(int sensorAddress) :
		sensorI2C(I2C::kOnboard, sensorAddress)
{

}

bool UltrasonicI2C::GetDistance(double& distance)
{
	unsigned char buffer[READ_BYTE_COUNT];
	bool success = sensorI2C.ReadOnly(READ_BYTE_COUNT, buffer);

	if (success)
	{
		distance = buffer[0] * 256 + buffer[1];
	}

	return success;
}
