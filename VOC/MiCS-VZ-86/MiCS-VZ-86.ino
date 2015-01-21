/*
  MiCS-VZ-86 to read PPM
 Sample every 5 seconds
 
 This example code is in the public domain.
 
 modified 21 Jan 2015
 by Chris Wang
 */

#define PWM_PIN 3

volatile unsigned long pwm_value = 0;
volatile unsigned long pwm_co2 = 0;
volatile unsigned long pwm_voc = 0;
volatile unsigned long prev_time = 0;

boolean channel = false;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(5000);
  attachInterrupt(1, rising, RISING);
}

// the loop function runs over and over again forever
void loop() {
  delay(1000);
  
  Serial.print("CO2: ");
  Serial.println((pwm_co2>pwm_voc?pwm_co2:pwm_voc));
  Serial.print("VOC: ");
  Serial.println((pwm_voc<pwm_co2?pwm_voc:pwm_co2));
}

void falling() {
  pwm_value = (millis() - prev_time)*3;
  if( !channel ){
    pwm_co2 = pwm_value;
  } 
  else {
    pwm_voc = pwm_value;
  }
   channel = !channel; 
  attachInterrupt(1, rising, RISING);
}

void rising(){
  prev_time = millis();
  attachInterrupt(1, falling, FALLING);
}

