/*
  MiCS-VZ-86 to read PPM
 Sample every 5 seconds
 
 This example code is in the public domain.
 
 modified 21 Jan 2015
 by Chris Wang
 */
#include <Wire.h>
#include <digitalWriteFast.h>
#include <PinChangeInt.h>
#include <AAS.h>

char delimiter[]=",";

// for SW-PWM-01A Dust sensor
#define DUST_PWM_PIN1  10
#define DUST_PWM_PIN2  11
unsigned long TimeOfsample = 0;
DATA DUST_Small_Pin1; 
DATA DUST_Small_Pin2;

// for Telaire 6713 CO2 sensor
#define ADDR_6713  0x15 // default I2C slave address
int ppm = 0;

// for SGX VOC VZ86 sensors
// the pin that will read in PWM signal
#define VOC_IN_PIN0 0
#define VOC_IN_PIN1 5
#define VOC_IN_PIN2 2
#define VOC_IN_PIN3 3
#define VOC_IN_PIN4 4

volatile unsigned long prevTimeShared[5]={0};
volatile unsigned long pwmValue1Shared[5]={0};
volatile unsigned long pwmValue2Shared[5]={0};
volatile boolean pwmChannelShared[5]={false}; 
int pinNumber[5] = {VOC_IN_PIN0, VOC_IN_PIN1, VOC_IN_PIN2, VOC_IN_PIN3, VOC_IN_PIN4};
unsigned long time=0;


// ------------ Function -------------------//

// send request to read current gas measurement in ppm
// return status: 0 success
byte queryCO2()
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
int readCO2PPM()
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

   return ((MSB<<8)|LSB);
}

void calcPin0PWM()
{
  int i = 0;
  if( digitalReadFast(VOC_IN_PIN0) == HIGH )
  {
    prevTimeShared[i] = micros();
  }
  else
  {
    if( !pwmChannelShared[i] )
    {
      pwmValue1Shared[i] = (micros() - prevTimeShared[i]);
    }
    else{
      pwmValue2Shared[i] = (micros() - prevTimeShared[i]);
    }
    pwmChannelShared[i] = !pwmChannelShared[i];
  }
}

void calcPin1PWM()
{
  int i = 1;
  if( digitalReadFast(VOC_IN_PIN1) == HIGH )
  {
    prevTimeShared[i] = micros();
  }
  else
  {
    if( !pwmChannelShared[i] )
    {
      pwmValue1Shared[i] = (micros() - prevTimeShared[i]);
    }
    else{
      pwmValue2Shared[i] = (micros() - prevTimeShared[i]);
    }
    pwmChannelShared[i] = !pwmChannelShared[i];
  }
}

void calcPin2PWM()
{
  int i = 2;
  if( digitalReadFast(VOC_IN_PIN2) == HIGH )
  {
    prevTimeShared[i] = micros();
  }
  else
  {
    if( !pwmChannelShared[i] )
    {
      pwmValue1Shared[i] = (micros() - prevTimeShared[i]);
    }
    else{
      pwmValue2Shared[i] = (micros() - prevTimeShared[i]);
    }
    pwmChannelShared[i] = !pwmChannelShared[i];
  }
}

void calcPin3PWM()
{
  int i = 3;
  if( digitalReadFast(VOC_IN_PIN3) == HIGH )
  {
    prevTimeShared[i] = micros();
  }
  else
  {
    if( !pwmChannelShared[i] )
    {
      pwmValue1Shared[i] = (micros() - prevTimeShared[i]);
    }
    else{
      pwmValue2Shared[i] = (micros() - prevTimeShared[i]);
    }
    pwmChannelShared[i] = !pwmChannelShared[i];
  }
}

void calcPin4PWM()
{
  int i = 4;
  if( digitalReadFast(VOC_IN_PIN4) == HIGH )
  {
    prevTimeShared[i] = micros();
  }
  else
  {
    if( !pwmChannelShared[i] )
    {
      pwmValue1Shared[i] = (micros() - prevTimeShared[i]);
    }
    else{
      pwmValue2Shared[i] = (micros() - prevTimeShared[i]);
    }
    pwmChannelShared[i] = !pwmChannelShared[i];
  }
}

