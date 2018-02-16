#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	// Instruction Format: Type, Target, Speed

	// Place block in exchange
	const AutoController::Instruction EXCHANGE_STEPS[] =
	{
		{AutoController::DRIVE_TO, 36, 0.2f},
		{AutoController::ROTATE_TO, -90, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144.132, 0.2f},
		{AutoController::ROTATE_TO, -180, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30, 0.2f}
	};
	const AutoController::InstructionSet EXCHANGE = {EXCHANGE_STEPS, 7};

	// Place block in exchange
	const AutoController::Instruction EXCHANGE_STEPS[] =
	{
		{AutoController::DRIVE_TO, 1, .3f},
		{AutoController::ROTATE_TO, -90, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 12.011f, 0.2f},
		{AutoController::ROTATE_TO, -180, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 2.5f, 0.2f}
	};
	const AutoController::InstructionSet EXCHANGE = {EXCHANGE_STEPS, 7};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};
}

#endif
