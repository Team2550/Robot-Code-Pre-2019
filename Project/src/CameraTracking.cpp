#include <CameraTracking.h>
#include <GripPipeline.h>

CameraTracking::CameraTracking()
{

}

//Destructor needed?
CameraTracking::~CameraTracking()
{

}

CameraTracking::Init()
{
	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera.SetResolution(640, 480);
	camera.SetExposureManual(25);
	cvSink = CameraServer::GetInstance()->GetVideo();
}

CameraTracking::Auto()
{
		// This code should be multithreaded to increase efficiency
		cv::Mat image;
		cvSink.GrabFrame(image);
		gripPipeline.Process(image);
		// ================================
}
