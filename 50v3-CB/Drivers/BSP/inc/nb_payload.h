#ifndef __NB_PAYLOAD_H__
#define __NB_PAYLOAD_H__

#include "nbInit.h"
void pro_data_thingspeak(void);
void pro_data(void);
void mode_data(char *buff);
void downilnk_check_data(void);
void downilnk_ack_data(void);
void Thingseye_mqtt_send_current_fw_info(void);
void Thingseye_mqtt_request_fw_info(void);
void Thingseye_mqtt_send_fw_downloading(void);
void Thingseye_mqtt_send_updated(void);
void Thingseye_mqtt_send_message_error_1(void);
void Thingseye_mqtt_send_message_error_2(void);
void Thingseye_mqtt_send_message_error_3(void);
void Thingseye_mqtt_send_message_error_4(void);
void Thingseye_mqtt_send_message_error_5(void);
void Thingseye_mqtt_send_message_error_6(void);
void Thingseye_mqtt_send_message_error_7(void);
void Thingseye_mqtt_send_message_update(void);
void Uplink_serial_log(void);
#endif
