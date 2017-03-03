#include <Choreographer.h>

Choreographer::Choreographer()
{
	timetable.clear();
}

Choreographer::Choreographer(int timePeriodCount, const float _timetable[][3])
{
	setTimetable(timePeriodCount, _timetable);
}

Period Choreographer::getPeriod(unsigned int index)
{
	if (index >= 0 && index < timetable.size())
		return timetable[index];
	else
		return {-1, -1, -1};
}

void Choreographer::setPeriod(unsigned int index, Period period)
{
	if (index >= 0 && index <= timetable.size())
	{
		if (index > 0)
			period.time += timetable[index - 1].time;

		if (index == timetable.size())
			timetable.push_back(period);
		else
		{
			float timeOffset = period.time - timetable[index].time;
			timetable[index] = period;

			for (unsigned int i = index + 1; i < timetable.size(); i++)
				timetable[i].time += timeOffset;
		}
	}
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
	// what part of timetable are we on
	while (i < timetable.size() && time > timetable[i++].time) {}

	if (i >= timetable.size())
		driveBase.drive(0); // STOP!! END OF TIMETABLE
	else
		driveBase.drive(timetable[i].leftSpeed, timetable[i].rightSpeed);
}
