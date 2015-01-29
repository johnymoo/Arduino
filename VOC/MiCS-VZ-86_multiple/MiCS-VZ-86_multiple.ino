#include <PinChangeInt.h>


/*
  MiCS-VZ-86 to read PPM
 Sample every 5 seconds
 
 This example code is in the public domain.
 
 modified 21 Jan 2015
 by Chris Wang
 */

#include <digitalWriteFast.h>

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

// the setup function runs once when you press reset or power the board
void setup() {
  int i = 0;
  Serial.begin(115200);
  delay(5000);
  Serial.print("Time");
  for( ; i<5; i++)
  {
    pinMode(pinNumber[i],INPUT);
    Serial.print("\tVOC");
    Serial.print(i); 
    Serial.print("\tCO2");
    Serial.print(i);
  }
  Serial.println();
  
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
  int i;

  noInterrupts();
  for( i=0; i<5; i++ )
  {
    pwmValueCO2[i] = (pwmValue1Shared[i] > pwmValue2Shared[i])?pwmValue1Shared[i]:pwmValue2Shared[i];
    pwmValueVOC[i] = (pwmValue1Shared[i] < pwmValue2Shared[i])?pwmValue1Shared[i]:pwmValue2Shared[i];
  }
  interrupts();
  Serial.print(time);
  for( i=0; i<5; i++ )
  {
    Serial.print("\t");
    Serial.print(pwmValueVOC[i]*3/1000.0);
    Serial.print("\t");
    Serial.print(pwmValueCO2[i]*3/1000.0);
  }
  Serial.println();
  time++;
  delay(1000);
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

