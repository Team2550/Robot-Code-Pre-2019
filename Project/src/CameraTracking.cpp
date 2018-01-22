#include <CameraTracking.h>
#include <grip/GripPipeline.h>

CameraTracking::CameraTracking() : gripPipeline()
{
	imgWidth = 640;
	imgHeight = 480;
	imgExposure = 10;

	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera.SetResolution(imgWidth, imgHeight);
	camera.SetExposureManual(imgExposure);
	cvSink = CameraServer::GetInstance()->GetVideo();

	targetX = 0;
	targetY = 0;
}

CameraTracking::~CameraTracking()
{

}


void CameraTracking::UpdateVision()
{
	cv::Mat image;
	cvSink.GrabFrame(image);
	gripPipeline.Process(image);

	std::vector<cv::Point> contourData = (*gripPipeline.GetFilterContoursOutput())[0];

	int minX = contourData[0].x;
	int maxX = contourData[0].x;
	int minY = contourData[0].y;
	int maxY = contourData[0].y;

	for (unsigned int point = 1; point < contourData.size(); point++)
	{
		if (contourData[point].x < minX)
			minX = contourData[point].x;

		if (contourData[point].x > maxX)
			maxX = contourData[point].x;

		if (contourData[point].y < minY)
			minY = contourData[point].y;

		if (contourData[point].y > maxY)
			maxY = contourData[point].y;
	}

	targetX = (minX + maxX) / 2;
	targetY = (minY + maxY) / 2;
}

int CameraTracking::GetTargetX()
{
	return targetX;
}

int CameraTracking::GetTargetY()
{
	return targetY;
}
