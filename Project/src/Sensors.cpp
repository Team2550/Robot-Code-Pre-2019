#include <Sensors.h>
int main(){
	Ultrasonic::initializeUltrasonic();
	if(Ultrasonic::read == true){
		Ultrasonic::readUltrasonic();
	}
}
