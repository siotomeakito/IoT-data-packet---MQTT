#ifndef MQTT_H_
#define MQTT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4

// MQTT_VERSION : Pick the version
//#define MQTT_VERSION MQTT_VERSION_3_1
#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif


// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 128
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 60
#endif

// Maximum size of fixed header and variable length size header
#define MQTT_MAX_HEADER_SIZE 2	//Beta, so we fix it  to 2.
                                //The value of the "Remaining Length" in header is according to the packet size (count the bytes of the content), and the maximum is 4 bytes.
                                //(Hex, 256 in 1 byte, so 4 bytes means the length of the content could be up to 256^4 bytes)
                                //Therefore the MAX HEADER SIZE should be setted 5. So we need a function to extend the header's length automatically.
                                //BTW the MQTT_MAX_PACKET_SIZE is 128 bytes, so I think setting to 2 is enough.

#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting

#define SETTING_STR_MAX_LEN 50

uint16_t mqtt_connect(char * buff, const char *id, const char *user, const char *pass);
uint16_t mqtt_publish(char * buff, const char* topic, const char* payload);
uint16_t mqtt_subscribe(char * buff, const char* topic, uint16_t MsgId);
uint16_t mqtt_writeBuffer(char * buff, uint16_t pos, const char* str, const uint8_t counter);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_H_ */
