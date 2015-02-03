import serial
import httplib
import urllib
import re
import time

displayList=[0,1,2,4,6,8,10,11]
def postData(data):
	values = re.split('[,]', data)
#print values
	content={}

	i=0
	j=1
	for value in values:
		if i in displayList:
			prefix='field'
			prefix += str(j)
			content[prefix]=value
			j=j+1
		i=i+1
	
	content['api_key']='[your_key]'
#print content
	params = urllib.urlencode(content)

	headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
	conn = httplib.HTTPConnection("api.thingspeak.com:80")
	conn.request("POST", "/update", params, headers)
	response = conn.getresponse()
#print response.status, response.reason
	data = response.read()
	conn.close()


ser = serial.Serial('/dev/ttyUSB0', 9600)
ser.open()

f=open('sensordata.csv','a') 

#ser.write("testing")
starttime = int(time.time())
try:
	while 1:
		 response = ser.readline()
		 #print response
		 strcurtime = time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+','
		 f.write(strcurtime+response)
		 f.flush()
		 curtime = time.time()
		 if (curtime-starttime)>16:
			postData(response)
			starttime = curtime
			

except KeyboardInterrupt:
	ser.close()
	f.close()
