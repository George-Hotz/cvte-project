#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include "json.h"

int recv_protocols_1(uint8_t *recv, uint8_t *proto, uint8_t *cmd_id, int n);
int recv_protocols_2(uint8_t *recv, uint8_t *buff, uint8_t *length, uint16_t *cmd_id, int n);

void send_protocols_1(uint8_t *send, uint8_t *payload, uint8_t length, uint8_t proto, uint8_t cmd_id, int fd);
void send_protocols_2(uint8_t *send, uint8_t *payload, uint8_t length, uint32_t cmd_id, uint8_t frame_id, uint8_t frame_cnt, int fd);

void parse_recv_buff(uint8_t *recv_buff, uint8_t *send_buff, cJSON *json, 
                    int cmd_id_version, int cmd_id_platform, int n, int fd);

#endif // PROTOCOLS_H


