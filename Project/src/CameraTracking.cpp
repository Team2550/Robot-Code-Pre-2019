#include "CameraTracking.h"

CameraTracking::CameraTracking(int imgWidth, int imgHeight, int imgExposure)
{
	cv::Mat frame;
	cv::VideoCapture cap(imgExposure);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 300);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600);
	bool bSuccess = cap.read(frame);
	grip::GripPipeline gp;
	return gripPipeline.Process(frame);

	targetPositionRelative.x = 0;
	targetPositionRelative.y = 0;

	targetIsVisible = false;

}

CameraTracking::~CameraTracking() {

}

void CameraTracking::VisionThread()
{
	// Get the USB camera from CameraServer,
	// start streaming to dashboard
	// (the vision processing will pick it up thusly)

		cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
		camera.SetResolution(640, 480);
		cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
		cs::CvSource outputStreamstd = CameraServer::GetInstance()->PutVideo("Gray", 640, 480);
		cv::Mat source;
		cv::Mat output;

		while (true)
		{
			cvSink.GrabFrame(source);
			cvtColor(source, output, cv::COLOR_BGR2GRAY);
			outputStreamstd.PutFrame(output);
		}
}

void CameraTracking::UpdateVision()
{
	cv::Mat source;
	std::cout << "Accessed UpdateVision" << std::endl;
	gripPipeline.Process(source);
	std::cout << "reached proccess" << std::endl;

	int minX = 0;
	int maxX = 0;
	int minY = 0;
	int maxY = 0;

	 std::vector<std::vector<cv::Point>>* contours = gripPipeline.GetFindContoursOutput();

	 if (contours->size() > 0)
	 {
	 std::cout << "coorPart1" << std::endl;
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

	 Vector2 target = { (minX + maxX) / 2.f, (minY + maxY) / 2.f };

	 //If the target is not zero, then the target is found
	 if (target.x != 0 && target.y != 0)
	 {
	 targetIsVisible = true;
	 std::cout << "target is found!!" << std::endl;
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

