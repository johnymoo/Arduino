#ifndef __AAS_H__
#define __AAS_H__

#define		WIDE		60  // 
#define		NEARBY	10	// 

#define		MAX			80  //
#define		MIN			10	//
typedef unsigned int DataType;
typedef struct 
{
	unsigned int position;     
	DataType data[WIDE];	    
}DATA;


void DataInit(DATA * d, DataType InitValue)	//?¨¢11¨¬?¨ºy?Y3?¨º??¡¥,???¨¢11¨¬??¨²¦Ì??a??3?¨º??¡¥?a???¡§?¦Ì¡ê?¨°?¡ã??a0 
{
	int i = 0;
 	d->position = 0;
 	for(i = 0 ; i < WIDE ; i++)
		d->data[i] = InitValue;
} 	

void UpdateWindowData(DATA * d, DataType NewData)  //?????¡¥¡ä¡ã?¨²¨¤???¦Ì?¨ºy?Y??DD?¨¹D? 
{
	int i = 0, pos = 0;
	double sum = 0.0 ,residue  = 0.0; 
	DataType Average = 0; 				//???¨´?¦Ì 
	
	NewData = NewData*60;//¡ê?¡À?¨¢?¡Á?¡ä¨®?¦Ì?¨¦?¨¹¨º?1000¡ê?3?¨°?60??o¨®¡ê?¡Á?¡ä¨®?¦Ì60000¡ê??a?¨´¡Á?¡ê??¨¦¨°????¨®???¨¨¡ê???¨¦¨´o¨®?¨²??¨ºy????¡ä¨°¨¤¡ä¦Ì??¨®2?
		
	if((NewData<(MAX*60)) && (NewData>(MIN*60)))  	   //¨ºy?Yo?¡¤¡§ 
	{
		d->data[d->position] =  NewData ;   
	}
	else								         //¨ºy?Y2?o?¡¤¡§¡ê?¨®|???¨´¨¬¡¥¦Ì?Nearby??¨ºy?Y¦Ì¡À?D 
	{
		sum = NewData;					   //NewData2?¨®????¨´ 
		
		pos = d->position ;			   //??¦Ì¡À?¡ã????¨¨?3?¨¤¡ä 
		for(i = 0 ; i < NEARBY-1 ; i++)      //?¨®???¨´?¦Ì 
		{
			pos	= (pos+1)%WIDE;         //¨¨¡¤¡À¡ê¨ºy¡Á¨¦??¡À¨º2?¨°?3? 
			
			sum = sum + d->data[pos];  	
			
		}
		Average = (DataType)(sum/(NEARBY*1.0));  //?¨®???¨´?¦Ì
	 
		d->data[d->position] = Average;                //¡Á?D?¨°???¨ºy?Y¨®????¨´?¦Ì¡ä¨²¨¬? 
		
		residue  = (NewData*1.0 - Average*1.0 )/(NEARBY-1.0);//¨º¡ê¨®¨¤¦Ì?¨ºy?Y¡ê?D¨¨¨°a??¨¬¡¥¦Ì???o¨®nearby??¨ºy?Y¦Ì¡À?D 
			
		pos = d->position ;			       			   //??¦Ì¡À?¡ã????¨¨?3?¨¤¡ä 	
		for(i = 0 ; i < NEARBY-1 ; i++) 	     //?¨´¨¬¡¥¨ºy?Y 
		{
			pos	= (pos + 1)%WIDE;  			         //¨¨¡¤¡À¡ê¨ºy¡Á¨¦??¡À¨º2?¨°?3? 
			
			if((d->data[pos]*1.0 + residue ) <= 0.0) //¨¨¡¤¡À¡ê¨ºy?Y2?¨°?3?	
			{
				d->data[pos] = 0 ;  	
			} 
			else
			{
				d->data[pos] = (DataType)(d->data[pos]*1.0 + residue ) ;   //¨º¦Ì??¨ºy?Y?¨´¨¬¡¥ 
			}
		}
	} 

	d->position = (d->position+1)%WIDE;  //????????o¨®¨°? 	
}


double GetDUSTReading(DATA * d)            //??¨¨??¨¢11¨¬??¨²¨ºy?Y¦Ì????¨´?¦Ì  ¦Ì£¤??¡êoug/M3
{
	int i = 0;
	double sum = 0.0;
	for(i = 0; i < WIDE ; i++)
	{
		sum += d->data[i];
	}
	
	return (sum*2.0/(WIDE*60.0));  //¦Ì£¤??¡êoug/m3
}

#endif
