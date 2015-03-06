unsigned char ide_workaround = 0;

#include <SoftwareSerial.h>

#define SET_PIN 12
#define SENSOR_BAUD_RATE 9600
#define DEBUG_BARUD_RATE 115200

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define mySerial Serial3
#define DebugSerial Serial
#else
#define _DBG_RXPIN_ 2
#define _DBG_TXPIN_ 3
SoftwareSerial mySerial(_DBG_RXPIN_,_DBG_TXPIN_);
#define DebugSerial	Serial
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
    delay(1000);
    pinMode(SET_PIN, OUTPUT);
    digitalWrite(SET_PIN, LOW); 
    delay(100);
    digitalWrite(SET_PIN, HIGH);  //enable sampling  
}

void loop() {
    static unsigned int data[15]={0};
    int i;
    
    DebugSerial.println("reading..");
    while (!( mySerial.available() > 0 && findHeader() )) { };
    
    //DebugSerial.println(mySerial.available());
    for( i=0;i<15;i++ )
    {
        data[i] = mySerial.read()<<8|mySerial.read();        
    }
    // 1~12 are dust data
    for( i=1;i<13;i++)
    {
        DebugSerial.print(",");
        DebugSerial.print(data[i]);
    }
    DebugSerial.println();
    delay(1000);//match sampling rate
}


