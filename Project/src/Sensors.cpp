#include <Sensors.h>
int main(){
	//defined device address to I2C ultrasonic sensor
		void openI2C()
		{
			//the I2C::Port is a preset bool. Only device address needs to be defined
			I2C(I2C::Port(1), deviceAddress);
		}
		void readUltrasonic(){
			I2C::ReadOnly(readCount);
		}
		//function that can be ran to automatically setup the ultrasonic system
		void initializeUltrasonic(){
			openI2C();

		}
	Ultrasonic::initializeUltrasonic();
	if(read == true){
		Ultrasonic::readUltrasonic();


	}
}
