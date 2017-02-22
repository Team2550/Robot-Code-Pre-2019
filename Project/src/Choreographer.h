#ifndef CHOREO_H
#define CHOREO_H

#include <vector>
#include "Constants.h"
#include "DriveBase.h"

class Choreographer {
private:
	struct Period {
		float time;
		float leftSpeed;
		float rightSpeed;
	};

	std::vector<Period> timetable;

public:
	Choreographer(int timePeriodCount, const float _timetable[][3]); // Array of arrays, inner arrays are formatted as (time, left, right)

	void applySchedule(float time, DriveBase& driveBase);
};

#endif
