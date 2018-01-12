#include <Sensors.h>
int main(){
	sensors::ultrasonic::initialize.initializeUltrasonic();
	if(sensors::ultrasonic::initialize.read == true){
		sensors::ultrasonic::initialize.readUltrasonic();
	}
}