unsigned int GetDUST_SmallPin1(void)  //??μ?D?·?3?(1-2um)?¨?è
{  
  return (unsigned int)(GetDUSTReading(&DUST_Small_Pin1));
}
unsigned int GetDUST_SmallPin2(void)  //??μ?D?·?3?(1-2um)?¨?è
{  
  return (unsigned int)(GetDUSTReading(&DUST_Small_Pin2));
}

// the setup function runs once when you press reset or power the board
void setup() {
  int i;
  //initialize
  Serial.begin(9600);
  Wire.begin();
  DataInit(&DUST_Small_Pin1 ,0); 
  DataInit(&DUST_Small_Pin2 ,0);
  
  pinMode(DUST_PWM_PIN1, INPUT);
  pinMode(DUST_PWM_PIN2, INPUT);
  
  // print header
  //Serial.print("Time");
  for( i=0; i<5; i++)
  {
    pinMode(pinNumber[i],INPUT);
    /*Serial.print(delimiter);
    Serial.print("VOC");
    Serial.print(i); 
    Serial.print(delimiter);
    Serial.print("CO2");
    Serial.print(i);*/
  }
  /*Serial.print(delimiter);
  Serial.print("AAS_CO2");
  Serial.print(delimiter);
  Serial.print("Dust1");
  Serial.print(delimiter);
  Serial.println("Dust2");*/
  
  //allow sensors to warm up
  delay(10000);    
  
  // attach interrupt for VOC
  PCintPort::attachInterrupt(VOC_IN_PIN0, calcPin0PWM,CHANGE);
  PCintPort::attachInterrupt(VOC_IN_PIN1, calcPin1PWM,CHANGE);
  PCintPort::attachInterrupt(VOC_IN_PIN2, calcPin2PWM,CHANGE);
  PCintPort::attachInterrupt(VOC_IN_PIN3, calcPin3PWM,CHANGE);
  PCintPort::attachInterrupt(VOC_IN_PIN4, calcPin4PWM,CHANGE);
}


// the loop function runs over and over again forever
void loop() {
  static unsigned long pwmValueCO2[5];
  static unsigned long pwmValueVOC[5];
  static int TimesOfInt = 0, P1TimesOfLowSignalPin1 = 0, P1TimesOfLowSignalPin2=0;
  int i;
  
  //sample PM2.5
  if( digitalReadFast(DUST_PWM_PIN1) == LOW )
  {
    P1TimesOfLowSignalPin1++;
  }
  if( digitalReadFast(DUST_PWM_PIN2) == LOW )
  {
    P1TimesOfLowSignalPin2++;
  }
  
  TimesOfInt++; 
  
  if(TimesOfInt%1000 == 0 )   // update PM2.5 window every second
  {     		
    TimeOfsample++;
					 
    UpdateWindowData(&DUST_Small_Pin1 , P1TimesOfLowSignalPin1 ) ;
    UpdateWindowData(&DUST_Small_Pin2 , P1TimesOfLowSignalPin2 ) ;	
    
    P1TimesOfLowSignalPin1 = 0;
    P1TimesOfLowSignalPin2 = 0;
  }
  
  // output VOC, CO2, PM2.5 reading every 5 seconds
  if( TimeOfsample == 5 )
  {
    TimeOfsample = 0;
    TimesOfInt = 0;
    time++;
    //read VOC
    noInterrupts();
    for( i=0; i<5; i++ )
    {
      pwmValueCO2[i] = (pwmValue1Shared[i] > pwmValue2Shared[i])?pwmValue1Shared[i]:pwmValue2Shared[i];
      pwmValueVOC[i] = (pwmValue1Shared[i] < pwmValue2Shared[i])?pwmValue1Shared[i]:pwmValue2Shared[i];
    }
    interrupts();

    //read CO2
    /*if( queryCO2() == 0 )
    {
      ppm = readCO2PPM();
    }*/
    ppm=800;
    //output all readings
    //Serial.print(time); // Time
    //VOC
    for( i=0; i<5; i++ )
    {
      Serial.print(pwmValueVOC[i]*3/1000.0);
      Serial.print(delimiter);
      Serial.print(pwmValueCO2[i]*3/1000.0);
      Serial.print(delimiter);
    }
    //Serial.print(delimiter);
    Serial.print(ppm); // CO2
    Serial.print(delimiter);
    Serial.print(GetDUST_SmallPin1()); //Dust 1
    Serial.print(delimiter);
    Serial.println(GetDUST_SmallPin2());//Dust 2
  }
  
  delay(1);
}


