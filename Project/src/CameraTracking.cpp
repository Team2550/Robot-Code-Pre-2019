#include "CameraTracking.h"

int CameraTracking::imgWidth = 480;
int CameraTracking::imgHeight = 360;
int CameraTracking::imgExposure = 100;

Vector2 CameraTracking::targetPositionRelative = {0, 0};
bool CameraTracking::targetIsVisible = false;

CameraTracking::CameraTracking(int imgWidth, int imgHeight, int imgExposure)
{
	CameraTracking::imgWidth = imgWidth;
	CameraTracking::imgHeight = imgHeight;
	CameraTracking::imgExposure = imgExposure;

	targetPositionRelative.x = 0;
	targetPositionRelative.y = 0;

	targetIsVisible = false;
}

CameraTracking::~CameraTracking()
{

}

void CameraTracking::VisionThread()
{
	// Get the USB camera from CameraServer
	cs::UsbCamera camera =
			CameraServer::GetInstance()
					->StartAutomaticCapture();
	// Set the resolution
	camera.SetResolution(640, 480);

	// Get a CvSink. This will capture Mats from the Camera
	cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	cs::CvSource outputStream =
			CameraServer::GetInstance()->PutVideo(
					"Rectangle", 640, 480);

	// Mats are very memory expensive. Lets reuse this Mat.
	cv::Mat mat;

	while (true)
	{
		// Tell the CvSink to grab a frame from the camera and
		// put it
		// in the source mat.  If there is an error notify the
		// output.
		if (cvSink.GrabFrame(mat) == 0) {
			// Send the output the error.
			outputStream.NotifyError(cvSink.GetError());
			// skip the rest of the current iteration
			continue;
		}
		// Put a rectangle on the image
		rectangle(mat, cv::Point(100, 100), cv::Point(400, 400),
				cv::Scalar(255, 255, 255), 5);
		// Give the output stream a new image to display
		outputStream.PutFrame(mat);
	}
}

void CameraTracking::LaunchVisionThread()
{
	std::thread visionThread(VisionThread);
	visionThread.detach();
}

/*void CameraTracking::UpdateVision()
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
}*/

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

