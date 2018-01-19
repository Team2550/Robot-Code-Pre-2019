#ifndef CAMERA_TRACKING_H
#define CAMERA_TRACKING_H

class CameraTracking
{
private:

	cs::CvSink cvSink; // OpenCV image data sink

public:
	CameraTracking();
	~CameraTracking();
	void Init();
	void Auto();

};

#endif
