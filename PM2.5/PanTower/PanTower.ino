#include <SoftwareSerial.h>

#define SET_PIN 12
#define SENSOR_BAUD_RATE 9600
#define DEBUG_BARUD_RATE 115200

#define _DBG_RXPIN_ 2
#define _DBG_TXPIN_ 3

#define SOFTSERIAL

#ifdef SOFTSERIAL
SoftwareSerial mySerial(_DBG_RXPIN_,_DBG_TXPIN_);
#define DebugSerial	Serial
#else
#define mySerial Serial
SoftwareSerial DebugSerial(_DBG_RXPIN_,_DBG_TXPIN_);
#endif

boolean findHeader()
{
    if( mySerial.read() == 0x42 && mySerial.read()==0x4d )
    {
        return true;
    }
    return false;
}

void setup() {
    mySerial.begin(SENSOR_BAUD_RATE);     // opens serial port, sets data rate to 9600 bps
    DebugSerial.begin(DEBUG_BARUD_RATE);
    pinMode(SET_PIN, OUTPUT);
    digitalWrite(SET_PIN, LOW); 
    delay(100);
    digitalWrite(SET_PIN, HIGH);  //enable sampling  
}

void loop() {
    static unsigned int data[15]={0};
    int i;

    while (!( mySerial.available() > 0 && findHeader() )) { };
    
    //DebugSerial.println(mySerial.available());
    // 1~12 are dust data
    for( i=0;i<15;i++ )
    {
        data[i] = mySerial.read()<<8|mySerial.read();        
    }
    for( i=0;i<15;i++)
    {
        DebugSerial.print(",");
        DebugSerial.print(data[i]);
    }
    DebugSerial.println();
    delay(1000);//match sampling rate
}


