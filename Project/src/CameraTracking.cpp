#include <CameraTracking.h>

CameraTracking::CameraTracking(int imgWidth, int imgHeight, int imgExposure)
{
	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera.SetResolution(imgWidth, imgHeight);
	camera.SetExposureManual(imgExposure);
	cvSink = CameraServer::GetInstance()->GetVideo();

	targetPositionRelative.x = 0;
	targetPositionRelative.y = 0;

	targetIsVisible = false;
}

CameraTracking::~CameraTracking()
{

}

void CameraTracking::UpdateVision()
{
	cv::Mat image;
	cvSink.GrabFrame(image);
	gripPipeline.Process(image);

	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;

	std::vector<std::vector<cv::Point>>* contours = gripPipeline.GetFilterContoursOutput();

	if (contours->size() > 0)
	{
		std::vector<cv::Point> targetContourData = (*contours)[0];

		minX = targetContourData[0].x;
		maxX = targetContourData[0].x;
		minY = targetContourData[0].y;
		maxY = targetContourData[0].y;

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
	}

	Vector2 target = {(minX + maxX) / 2.f, (minY + maxY) / 2.f};

	//If the target is not zero, then the target is found
	if (target.x == 0 && target.y == 0)
	{
		targetIsVisible = false;
	}

	targetPositionRelative.x = target.x;
	targetPositionRelative.y = target.y;
}

Vector2 CameraTracking::GetTargetPositionRelative()
{
	return targetPositionRelative;
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

