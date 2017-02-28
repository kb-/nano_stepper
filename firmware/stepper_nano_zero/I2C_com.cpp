#include "I2C_com.h"
#include <string.h>
#include <stdio.h>
#include <Wire.h>
#include "I2C_Anything.h"

#define CTRL_I2C_ADDR 12
#define CTRL_SEND_INTERVAL 2000
#define CTRL_GET_INTERVAL 500
#define I2C_CLOCK 100000

int32_t i2c_com::geterr(void){
	int32_t err = abs(ptrStepperCtrl->getLoopError());
	if (err > maxerr)
	{
		maxerr = err;
	}
	return maxerr;
}

void i2c_com::begin(void){
	Wire.begin();        // join i2c bus
	Wire.setClock(I2C_CLOCK);
}

void i2c_com::setup(StepperCtrl *ptrsCtrl){
	ptrStepperCtrl=ptrsCtrl;
}

void i2c_com::process(void){//call this in loop
	data_to_slave.data = geterr();
	communicate(data_to_slave);
}
void i2c_com::process(i2c_com_data data){
	communicate(data);
}


void i2c_com::communicate(i2c_com_data data){
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
  if ((millis()-t1)>CTRL_GET_INTERVAL)//delay, without delay()
  {
	uint8_t data_size = Wire.requestFrom(CTRL_I2C_ADDR, 2*sizeof(data_to_slave));
	t1=millis();
	if(data_size==2*sizeof(data_to_slave)){
		do{
		  I2C_readAnything(data_from_slave);
		}while(data_from_slave.to!=NZS_LABEL);//reading till the expected data is delivered
		if(data_from_slave.to==NZS_LABEL||data_from_slave.to==NZS_LABEL==0)//run if for this NZS or all NZS
		{
			run_cmd_from_slave();
		}
	}
  }
}

void i2c_com::run_cmd_from_slave(void){
	if(data_from_slave.action=='t')//test time tick transfer
	{
		Serial.println (data_from_slave.from);
		Serial.println (data_from_slave.to);
		Serial.println (data_from_slave.action);
		Serial.println (data_from_slave.data);
	}

}

class i2c_com i2c_com;
