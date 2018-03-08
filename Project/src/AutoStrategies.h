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
	const float EXCHANGE_DEPTH = 36.0f;
	const float EXCHANGE_OFFSET = 5.0f; // Exchange slot is slightly offset from the center of the exchange

	const float SWITCH_DIST = 140.0f;

	const float SWITCH_WIDTH = 56.0f;
	const float SWITCH_LENGTH = 152.88f;

	// Instruction Format: Type, Target (Default = 0), Stop at Target (Default = true),
	//                     Left Speed (Default = 0), Right Speed (Default = left speed)

	// =========================== EXCHANGE OPTIONS ===========================
	// Right Starting Position
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
		{AutoController::DRIVE_DIST, -(SWITCH_DIST - ROBOT_LENGTH + 4), true, AUTO_DRIVE_SPEED * 1.2}
	};
	const AutoController::InstructionSet RIGHT_EXCHANGE = {RIGHT_EXCHANGE_STEPS, 17};

	const AutoController::PositionOptions RIGHT_EXCHANGE_OPTIONS = {&RIGHT_EXCHANGE, &RIGHT_EXCHANGE};


	// Left Starting Position
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive from wall to distance of three feet from wall. Drives along hypotenuse of length two feet at angle of 45 degrees.
		{AutoController::DRIVE_DIST, 40 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 45, true, AUTO_ROTATE_SPEED, 0},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED, 0},

		// Drive left down field to put robot center over exchange hole.
		{AutoController::DRIVE_DIST,
				PLAYER_STATION_WIDTH - ROBOT_WIDTH + EXCHANGE_WIDTH / 2 + EXCHANGE_OFFSET - ROBOT_WIDTH / 2 - (48 / sqrt(2)),
				true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 135, true, AUTO_ROTATE_SPEED, 0},

		// Diagonal
		{AutoController::DRIVE_DIST, 24, true, AUTO_DRIVE_SPEED},
		{AutoController::WAIT_TIME, 0.05},
		{AutoController::ROTATE_TO, 180, true, AUTO_ROTATE_SPEED, 0},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::DRIVE_DIST, 32 - (24 / sqrt(2)), true, AUTO_DRIVE_SPEED},

		// Extend bulldozer and wait
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.75},

		// Retract and backup to auto line
		{AutoController::RETRACT},
		{AutoController::DRIVE_DIST, -(SWITCH_DIST - ROBOT_LENGTH + 4), true, AUTO_DRIVE_SPEED * 1.2}
	};
	const AutoController::InstructionSet LEFT_EXCHANGE = {LEFT_EXCHANGE_STEPS, 17};

	const AutoController::PositionOptions LEFT_EXCHANGE_OPTIONS = {&LEFT_EXCHANGE, &LEFT_EXCHANGE};


	// =========================== SWITCH OPTIONS ===========================
	// Right Starting Position
	// Ownership of right side of switch
	const AutoController::Instruction RIGHT_SWITCH_SAME_SIDE_STEPS[] =
	{
		// Push block onto switch
		{AutoController::RESET_DIST_0},
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 8, true, AUTO_DRIVE_SPEED},
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::KICK},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::RETRACT},
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 36, true, AUTO_DRIVE_SPEED},

		// Drive around switch
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED}, // Turn to the right
		{AutoController::DRIVE_DIST, 60, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, 0, true, AUTO_ROTATE_SPEED}, // Turn forward
		{AutoController::DRIVE_DIST, SWITCH_WIDTH + 72 + ROBOT_LENGTH, true, AUTO_DRIVE_SPEED}, // Drive past switch
		{AutoController::ROTATE_TO, -90, true, AUTO_ROTATE_SPEED}, // Turn left
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::DRIVE_DIST, SWITCH_LENGTH, true, AUTO_DRIVE_SPEED * 1.2}, // Drive along length of switch
	};
	const AutoController::InstructionSet RIGHT_SWITCH_SAME_SIDE = {RIGHT_SWITCH_SAME_SIDE_STEPS, 15};

	// Ownership of left side of switch
	// Inner route
	const AutoController::Instruction RIGHT_SWITCH_OTHER_SIDE_INNER_STEPS[] =
	{
		// Drive around to left side of switch
		{AutoController::DRIVE_DIST, EXCHANGE_DEPTH + 12, true, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, -90, true, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.25},
		{AutoController::DRIVE_DIST, 2 * PLAYER_STATION_WIDTH + EXCHANGE_WIDTH - ROBOT_WIDTH / 2 - ROBOT_LENGTH / 2, true, AUTO_DRIVE_SPEED * 1.2},
		{AutoController::ROTATE_TO, 0, true, AUTO_ROTATE_SPEED},
		{AutoController::DRIVE_DIST, SWITCH_DIST + SWITCH_WIDTH / 2 - ROBOT_LENGTH / 2 - EXCHANGE_DEPTH - 12, true, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED},

		// Place block on switch
		{AutoController::DRIVE_DIST, 55.56 - ROBOT_LENGTH - 8, true, AUTO_DRIVE_SPEED},
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::KICK},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::RETRACT}
	};
	const AutoController::InstructionSet RIGHT_SWITCH_OTHER_SIDE_INNER = {RIGHT_SWITCH_OTHER_SIDE_INNER_STEPS, 13};

	// TODO: Outer Route


	const AutoController::PositionOptions RIGHT_SWITCH_INNER_OPTIONS = {&RIGHT_SWITCH_OTHER_SIDE_INNER, &RIGHT_SWITCH_SAME_SIDE};


	// Left Starting Position
	// Ownership of right side of switch
	// Inner route
	const AutoController::Instruction LEFT_SWITCH_OTHER_SIDE_INNER_STEPS[] =
	{
		// Drive around to right side of switch
		{AutoController::DRIVE_DIST, EXCHANGE_DEPTH + 12, true, AUTO_DRIVE_SPEED},
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED},
		{AutoController::WAIT_TIME, 0.25},
		{AutoController::DRIVE_DIST, 2 * PLAYER_STATION_WIDTH + EXCHANGE_WIDTH - ROBOT_WIDTH, true, AUTO_DRIVE_SPEED * 1.2},
		{AutoController::ROTATE_TO, 0, true, AUTO_ROTATE_SPEED},

		// Place block on switch
		{AutoController::DRIVE_DIST, SWITCH_DIST - ROBOT_LENGTH - EXCHANGE_DEPTH - 20, true, AUTO_DRIVE_SPEED},
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::KICK},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::RETRACT}
	};
	const AutoController::InstructionSet LEFT_SWITCH_OTHER_SIDE_INNER = {LEFT_SWITCH_OTHER_SIDE_INNER_STEPS, 11};

	// TODO: Outer route

	// Ownership of left side of switch
	const AutoController::Instruction LEFT_SWITCH_SAME_SIDE_STEPS[] =
	{
		// Drive forward and face switch
		{AutoController::DRIVE_DIST, SWITCH_DIST + SWITCH_WIDTH / 2 - ROBOT_LENGTH / 2, true, AUTO_DRIVE_SPEED * 1.2},
		{AutoController::ROTATE_TO, 90, true, AUTO_ROTATE_SPEED},

		// Place block on switch
		{AutoController::DRIVE_DIST, 55.56 - ROBOT_WIDTH / 2 - ROBOT_LENGTH / 2 - 8, true, AUTO_DRIVE_SPEED},
		{AutoController::EXTEND},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::KICK},
		{AutoController::WAIT_TIME, 0.5},
		{AutoController::RETRACT}
	};
	const AutoController::InstructionSet LEFT_SWITCH_SAME_SIDE = {LEFT_SWITCH_SAME_SIDE_STEPS, 8};


	const AutoController::PositionOptions LEFT_SWITCH_INNER_OPTIONS = {&LEFT_SWITCH_SAME_SIDE, &LEFT_SWITCH_OTHER_SIDE_INNER};


	// =========================== FALLBACK OPTIONS ===========================
	// Cross line
	const AutoController::Instruction CROSS_STEPS[] =
	{
		{AutoController::DRIVE_TO, SWITCH_DIST - ROBOT_LENGTH - 12, true, AUTO_DRIVE_SPEED}
	};
	const AutoController::InstructionSet CROSS = {CROSS_STEPS, 1};

	const AutoController::PositionOptions CROSS_OPTIONS = {&CROSS, &CROSS};

	// Cross line time-based
	const AutoController::Instruction CROSS_TIME_STEPS[] =
	{
		{AutoController::WAIT_TIME, 4, true, AUTO_DRIVE_SPEED * 0.8}
	};
	const AutoController::InstructionSet CROSS_TIME = {CROSS_TIME_STEPS, 1};

	const AutoController::PositionOptions CROSS_TIME_OPTIONS = {&CROSS_TIME, &CROSS_TIME};

	// Do nothing
	const AutoController::Instruction NOTHING_STEPS[] = {};
	const AutoController::InstructionSet NOTHING = {NOTHING_STEPS, 0};

	const AutoController::PositionOptions NOTHING_OPTIONS = {&NOTHING, &NOTHING};

	// Side-dependent line crossing options
	// Cross Right Only
	const AutoController::PositionOptions CROSS_RIGHT_OPTIONS = {&CROSS_TIME, &CROSS};

	// Cross Left Only
	const AutoController::PositionOptions CROSS_LEFT_OPTIONS = {&CROSS, &CROSS_TIME};
}

#endif
