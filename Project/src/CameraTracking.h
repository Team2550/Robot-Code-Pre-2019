#ifndef CAMERA_TRACKING_H
#define CAMERA_TRACKING_H

#include <WPILib.h>
#include "grip/GripPipeline.h"

class CameraTracking {
private:
	int imgWidth;
	int imgHeight;
	int imgExposure;

	cs::CvSink cvSink; // OpenCV image data sink
	grip::GripPipeline gripPipeline;

	int targetX;
	int targetY;
	bool targetTracked;

	int ultraPlaceholder;

	struct position
	{
		position() = default;
		position(float x, float y);

		float x = 0.0;
		float y = 0.0;
	};

public:
	CameraTracking();
	~CameraTracking();

	void UpdateVision();
	float GetTargetX();
	float GetTargetY();

	double GetRobotPosition(float position);
	float CalculateAngle(float x, float y);

};

#endif
