#include "RadioButtons.h"
#include <iostream>

template<class E>
RadioButtons<E>::RadioButtons(std::string name) : sendableChooser(name)
{

}

template<class E>
RadioButtons<E>::~RadioButtons()
{
	for(typename std::map<std::string, E*>::iterator i = enumPointers.begin();
	    i != enumPointers.end(); ++i)
		delete i->second;
}

template<class E>
void RadioButtons<E>::AddObject(std::string name, E enumValue)
{
	try
	{
		enumPointers.at(name);
	}
	catch(const std::out_of_range &e)
	{
		enumPointers.insert(name, new E(enumValue));
		sendableChooser.AddObject(name, enumPointers.at(name));
	}
}

template<class E>
void RadioButtons<E>::AddDefault(std::string name, E enumValue)
{
	try
	{
		enumPointers.at(name);
	}
	catch(const std::out_of_range &e)
	{
		enumPointers.insert(name, new E(enumValue));
		sendableChooser.AddDefault(name, enumPointers.at(name));
	}
}

template<class E>
E RadioButtons<E>::GetSelected()
{
	E *selection = sendableChooser.GetSelected();
	if(selection == nullptr)
	{
		std::cerr << "Radio button not received; undefined behavior starting now..." << std::endl;
		return E(0);
	}
	else
		return *selection;
}
