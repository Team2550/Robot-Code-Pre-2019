#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(int port) : sensor(port)
{

}

bool LimitSwitch::GetPressed()
{
	return sensor.Get();
}
