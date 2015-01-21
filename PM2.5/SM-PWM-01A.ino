/*
  Amphenol 6713 CO2 using I2C to read PPM
  Sample every 5 seconds
  
  This example code is in the public domain.

  modified 21 Jan 2015
  by Chris Wang
 */
#include <Wire.h>

#define ADDR_6713  0x15 // default I2C slave address

int ppm = 0;


void queryPPM()
{
  int bytes;

  Wire.beginTransmission(ADDR_6713);

  Wire.write(0x04);

  bytes =Wire.write(0x13);

 bytes = Wire.write(0x8B);

  bytes =Wire.write(0x00);

  bytes =Wire.write(0x01);
  //Serial.println(bytes);
  bytes = Wire.endTransmission();
  //Serial.print("end status: ");
  //Serial.println(bytes);
}

 int readPPM()
{
  byte func_code, byte_count, MSB, LSB;
  func_code = 0;
  byte_count = 0;
  MSB = 0;
  LSB = 0;
  Wire.requestFrom(ADDR_6713, 4);    // request 6 bytes from slave device #2

   while(Wire.available() == 0);    // slave may send less than requested
   func_code = Wire.read();
   byte_count = Wire.read();
   MSB = Wire.read(); 
   LSB = Wire.read(); 
   /*Serial.print("Func code: ");
   Serial.println(func_code);
    Serial.print("byte count: ");
   Serial.println(byte_count);
    Serial.print("MSB: ");
   Serial.println(MSB);
    Serial.print("LSB: ");
   Serial.println(LSB);*/
   
   return ((MSB<<8)|LSB);
}

// the setup function runs once when you press reset or power the board
void setup() {
  Wire.begin();
  Serial.begin(115200);
  // initialize digital pin 13 as an output.
  //pinMode(LED, OUTPUT);
  //pinMode(PWM_PIN, INPUT);
  //attachInterrupt(1, falling, FALLING);
  
 // Wire.onReceive(readPPM); // register event
}

// the loop function runs over and over again forever
void loop() {
  /*digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  */
 // pwm_value = pulseIn(PWM_PIN, LOW);
  //Serial.println(pwm_value);
  queryPPM();
  //delayMicroseconds(50);
  ppm = readPPM();
  
    Serial.print("PPM: ");
    Serial.println(ppm);
  delay(5000);
}

void rising() {
    unsigned long cur_time = micros();
  attachInterrupt(1, falling, FALLING);
  pwm_value = cur_time - prev_time;
  Serial.print("rising @ ");
  Serial.println(cur_time);
  Serial.print("P1: ");
  Serial.println(pwm_value);
}

void falling(){
  unsigned long cur_time = micros();
  Serial.print("start falling.. ");
  Serial.println(cur_time);
  attachInterrupt(1, rising, RISING);
  prev_time = cur_time;
}
