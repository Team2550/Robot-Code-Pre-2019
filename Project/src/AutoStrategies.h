#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

const float AUTO_DRIVE_SPEED = 0.4f;
const float AUTO_ROTATE_SPEED = 0.3f;

namespace AUTO_STRATEGIES
{
	// Instruction Format: Type, Target, Speed

	// Place block in exchange
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_TO, 36 - 16.92, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -45, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -90, AUTO_ROTATE_SPEED},

		// Drive left down field 12.011 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144.132 - 33.84, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -135, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -180, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30 - 16.92, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet RIGHTEXCHANGE = {RIGHT_EXCHANGE_STEPS, 13};

	// Gets to the exchange zone from the left side placement on the field
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 30 degrees.
		{AutoController::DRIVE_TO, 36, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 45, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 68.4, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 90, AUTO_ROTATE_SPEED},

		// Drive left down field 10.989 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 131.868 - 33.84, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 135, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 180, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30 - 16.92, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet LEFTEXCHANGE = {LEFT_EXCHANGE_STEPS, 13};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};
}

#endif
