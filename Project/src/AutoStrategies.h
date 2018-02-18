#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	const float AUTO_DRIVE_SPEED = 0.4f;
	const float AUTO_ROTATE_SPEED = 0.35f;
	const float ROBOT_WIDTH = 31.25f;
	const float PLAYER_STATION_WIDTH = 69.0f;
	const float EXCHANGE_WIDTH = 48.0f;
	const float EXCHANGE_OFFSET = -5.0f;

	// Instruction Format: Type, Target, Speed / Left Speed, Right Speed
	// Speed values are multipliers to each side. The signs of turning instructions are managed later on, having a negative speed on a rotate instruction
	// infers that both sides will be moving the same direction instead of opposite directions. Be careful when doing this, as it can lead to the robot
	// spinning and never achieving the target angle.

	// Place block in exchange, starting just to the right of the center player, from their perspective.
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_TO, 42 - (24 / sqrt(2)), AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -45, 0, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -90, 0, AUTO_ROTATE_SPEED},

		// Drive left down field to put robot center over exchange hole.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO,
				PLAYER_STATION_WIDTH + EXCHANGE_WIDTH / 2 + EXCHANGE_OFFSET - ROBOT_WIDTH / 2 - (48 / sqrt(2)),
				AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -135, 0, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -180, 0, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 36 - (24 / sqrt(2)), AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet RIGHTEXCHANGE = {RIGHT_EXCHANGE_STEPS, 13};

	// Gets to the exchange zone from the left side placement on the field
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_TO, 42 - (24 / sqrt(2)), AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 45, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 90, 0, AUTO_ROTATE_SPEED},

		// Drive right down field 12.011 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 134.868 - (48 / sqrt(2)), AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 135, 0, AUTO_ROTATE_SPEED},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 180, 0, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 36 - (24 / sqrt(2)), AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet LEFTEXCHANGE = {LEFT_EXCHANGE_STEPS, 13};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};
}

#endif
