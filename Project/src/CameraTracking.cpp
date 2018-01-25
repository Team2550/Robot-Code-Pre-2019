#include <CameraTracking.h>
#include <GripPipeline.h>
#include <math.h>

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

	std::vector<cv::Point> contourData = gripPipeline.GetFilterContoursOutput()[0];

	int minX = contourData[0].x;
	int maxX = contourData[0].x;
	int minY = contourData[0].y;
	int maxY = contourData[0].y;

	for (int point = 1; point < contourData; point++)
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
	//TODO need to update the position struct;
	position = gripPipeline.GetFilterContoursOutput();

	//If the target is detected, return the X and Y positions and calculate the angle
	while (targetTracked = true)
	{
		GetTargetX();
		GetTargetY();
		CalculateAngle(targetX, targetY);
	}

	return position;
}

float CameraTracking::CalculateAngle(float x, float y)
{
	float angle;
	float finalAngle;

	//calculate angle based on ultrasonic readings
	//angle = arctan();
	finalAngle = angle * 180 / M_PI; //convert radians to degrees

	return finalAngle;
}

/* Possible solutions
 *  atan2(max.y - min.y, max.x - min.x).
*/

