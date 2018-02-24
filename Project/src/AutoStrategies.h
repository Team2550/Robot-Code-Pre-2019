#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	const float AUTO_DRIVE_SPEED = 0.45f; // Increased from 0.4 to better grip block while moving
	const float AUTO_ROTATE_SPEED = 0.375f;
	const float ROBOT_WIDTH = 31.25f;
	const float PLAYER_STATION_WIDTH = 69.0f; // 6 inches wider than stated width of table
	const float EXCHANGE_WIDTH = 48.0f;
	const float EXCHANGE_OFFSET = -5.0f;

	// Instruction Format: Type, Target, Left Speed, Right Speed (Defaults to equal left speed)

	// Place block in exchange, starting just to the right of the center player, from their perspective.
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_TO, 40 - (24 / sqrt(2)), AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.1, 0},
		{AutoController::ROTATE_TO, -45, 0, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.05, 0},

		// Diagonal
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.1, 0},
		{AutoController::ROTATE_TO, -90, 0, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.05, 0},

		// Drive left down field to put robot center over exchange hole.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO,
				PLAYER_STATION_WIDTH + EXCHANGE_WIDTH / 2 + EXCHANGE_OFFSET - ROBOT_WIDTH / 2 - (48 / sqrt(2)),
				AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.1, 0},
		{AutoController::ROTATE_TO, -135, 0, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.05, 0},

		// Diagonal
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 24, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.1, 0},
		{AutoController::ROTATE_TO, -180, 0, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.05, 0},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 32 - (24 / sqrt(2)), AUTO_DRIVE_SPEED},

		// Extend bulldozer and wait
		{AutoController::EXTEND, 0, 0},
		{AutoController::WAIT_TIME, 0.8, 0},

		// Retract and backup to auto line
		{AutoController::RETRACT, 0, 0},
		{AutoController::DRIVE_DIST, -98, AUTO_DRIVE_SPEED * 1.2}
	};
	const AutoController::InstructionSet RIGHT_EXCHANGE = {RIGHT_EXCHANGE_STEPS, 25};

	// Right Ram and Grab
	const AutoController::Instruction RIGHT_RAM_AND_GRAB_STEPS[] =
	{
		// Ram into switch
		{AutoController::DRIVE_TO, 50, AUTO_DRIVE_SPEED},
		{AutoController::DRIVE_TO, 100, AUTO_DRIVE_SPEED * 1.2}, //1},

		// Backup and drive around switch
		{AutoController::DRIVE_DIST, -24, AUTO_DRIVE_SPEED}, // Backup
		{AutoController::ROTATE_TO, 90, AUTO_ROTATE_SPEED}, // Turn to the right
		{AutoController::DRIVE_DIST, 48, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, 0, AUTO_ROTATE_SPEED}, // Turn forward
		{AutoController::DRIVE_DIST, 132, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, -90, AUTO_ROTATE_SPEED}, // Turn left
		{AutoController::RESET_DIST_0, 0, 0}, // Drive toward block
		{AutoController::DRIVE_TO, 105, AUTO_DRIVE_SPEED * 1.2}, // Drive toward block
		{AutoController::DRIVE_TO, 150, AUTO_DRIVE_SPEED}, // Drive toward block

		// Push block
		{AutoController::ROTATE_TO, -135, AUTO_ROTATE_SPEED}, // Turn toward block
		{AutoController::DRIVE_DIST, 24, AUTO_DRIVE_SPEED}, // Drive and push block
		{AutoController::ROTATE_TO, -90, AUTO_ROTATE_SPEED, 0}, // Rotate right side backwards
		//{AutoController::DRIVE_DIST, 24, AUTO_DRIVE_SPEED}, // Drive left

		// Drive around left side of switch
		{AutoController::ROTATE_TO, -135, 0, AUTO_ROTATE_SPEED},
		{AutoController::DRIVE_DIST, 24, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -180, 0, AUTO_ROTATE_SPEED},
		{AutoController::DRIVE_DIST, 50, AUTO_DRIVE_SPEED},

		// Drive diagonally towards the exchange
		{AutoController::ROTATE_TO, -225, 0, AUTO_ROTATE_SPEED},
		{AutoController::DRIVE_DIST, 100, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -180, AUTO_ROTATE_SPEED, 0},
		{AutoController::DRIVE_DIST, 12, AUTO_DRIVE_SPEED},

		// Extend the bulldozer
		{AutoController::EXTEND, 0, 0},
		{AutoController::WAIT_TIME, 0.5, 0},
		{AutoController::RETRACT, 0, 0},
		{AutoController::DRIVE_DIST, -20, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -225, -AUTO_ROTATE_SPEED, 0},
		{AutoController::DRIVE_DIST, -120, AUTO_DRIVE_SPEED},
	};
	const AutoController::InstructionSet RIGHT_RAM_AND_GRAB = {RIGHT_RAM_AND_GRAB_STEPS, 28};

	// Cross line
	const AutoController::Instruction CROSS_STEPS[] =
	{
		{AutoController::DRIVE_TO, 102, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet CROSS = {CROSS_STEPS, 1};

	// Backup
	const AutoController::Instruction BACKUP_STEPS[] =
	{
		{AutoController::DRIVE_TO, -120, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet BACKUP = {BACKUP_STEPS, 1};

	// Ram
	const AutoController::Instruction RAM_STEPS[] =
	{
		{AutoController::DRIVE_TO, 48, AUTO_DRIVE_SPEED},
		{AutoController::DRIVE_TO, 100, 1}
	};
	const AutoController::InstructionSet RAM = {RAM_STEPS, 2};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};
}

#endif
