#include <Choreographer.h>

Choreographer::Choreographer()
{
	timetable.clear();
}

Choreographer::Choreographer(int timePeriodCount, const float _timetable[][3])
{
	setTimetable(timePeriodCount, _timetable);
}

Period Choreographer::getPeriod(int index)
{
	return timetable[index];
}

void Choreographer::setPeriod(int index, Period period)
{
	timetable[index] = period;
}

void Choreographer::setTimetable(int timePeriodCount, const float _timetable[][3])
{
	float currentTime = 0;
	for (int i = 0; i < timePeriodCount; i++)
	{
		Period newPeriod;
		newPeriod.time = (currentTime += _timetable[i][0]);
		newPeriod.leftSpeed = _timetable[i][1];
		newPeriod.rightSpeed = _timetable[i][2];
		timetable.push_back(newPeriod);
	}
}

void Choreographer::applyScheduleToRobot(float time, DriveBase& driveBase)
{
	unsigned int i = 0;
	while (i < timetable.size() && time > timetable[i++].time) {}

	if (i >= timetable.size())
		driveBase.drive(0);
	else
		driveBase.drive(timetable[i].leftSpeed, timetable[i].rightSpeed);
}
