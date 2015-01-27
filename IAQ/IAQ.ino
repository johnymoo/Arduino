/*
  MiCS-VZ-86 to read PPM
 Sample every 5 seconds
 
 This example code is in the public domain.
 
 modified 21 Jan 2015
 by Chris Wang
 */
#include <Wire.h>

#define ADDR_6713  0x15 // default I2C slave address

int ppm = 0;
#define PWM_PIN 3

volatile unsigned long pwm_value = 0;
volatile unsigned long pwm_value1 = 0;
volatile unsigned long pwm_value2 = 0;
volatile unsigned long prev_time = 0;
int pwm_co2 = 0;
int pwm_voc = 0;

boolean channel = false;
unsigned long tmp;

// send request to read current gas measurement in ppm
// return status: 0 success
byte queryPPM()
{
  int bytes;

  // start I2C
  Wire.beginTransmission(ADDR_6713);

  // Function code = 0x04
  Wire.write(0x04);
  // Starting address (MSB) = 0x13
  Wire.write(0x13);
  // Starting address (LSB) = 0x8B
  Wire.write(0x8B);
  // Input registers to read (MSB) = 0x00
  Wire.write(0x00);
  // Input registers to read (LSB) = 0x01
  Wire.write(0x01);

  // end transmission
  bytes = Wire.endTransmission();
  return bytes;
}

// read report of current gas measurement in ppm
int readPPM()
{
  byte func_code, byte_count, MSB, LSB;
  func_code = 0;
  byte_count = 0;
  MSB = 0;
  LSB = 0;
  Wire.requestFrom(ADDR_6713, 4);    // request 6 bytes from slave device 

   while(Wire.available() == 0);    
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
   
   // ppm = MSB*256 + LSB
   return ((MSB<<8)|LSB);
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(5000);
  attachInterrupt(1, rising, RISING);
}

void readVOC()
{
  // make sure pwm_value 1 > pwm_value2
  if( pwm_value1 < pwm_value2 )
  {
    tmp = pwm_value1;
    pwm_value1 = pwm_value2;
    pwm_value2 = tmp;
  }
  pwm_co2 = 40*pwm_value1-1800;
  pwm_voc = 25*pwm_value2-125;
    
  //Serial.print(pwm_value1);
  //Serial.print(" ");
  //Serial.println(pwm_value2);
  Serial.print("CO2: ");
  Serial.println(pwm_co2);
  Serial.print("VOC: ");
  Serial.println(pwm_voc);
}

// the loop function runs over and over again forever
void loop() {
  
  if( queryPPM() == 0 )
  {
    ppm = readPPM();
  
    Serial.print("6713 ppm: ");
    Serial.println(ppm);
     readVOC();
	// 6713 sample CO2 every 5 seconds
    delay(5000);
  }
}

void falling() {
  pwm_value = (millis() - prev_time)*3;
  if( !channel ){
    pwm_value1 = pwm_value;
  } 
  else {
    pwm_value2 = pwm_value;
  }
   channel = !channel; 
  attachInterrupt(1, rising, RISING);
}

void rising(){
  prev_time = millis();
  attachInterrupt(1, falling, FALLING);
}

