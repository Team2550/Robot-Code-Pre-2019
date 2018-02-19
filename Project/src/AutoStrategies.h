#ifndef AUTOSTRATEGIES_H
#define AUTOSTRATEGIES_H

#include "AutoController.h"

namespace AUTO_STRATEGIES
{
	// Instruction Format: Type, Target, Speed

	// Place block in exchange
	const AutoController::Instruction RIGHT_EXCHANGE_STEPS[] =
	{
		// Drive a x distance of 33.84 inches and y distance of 30 inches at a pace of 6 sections
		{AutoController::DRIVE_POINTS, 33.84, 36, 6, .2f, -90},

		// Drive left down field 12.011 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144.132 - 33.84, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_POINTS, 33.84, 36, 6, .2f, -180},

		// Drive towards wall for 30 inches (slightly less than three feet).
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 30 - 16.92, 0.2f}
	};
	const AutoController::InstructionSet RIGHTEXCHANGE = {RIGHT_EXCHANGE_STEPS, 13};

	//Gets to the exchange zone from the left side placement on the field
	const AutoController::Instruction LEFT_EXCHANGE_STEPS[] =
	{
		// Drive a x distance of 33.84 inches and y distance of 30 inches at a pace of 6 sections
		{AutoController::DRIVE_POINTS, 33.84, 30, 6, .2f, 90},

		// Drive left down field 10.989 feet.
		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 131.868 - 33.84, 0.2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_POINTS, 33.84, 30, 6, .2f, 180},

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

	//This makes the robot drive around the switch twice
	const AutoController::Instruction LOOKDANGCOOL_STEPS[] =
	{
		{AutoController::DRIVE_TO, 144, .2f},
		{AutoController::ROTATE_TO, -90, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 235, .2f},
		{AutoController::ROTATE_TO, -180, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144 - 36, .2f},
		{AutoController::ROTATE_TO, -270, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 235, .2f},
		{AutoController::ROTATE_TO, 0, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144 - 36, .2f},
		{AutoController::ROTATE_TO, -90, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 235, .2f},
		{AutoController::ROTATE_TO, -180, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 144 - 36, .2f},
		{AutoController::ROTATE_TO, -270, .2f},

		{AutoController::RESET_DIST_0, 0, 0},
		{AutoController::DRIVE_TO, 235, .2f},
		{AutoController::ROTATE_TO, 0, .2f}
	};
	const AutoController::InstructionSet LOOKDANGCOOL = {LOOKDANGCOOL_STEPS, 23};

	//Rotate around one then back to starting position
	const AutoController::Instruction ROTATE_STEPS[] =
		{
				{AutoController::ROTATE_TO, -360, .2f},
				{AutoController::ROTATE_TO, 0, .2f}
		};
	const AutoController::InstructionSet ROTATE = {ROTATE_STEPS, 2};
}

#endif
