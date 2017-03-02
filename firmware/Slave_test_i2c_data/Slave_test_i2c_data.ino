#include "Arduino.h"
#include <string.h>
#include <stdio.h>
#include <Wire.h>
#include "I2C_Anything.h"
#include <SoftwareSerial.h>
#include <SerialCommand.h>

#define CTRL_I2C_ADDR 12

SerialCommand SCmd;   // The demo SerialCommand object

struct __attribute__ ((packed)) ctrlcomdata {
  char from='C';
  char to=0;
  char action='e';
  int32_t data=0;
};

volatile ctrlcomdata data_from_Master;
volatile ctrlcomdata data_to_Master;
volatile bool data_ready=false;
volatile bool send_cmd=false;

// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 50;


//The setup function is called once at startup of the sketch
void setup()
{
  pinMode(12, OUTPUT);//enable 12V
  digitalWrite(12, LOW);//enable 12V

  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println ("Arduino connected");
  Wire.begin(CTRL_I2C_ADDR);        // join i2c bus
  Wire.onReceive(receiveEvent);
  Wire.onRequest (requestEvent);

  SCmd.addCommand("pwr",pwr);
  SCmd.addCommand("pwrOn",pwr_on);
  SCmd.addCommand("pwrOff",pwr_off);
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
  Serial.println("Ready"); 
}

void pwr_on(int arg_cnt, char **args)
{
  digitalWrite(12, LOW);//enable 12V
  Serial.println ("on");
}

void pwr_off(int arg_cnt, char **args){
  digitalWrite(12, HIGH);//disable 12V
  Serial.println ("off");
}

void pwr()    
{
  int aNumber;  
  char *arg;
  arg = SCmd.next(); 
  process_arg(arg);
}

void process_arg(char *a)
{
//  Serial.println(a);
////  int aNumber=atoi(*a);
//  if (strcmp(a,"on")) 
//  {
//    digitalWrite(12, LOW);//enable 12V
//    Serial.println ("on");
//  }else if (strcmp(a,"off"))
//  {
//    digitalWrite(12, HIGH);//disable 12V
//    Serial.println ("off");
//  }else{
//    Serial.println("What??"); 
//    Serial.println(a);
//  }
}

// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  Serial.println("What?"); 
}

void loop(){
  SCmd.readSerial();     //process serial commands


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
  if(!send_cmd){
    static uint32_t t=0;
    data_to_Master.data = millis();
    data_to_Master.action = 't';
    data_to_Master.to = 'X';
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));
    data_to_Master.to = 'Y';
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));//we don't know which master is calling. Sending data fo both.
  }else{
  send_cmd=false;
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));
    Wire.write ((uint8_t*) &data_to_Master, sizeof(ctrlcomdata));//we don't know which master is calling. Sending data fo both.   
  }
}
