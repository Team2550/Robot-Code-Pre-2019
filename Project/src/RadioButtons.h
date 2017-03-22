#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H

#include <string>
#include <map>
#include <WPILib.h>

template<class E>
class RadioButtons
{
public:
	RadioButtons(std::string name);
	~RadioButtons();

	void AddObject(std::string name, E enumValue);
	void AddDefault(std::string name, E enumValue);
	E GetSelected();
private:
	std::map<std::string, E*> enumPointers;
	SendableChooser<E*> sendableChooser;
};

#endif
