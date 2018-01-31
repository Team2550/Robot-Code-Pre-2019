#include <CameraTracking.h>

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

	targetTracked = false;

	ultraPlaceholder = 0;
}

CameraTracking::~CameraTracking()
{

}

void CameraTracking::UpdateVision()
{
	cv::Mat image;
	cvSink.GrabFrame(image);
	gripPipeline.Process(image);

	std::vector<std::vector<cv::Point>>* contours = gripPipeline.GetFilterContoursOutput();
	std::vector<cv::Point> targetContourData = (*contours)[0];

	int minX = targetContourData[0].x;
	int maxX = targetContourData[0].x;
	int minY = targetContourData[0].y;
	int maxY = targetContourData[0].y;

	for (unsigned int point = 1; point < targetContourData.size(); point++)
	{
		if (targetContourData[point].x < minX)
			minX = targetContourData[point].x;

		if (targetContourData[point].x > maxX)
			maxX = targetContourData[point].x;

		if (targetContourData[point].y < minY)
			minY = targetContourData[point].y;

		if (targetContourData[point].y > maxY)
			maxY = targetContourData[point].y;
	}

	targetX = (minX + maxX) / 2;
	targetY = (minY + maxY) / 2;

	//If the target is not zero, then the target is found
	if (targetX > 0 || targetX < 0 || targetY > 0 || targetY < 0)
	{
		targetTracked = true;
	}
}

float CameraTracking::GetTargetX()
{
	return targetX;
}

float CameraTracking::GetTargetY()
{
	return targetY;
}


double CameraTracking::GetRobotPosition(float position)
{
	return position;
}

float CameraTracking::CalculateAngle(float x, float y)
{
	float angle = 0;
	float finalAngle = 0;

	//calculate angle based on ultrasonic readings
	//angle = arctan();
	finalAngle = angle * 180 / M_PI; //convert radians to degrees

	return finalAngle;
}

/* Possible solutions
 *  atan2(max.y - min.y, max.x - min.x).
*/

