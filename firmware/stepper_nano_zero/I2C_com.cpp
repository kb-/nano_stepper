#include "I2C_com.h"
#include <string.h>
#include <stdio.h>
#include <Wire.h>
#include "I2C_Anything.h"

#define CTRL_I2C_ADDR 12
#define CTRL_SEND_INTERVAL 2000
#define I2C_CLOCK 100000

int32_t i2c_com::geterr(void){
	int32_t err = abs(ptrStepperCtrl->getLoopError());
	if (err > maxerr)
	{
		maxerr = err;
	}
	return maxerr;
}


void i2c_com::setup(StepperCtrl *ptrsCtrl){
	ptrStepperCtrl=ptrsCtrl;
}

void i2c_com::process(void){//call this in loop
	send(data_to_slave);
}
void i2c_com::process(i2c_com_data data){
	send(data);
}


void i2c_com::send(i2c_com_data data){
  static uint32_t t=0;
  static uint32_t t1=0;
  if ((millis()-t)>CTRL_SEND_INTERVAL)//delay, without delay()
  {
	t=millis();
	Wire.beginTransmission (CTRL_I2C_ADDR);
	Wire.write ((uint8_t*) &data_to_slave, sizeof(i2c_com_data));
	Wire.endTransmission ();
	maxerr = 0;//reset stored error
  }
  if ((millis()-t1)>CTRL_SEND_INTERVAL)//delay, without delay()
  {
	Wire.requestFrom(CTRL_I2C_ADDR, 2*sizeof(data_to_slave));
	t1=millis();
	do{
	  I2C_readAnything(data_from_slave);
	}while(data_from_slave.action!='Z');//reading till the expected data is delivered
	Serial.println (data_from_slave.id);
	Serial.println (data_from_slave.action);
	Serial.println (data_from_slave.data);
  }
}

void i2c_com::begin(void){
	Wire.begin();        // join i2c bus
	Wire.setClock(I2C_CLOCK);
}

class i2c_com i2c_com;
