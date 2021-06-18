#include "mqtt.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


//input your message char array, client Id, user name, password
uint16_t mqtt_connect(char * buff, const char *id, const char *user, const char *pass)
{
 	//set header to CONNECT(0001 0000)
 	buff[0] = 0x10;
 	
 	uint16_t length = MQTT_MAX_HEADER_SIZE;
 	
    //set mqtt version
 	#if MQTT_VERSION == MQTT_VERSION_3_1
 	uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
 	#define MQTT_HEADER_VERSION_LENGTH 9
 	#elif MQTT_VERSION == MQTT_VERSION_3_1_1
 	uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
 	#define MQTT_HEADER_VERSION_LENGTH 7
 	#endif

 	for (int j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
 		buff[length++] = d[j];
 	}
 	
 	uint8_t v = 0x00;	//set "FLAG" (1 byte)
 	v = v|0x02;			//set "clean session", automatically clean the setting of client after client disconnecting
 	v = v|0x80;			//use the "user name"
 	v = v|(0x80>>1);	//use the "password"
 	buff[length++] = v;
 	
 	buff[length++] = ((MQTT_KEEPALIVE) >> 8);       //keep alive, char: 8bit ; uint16_t: 16bit. cut MQTT_KEEPALIVE to 2 parts
 	buff[length++] = ((MQTT_KEEPALIVE) & 0xFF);

 	length = mqtt_writeBuffer(buff, length, id, 1);  
 	length = mqtt_writeBuffer(buff, length, user, 1);
 	length = mqtt_writeBuffer(buff, length, pass, 1);
 	buff[MQTT_MAX_HEADER_SIZE-1] = (length-MQTT_MAX_HEADER_SIZE);	//RemainLength
 	
 	if(length > MQTT_MAX_PACKET_SIZE || length == 0)
 	{
 	return 0;
 	}
 	else{
 	return length;
 	}
}

//QOS 0
//input your message char array, topic, payload
uint16_t mqtt_publish(char * buff, const char* topic, const char* payload)
{
    buff[0] = 0x30;  //QOS 0
        
    // Leave room in the buffer for header and variable length field
    uint16_t length = MQTT_MAX_HEADER_SIZE;
        
    length = mqtt_writeBuffer(buff, length, topic, 1);
    length = mqtt_writeBuffer(buff, length, payload, 0);
    buff[MQTT_MAX_HEADER_SIZE-1] = (length-MQTT_MAX_HEADER_SIZE);

    if(length > MQTT_MAX_PACKET_SIZE || length == 0)
    {
        return 0;
    }
    else{
        return length;
    }
}

uint16_t mqtt_subscribe(char * buff, const char* topic, uint16_t MsgId)
{
    buff[0] = 0x82;
    
    uint16_t length = MQTT_MAX_HEADER_SIZE;

    buff[length++] = (MsgId >> 8);
    buff[length++] = (MsgId & 0xFF);
    length = mqtt_writeBuffer(buff, length, topic, 1);
    buff[length++] = 0x00;  //QOS 0
    buff[MQTT_MAX_HEADER_SIZE-1] = (length-MQTT_MAX_HEADER_SIZE);
    
    if(length > MQTT_MAX_PACKET_SIZE || length == 1)
    {
        return 0;
    }
    else{
        return length;
    }
}

uint16_t mqtt_writeBuffer(char * buff, uint16_t pos, const char* str, const uint8_t counter)
{
	uint8_t tmpPos = pos;
	uint8_t strlength = strnlen(str, SETTING_STR_MAX_LEN);
    if (strlength >= SETTING_STR_MAX_LEN || pos == 0) {
        return 0;
    }
    
	char tmpArray[SETTING_STR_MAX_LEN+1] = {0};
	if(counter){
        *(buff+tmpPos) = ((strlength) >> 8);
        tmpPos++;
        *(buff+tmpPos) = ((strlength) & 0xFF);
        tmpPos++;
    }
    
	sprintf(tmpArray, "%s", str);
	for (int i = 0;i<strlength;i++) {
		*(buff+tmpPos) = tmpArray[i];
        tmpPos++;
	}
	return tmpPos;
}