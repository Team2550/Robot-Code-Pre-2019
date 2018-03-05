/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include "OI.h"

#include <WPILib.h>

OI::OI()
{
	// Process operator interface input here.
	driveController = new Joystick(1);

	JoystickButton* button1 = new JoystickButton(driveController, 1),
				    button2 = new JoystickButton(driveController, 2),
				    button3 = new JoystickButton(driveController, 3),
				    button4 = new JoystickButton(driveController, 4),
				    button5 = new JoystickButton(driveController, 5),
				    button6 = new JoystickButton(driveController, 6),
				    button7 = new JoystickButton(driveController, 7),
				    button8 = new JoystickButton(driveController, 8);

	perifController = new Joystick(1);

	JoystickButton* button1 = new JoystickButton(perifController, 1),
					button2 = new JoystickButton(perifController, 2),
					button3 = new JoystickButton(perifController, 3),
					button4 = new JoystickButton(perifController, 4),
					button5 = new JoystickButton(perifController, 5),
					button6 = new JoystickButton(perifController, 6),
					button7 = new JoystickButton(perifController, 7),
					button8 = new JoystickButton(perifController, 8);


	//Associate joysticks with commmands

	/* button1->WhenPressed(new PrepareToGrab());
	button2->WhenPressed(new Grab());
	button3->WhenPressed(new DriveToDistance(0.11));
	button4->WhenPressed(new PlaceSoda());
	button6->WhenPressed(new DriveToDistance(0.2));
	button8->WhenPressed(new Stow());

	button7->WhenPressed(new SodaDelivery()); */
}
