#ifndef RADIOBUTTONS_H
#define RADIOBUTTONS_H

#include <string>
#include <map>

template<class E>
class RadioButtons
{
public:
	RadioButtons(std::string name);
	~RadioButtons();

	void AddObject(std::string, E enumValue);
	void AddDefault(std::string, E enumValue);
	E GetSelected();
private:
	std::map<std::string, E*> enumPointers;
};

#endif
