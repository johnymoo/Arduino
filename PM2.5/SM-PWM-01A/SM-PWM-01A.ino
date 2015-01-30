/*
  Amphenol 6713 CO2 using I2C to read PPM
  Sample every 5 seconds
  
  This example code is in the public domain.

  modified 21 Jan 2015
  by Chris Wang
 */
#include <digitalWriteFast.h>
#include <AAS.h>

#define DUST_PWM_PIN1  10
#define DUST_PWM_PIN2  11
unsigned long TimeOfsample = 0;

DATA DUST_Small_Pin1; 					 //¶¨ÒåÒ»¸ö´°¿ÚÊý¾Ý½á¹¹Ìå
DATA DUST_Small_Pin2; 
// the setup function runs once when you press reset or power the board
void setup() {
  //Wire.begin();
  Serial.begin(115200);
  DataInit(&DUST_Small_Pin1 ,0);	   //³õÊ¼»¯Êý¾Ý 
  DataInit(&DUST_Small_Pin2 ,0);
  
  pinMode(DUST_PWM_PIN1, INPUT);
  pinMode(DUST_PWM_PIN2, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  static int TimesOfInt = 0, P1TimesOfLowSignalPin1 = 0, P1TimesOfLowSignalPin2=0;
  if( digitalRead(DUST_PWM_PIN1) == LOW )
  {
    P1TimesOfLowSignalPin1++;
  }
  if( digitalRead(DUST_PWM_PIN2) == LOW )
  {
    P1TimesOfLowSignalPin2++;
  }
  TimesOfInt++; 					 //ÓÃÀ´¼ÇÂ¼½øÈëÖÐ¶ÏµÄ´ÎÊý
  if(TimesOfInt >= 1000)   //ËµÃ÷1sÊ±¼äµ½ÁË
  {     		
    TimeOfsample++;
					 
    //UpdateWindowData(&DUST_Big ,   P2TimesOfLowSignal) ; //¶Ô»¬¶¯´°¿ÚÀïÃæµÄÊý¾Ý½øÐÐ¸üÐÂ
    UpdateWindowData(&DUST_Small_Pin1 , P1TimesOfLowSignalPin1 ) ; //¶Ô»¬¶¯´°¿ÚÀïÃæµÄÊý¾Ý½øÐÐ¸üÐÂ
    UpdateWindowData(&DUST_Small_Pin2 , P1TimesOfLowSignalPin2 ) ;	
    //P2TimesOfLowSignal = 0;
    //Serial.print(",");
    //Serial.print(P1TimesOfLowSignalPin1);
    Serial.print(",");
    Serial.print(GetDUST_SmallPin1());
    //Serial.print(",");	
    //Serial.print(P1TimesOfLowSignalPin2);
    Serial.print(",");
    Serial.println(GetDUST_SmallPin2());		
    TimesOfInt = 0;
    P1TimesOfLowSignalPin1 = 0;
    P1TimesOfLowSignalPin2 = 0;
  }
  
  delay(1);
  
}

unsigned int GetDUST_SmallPin1(void)  //»ñµÃÐ¡·Û³¾(1-2um)Å¨¶È
{  
  return (unsigned int)(GetDUSTReading(&DUST_Small_Pin1));
}
unsigned int GetDUST_SmallPin2(void)  //»ñµÃÐ¡·Û³¾(1-2um)Å¨¶È
{  
  return (unsigned int)(GetDUSTReading(&DUST_Small_Pin2));
}

void DataInit(DATA * d, DataType InitValue)	//½á¹¹ÌåÊý¾Ý³õÊ¼»¯,½«½á¹¹ÌåÄÚµÄÔªËØ³õÊ¼»¯ÎªÖ¸¶¨Öµ£¬Ò»°ãÎª0 
{
	int i = 0;
 	d->position = 0;
 	for(i = 0 ; i < WIDE ; i++)
		d->data[i] = InitValue;
} 	

void UpdateWindowData(DATA * d, DataType NewData)  //¶Ô»¬¶¯´°¿ÚÀïÃæµÄÊý¾Ý½øÐÐ¸üÐÂ 
{
	int i = 0, pos = 0;
	double sum = 0.0 ,residue  = 0.0; 
	DataType Average = 0; 				//Æ½¾ùÖµ 
	
	NewData = NewData*60;//£¬±äÁ¿×î´óÖµ¿ÉÄÜÊÇ1000£¬³ËÒÔ60Ö®ºó£¬×î´óÖµ60000£¬ÕâÑù×ö£¬¿ÉÒÔÔö¼Ó¾«¶È£¬¼õÉÙºóÆÚÕûÊýÔËËã´òÀ´µÄÎó²î
		
	if((NewData<(MAX*60)) && (NewData>(MIN*60)))  	   //Êý¾ÝºÏ·¨ 
	{
		d->data[d->position] =  NewData ;   
	}
	else								         //Êý¾Ý²»ºÏ·¨£¬Ó¦¸Ã¾ùÌ¯µ½Nearby¸öÊý¾Ýµ±ÖÐ 
	{
		sum = NewData;					   //NewData²ÎÓëÆ½¾ù 
		
		pos = d->position ;			   //½«µ±Ç°Î»ÖÃÈ¡³öÀ´ 
		for(i = 0 ; i < NEARBY-1 ; i++)      //ÇóÆ½¾ùÖµ 
		{
			pos	= (pos+1)%WIDE;         //È·±£Êý×éÏÂ±ê²»Òç³ö 
			
			sum = sum + d->data[pos];  	
			
		}
		Average = (DataType)(sum/(NEARBY*1.0));  //ÇóÆ½¾ùÖµ
	 
		d->data[d->position] = Average;                //×îÐÂÒ»¸öÊý¾ÝÓÃÆ½¾ùÖµ´úÌæ 
		
		residue  = (NewData*1.0 - Average*1.0 )/(NEARBY-1.0);//Ê£ÓàµÄÊý¾Ý£¬ÐèÒªÆ½Ì¯µ½Ö®ºónearby¸öÊý¾Ýµ±ÖÐ 
			
		pos = d->position ;			       			   //½«µ±Ç°Î»ÖÃÈ¡³öÀ´ 	
		for(i = 0 ; i < NEARBY-1 ; i++) 	     //¾ùÌ¯Êý¾Ý 
		{
			pos	= (pos + 1)%WIDE;  			         //È·±£Êý×éÏÂ±ê²»Òç³ö 
			
			if((d->data[pos]*1.0 + residue ) <= 0.0) //È·±£Êý¾Ý²»Òç³ö	
			{
				d->data[pos] = 0 ;  	
			} 
			else
			{
				d->data[pos] = (DataType)(d->data[pos]*1.0 + residue ) ;   //ÊµÏÖÊý¾Ý¾ùÌ¯ 
			}
		}
	} 

	d->position = (d->position+1)%WIDE;  //Î»ÖÃÖ¸ÕëºóÒÆ 	
}


double GetDUSTReading(DATA * d)            //»ñÈ¡½á¹¹ÌåÄÚÊý¾ÝµÄÆ½¾ùÖµ  µ¥Î»£ºug/M3
{
	int i = 0;
	double sum = 0.0;
	for(i = 0; i < WIDE ; i++)
	{
		sum += d->data[i];
	}
	
	return (sum*2.0/(WIDE*60.0));  //µ¥Î»£ºug/m3
}
