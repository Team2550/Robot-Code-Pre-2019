#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	// Instruction Format: Type, Target, Speed

	// Place block in exchange
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_TO, 36 - 16.92, 0.2f},
		{AutoController::ROTATE_TO, -45, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, 0.2f},
		{AutoController::ROTATE_TO, -90, 0.2f},

		// Drive left down field 12.011 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144.132 - 33.84, 0.2f},
		{AutoController::ROTATE_TO, -135, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, 0.2f},
		{AutoController::ROTATE_TO, -180, 0.2f},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30 - 16.92, 0.2f}
	};
	const AutoController::InstructionSet RIGHTEXCHANGE = {RIGHT_EXCHANGE_STEPS, 13};

	//Gets to the exchange zone from the left side placement on the field
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 30 degrees.
		{AutoController::DRIVE_TO, 36, 0.2f},
		{AutoController::ROTATE_TO, 30, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 68.4, 0.2f},
		{AutoController::ROTATE_TO, 60, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 68.4, 0.2f},
		{AutoController::ROTATE_TO, 90, 0.2f},

		// Drive left down field 10.989 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 131.868 - 33.84, 0.2f},
		{AutoController::ROTATE_TO, 135, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, 0.2f},
		{AutoController::ROTATE_TO, 180, 0.2f},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30 - 16.92, 0.2f}
	};
	const AutoController::InstructionSet LEFTEXCHANGE = {LEFT_EXCHANGE_STEPS, 16};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};

	const AutoController::Instruction AUTOLINE_STEPS[] =
	{
			{AutoController::DRIVE_TO, 144, .2f}
	};
	const AutoController::InstructionSet AUTOLINE = {AUTOLINE_STEPS, 1};

	/**const AutoController::Instruction LOOKDANGCOOL_STEPS[] =
	{
			{AutoController::DRIVE_TO, 144, .2f},
			{AutoController::ROTATE_TO, -90, .2f},

			{AutoController::RESET_DIST_0, 0, 0},
			{AutoController::DRIVE_TO, 144, .2f},


	};**/
}

#endif
