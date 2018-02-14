#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	static AutoController::Instruction DEFAULT_STEPS[] =
	{
		{AutoController::WAIT_TIME, 3, 0}
	};
	static AutoController::InstructionSet DEFAULT = {DEFAULT_STEPS, 1};


	static AutoController::Instruction OTHER_STEPS[] =
	{
		{AutoController::WAIT_UNTIL, 5, 0},
		{AutoController::WAIT_TIME, 3, 0}
	};
	static AutoController::InstructionSet OTHER = {OTHER_STEPS, 2};
}

#endif
