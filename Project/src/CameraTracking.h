#ifndef CAMERA_TRACKING_H
#define CAMERA_TRACKING_H

#include <WPILib.h>
#include "grip/GripPipeline.h"

class CameraTracking
{
private:
	int imgWidth;
	int imgHeight;
	int imgExposure;

	cs::CvSink cvSink; // OpenCV image data sink
	grip::GripPipeline gripPipeline;

	int targetX;
	int targetY;

	cs::CvSink cvSink; // OpenCV image data sink

public:
	CameraTracking();
	~CameraTracking();
	void Init();
	void Auto();

	void UpdateVision();
	int GetTargetX();
	int GetTargetY();

};

#endif
