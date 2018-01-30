#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(int port) : sensor(port)
{

}

bool LimitSwitch::Get()
{
	return sensor.Get();
}
