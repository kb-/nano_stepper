#include "Arduino.h"
#include <string.h>
#include <stdio.h>
#include <Wire.h>
#include "I2C_Anything.h"

#define CTRL_I2C_ADDR 12

int ByteReceived;

struct __attribute__ ((packed)) ctrlcomdata {
  char from='C';
  char to=0;
  char action='e';
  int32_t data;
};

volatile ctrlcomdata data_from_Master;
volatile ctrlcomdata data_to_Master;
volatile bool data_ready=false;

//The setup function is called once at startup of the sketch
void setup()
{
  pinMode(12, OUTPUT);//enable 12V
  digitalWrite(12, LOW);//enable 12V

	Serial.begin(115200);  // start serial for output
	while (!Serial) {
	; // wait for serial port to connect. Needed for native USB
	}
	Serial.println ("Arduino connected");
	Wire.begin(CTRL_I2C_ADDR);        // join i2c bus
	Wire.onReceive(receiveEvent);
  Wire.onRequest (requestEvent);
}

void loop(){
  //12V power ON/OFF
  if (Serial.available() > 0)
  {
    ByteReceived = Serial.read();   
    if(ByteReceived == '1') // Single Quote! This is a character.
    {
      digitalWrite(12, LOW);//enable 12V
      Serial.println ("on");
    }
    
    if(ByteReceived == '0')
    {
      digitalWrite(12, HIGH);//disable 12V
      Serial.println ("off");
    }
  }

  //Handle incoming data
  if(data_ready){
    Serial.println (data_from_Master.from);
    Serial.println (data_from_Master.to);
    Serial.println (data_from_Master.action);
    Serial.println (data_from_Master.data);
    data_ready = false;
  }
}

void receiveEvent(int howMany) {
  if(howMany==sizeof(ctrlcomdata)){//requestEvent is confused with data without this test
    I2C_readAnything(data_from_Master);
    data_ready = true;
  }
}

void requestEvent(){
  static uint32_t t=0;
    data_to_Master.data = millis();
    data_to_Master.action = 't';
    data_to_Master.to = 'X';
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));
    data_to_Master.to = 'Y';
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));//we don't know which master is calling. Sending data fo both.
}