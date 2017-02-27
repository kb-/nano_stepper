#ifndef I2C_COM_H
#define I2C_COM_H
#include "Arduino.h"
#include "syslog.h"
#include "board.h"
#include "stepper_controller.h"

struct __attribute__ ((packed)) i2c_com_data {
	char id=NZS_LABEL;		//NZS axis (X,Y,E...)
	char action='e';//e: absolute max error stream, i: motor current setting (mA),h: hold current setting (mA),c: calibrate
	int32_t data;
};

class i2c_com
{
	private:
		StepperCtrl *ptrStepperCtrl;
		int32_t maxerr;
		int32_t geterr(void);
		void send(i2c_com_data);
		i2c_com_data data_to_slave;
		i2c_com_data data_from_slave;

	public:
    bool on=false;
		void setup(StepperCtrl *ptrStepperCtrl); //sets up the USB stream
		void begin(void);
		void process(void);
		void process(i2c_com_data);
};

#endif
