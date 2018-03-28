#ifndef CLAMP_H
#define CLAMP_H

#include <WPILib.h>

class Clamp
{
public:
	Clamp(int armUpPort, int armDownPort, int closePort, int openPort);

	void ArmUp();
	void ArmDown();
	void Close();
	void Open();

	bool GetIsDown();
	bool GetIsOpen();

private:
	DoubleSolenoid armSolenoid;
	DoubleSolenoid clampSolenoid;

};

#endif