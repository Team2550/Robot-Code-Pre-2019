#include "RadioButtons.h"
#include <iostream>

template<class E>
RadioButtons<E>::RadioButtons(std::string name)
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
void RadioButtons<E>::AddObject(std::string, E enumValue)
{

}

template<class E>
void RadioButtons<E>::AddDefault(std::string, E enumValue)
{

}

template<class E>
E RadioButtons<E>::GetSelected()
{

}
