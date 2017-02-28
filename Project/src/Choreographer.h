#ifndef CHOREO_H
#define CHOREO_H

#include <vector>
#include "Constants.h"
#include "DriveBase.h"

struct Period {
	float time;
	float leftSpeed;
	float rightSpeed;
};

class Choreographer {
private:
	std::vector<Period> timetable;

public:
	Choreographer();
	Choreographer(int timePeriodCount, const float _timetable[][3]); // Array of arrays, inner arrays are formatted as (time, left, right)

	Period getPeriod(int index);
	void setPeriod(int index, Period period);
	void setTimetable(int timePeriodCount, const float _timetable[][3]);

	void applyScheduleToRobot(float time, DriveBase& driveBase);
};

#endif
