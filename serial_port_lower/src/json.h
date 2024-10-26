#ifndef JSON_H
#define JSON_H

#include "protocols.h"

char* load_json(const char *filename);

int get_cmd_id_version(cJSON *json);
int get_cmd_id_platform(cJSON *json);

void query_version(cJSON *json, uint8_t *buff, uint8_t *send_buff, uint8_t cmd_id_1, uint16_t cmd_id_2, int fd);
void query_version(cJSON *json, uint8_t *buff, uint8_t *send_buff, uint8_t cmd_id_1, uint16_t cmd_id_2, int fd);


typedef void (*cmd_func_t)(cJSON *json, uint8_t *buff, uint8_t *send_buff, uint8_t cmd_id_1, uint16_t cmd_id_2, int fd);
extern cmd_func_t cmd_functions[];  

#endif // JSON_H


