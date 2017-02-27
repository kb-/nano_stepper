#ifndef __COMMANDS_H__
#define __COMMANDS_H__
#include <Arduino.h>
#include "stepper_controller.h"
#include "USB_nzs_datastream.h"
#include "I2C_com.h"

extern StepperCtrl stepperCtrl;
extern USB_stream USB_stream;
extern i2c_com i2c_com;

void commandsInit(void);
int commandsProcess(void);

#endif //__COMMANDS_H__
