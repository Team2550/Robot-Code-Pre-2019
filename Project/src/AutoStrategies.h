#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	const float AUTO_DRIVE_SPEED = 0.45f; // Increased from 0.4 to better grip block while moving
	const float AUTO_ROTATE_SPEED = 0.375f;

	const float ROBOT_WIDTH = 31.25f;
	const float ROBOT_LENGTH = 24.0f;

	const float PLAYER_STATION_WIDTH = 72.0f;
	const float EXCHANGE_WIDTH = 48.0f;
	const float EXCHANGE_OFFSET = 5.0f; // Exchange slot is slightly offset from the center of the exchange

	const float SWITCH_DIST = 140.0f;

	const float SWITCH_WIDTH = 56.0f;
	const float SWITCH_LENGTH = 152.88f;

	// Instruction Format: Type, Target (Default = 0), Stop at Target (Default = true),
	//                     Left Speed (Default = 0), Right Speed (Default = left speed)

	// Place block in exchange, starting just to the right of the center player, from their perspective.

	// Right Position Options
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_DIST, 40 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, -45, true, 0, AUTO_ROTATE_SPEED},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, -90, true, 0, AUTO_ROTATE_SPEED},

		// Drive left down field to put robot center over exchange hole.
		{AutoController::DRIVE_DIST,
				PLAYER_STATION_WIDTH - ROBOT_WIDTH + EXCHANGE_WIDTH / 2 - EXCHANGE_OFFSET - ROBOT_WIDTH / 2 - (48 / sqrt(2)),
				true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, -135, true, 0, AUTO_ROTATE_SPEED},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, -180, true, 0, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::DRIVE_DIST, 32 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},

		// Extend bulldozer and wait
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.75},

		// Retract and backup to auto line
		{AutoController::RETRACT},
		{AutoController::DRIVE_DIST, 120 - ROBOT_LENGTH + 12, true, AUTO_DRIVE_SPEED * 1.2}
	};
	const AutoController::InstructionSet RIGHT_EXCHANGE = {RIGHT_EXCHANGE_STEPS, 17};

	// Right Ram and Grab
	const AutoController::Instruction RIGHT_RAM_AND_GRAB_STEPS[] =
	{
		// Ram into switch
		{AutoController::DRIVE_TO, 50, false, AUTO_DRIVE_SPEED},
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 12, false, AUTO_DRIVE_SPEED * 1.2}, //1},

		// Backup
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 36, true, AUTO_DRIVE_SPEED},

		// Drive around switch
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED}, // Turn to the right
		{AutoController::DRIVE_DIST, 60, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, 0, true, AUTO_ROTATE_SPEED}, // Turn forward
		{AutoController::DRIVE_DIST, SWITCH_WIDTH + 48 + ROBOT_LENGTH, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, -90, true, AUTO_ROTATE_SPEED}, // Turn left
		{AutoController::DRIVE_TO, SWITCH_LENGTH, true, AUTO_DRIVE_SPEED * 1.2}, // Drive along length of switch
	};
	const AutoController::InstructionSet RIGHT_RAM_AND_GRAB = {RIGHT_RAM_AND_GRAB_STEPS, 9};

	const AutoController::PositionOptions RIGHT_OPTIONS = {&RIGHT_EXCHANGE, &RIGHT_RAM_AND_GRAB};


	// Left Position Options
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_DIST, 40 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 45, true, 0, AUTO_ROTATE_SPEED},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 90, true, 0, AUTO_ROTATE_SPEED},

		// Drive left down field to put robot center over exchange hole.
		{AutoController::DRIVE_DIST,
				EXCHANGE_WIDTH / 2 + EXCHANGE_OFFSET - ROBOT_WIDTH / 2 - (48 / sqrt(2)),
				true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 135, true, 0, AUTO_ROTATE_SPEED},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 180, true, 0, AUTO_ROTATE_SPEED},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::DRIVE_DIST, 32 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},

		// Extend bulldozer and wait
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.75},

		// Retract and backup to auto line
		{AutoController::RETRACT},
		{AutoController::DRIVE_DIST, 120 - ROBOT_LENGTH + 12, true, AUTO_DRIVE_SPEED * 1.2}
	};
	const AutoController::InstructionSet LEFT_EXCHANGE = {LEFT_EXCHANGE_STEPS, 17};

	// Right Ram and Grab
	const AutoController::Instruction LEFT_RAM_AND_GRAB_STEPS[] =
	{
		// Ram into switch
		{AutoController::DRIVE_TO, 50, false, AUTO_DRIVE_SPEED},
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 12, false, AUTO_DRIVE_SPEED * 1.2}, //1},

		// Backup
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 36, true, AUTO_DRIVE_SPEED},

		// Drive around switch
		{AutoController::ROTATE_TO, -90, true, AUTO_ROTATE_SPEED}, // Turn to the right
		{AutoController::DRIVE_DIST, 36, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, 0, true, AUTO_ROTATE_SPEED}, // Turn forward
		{AutoController::DRIVE_DIST, SWITCH_WIDTH + 48 + ROBOT_LENGTH, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED}, // Turn left
	};
	const AutoController::InstructionSet LEFT_RAM_AND_GRAB = {LEFT_RAM_AND_GRAB_STEPS, 8};

	const AutoController::PositionOptions LEFT_OPTIONS = {&LEFT_RAM_AND_GRAB, &LEFT_EXCHANGE};


	// Cross line
	const AutoController::Instruction CROSS_STEPS[] =
	{
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 12, true, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet CROSS = {CROSS_STEPS, 1};

	const AutoController::PositionOptions CROSS_OPTIONS = {&CROSS, &CROSS};


	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};

	const AutoController::PositionOptions NOTHING_OPTIONS = {&NOTHING, &NOTHING};
}

#endif
