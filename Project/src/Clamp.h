#ifndef CLAMP_H
#define CLAMP_H

#include <WPILib.h>

class Clamp
{
public:
	Clamp(int armDownPort, int armUpPort, int openPort, int closePort);

	void ArmUp();
	void ArmDown();
	void Open();
	void Close();

	bool GetIsDown();
	bool GetIsOpen();

private:
	DoubleSolenoid armSolenoid;
	DoubleSolenoid clampSolenoid;

};

#endif
