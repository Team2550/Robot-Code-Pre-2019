#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(int port, SignalState stateWhenPushed) : sensor(port)
{
	this->stateWhenPushed = stateWhenPushed;
}

bool LimitSwitch::GetPushed()
{
	if (stateWhenPushed == HIGH)
		return sensor.Get();
	else
		return !sensor.Get();
}
