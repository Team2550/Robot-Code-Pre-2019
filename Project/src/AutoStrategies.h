#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	// Default
	const AutoController::Instruction DEFAULT_STEPS[] =
	{
		{AutoController::WAIT_TIME, 3, 0}
	};
	const AutoController::InstructionSet DEFAULT = {DEFAULT_STEPS, 1};

	// Other
	const AutoController::Instruction OTHER_STEPS[] =
	{
		{AutoController::WAIT_UNTIL, 5, 0},
		{AutoController::WAIT_TIME, 3, 0}
	};
	const AutoController::InstructionSet OTHER = {OTHER_STEPS, 2};

	// Nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};
}

#endif
