#include "common.h"
#include "json.h"

uint8_t hash_cmd_id[256];

char* load_json(const char *filename) {
    FILE *file = fopen(filename, "rb");  
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(length + 1);
    if (!buffer) {
        perror("Failed to allocate memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, 1, length, file);
    if (bytesRead < length) {
        if (ferror(file)) {
            perror("Error reading file");
        } else {
            fprintf(stderr, "Warning: Expected to read %ld bytes, but got %zu bytes\n", length, bytesRead);
        }
        free(buffer);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    buffer[bytesRead] = '\0';  // Null-terminate the string
    fclose(file);
    return buffer;
}


int get_cmd_id_version(cJSON *json){
    cJSON *function = cJSON_GetObjectItem(json, "function");
    cJSON *query_version = cJSON_GetObjectItem(function, "query_version");

    int ret = query_version->valueint;
    if (cJSON_IsNumber(query_version)) {
        hash_cmd_id[ret]++;
        if(hash_cmd_id[ret] == 1){
            return ret;
        }else{
            printf("[Error] : cmd_id conflict in config.json\n");
            return -1;
        }
    }else{
        return -1;
    }
}

int get_cmd_id_platform(cJSON *json){
    cJSON *function = cJSON_GetObjectItem(json, "function");
    cJSON *query_platform = cJSON_GetObjectItem(function, "query_platform");

    int ret = query_platform->valueint;
    if (cJSON_IsNumber(query_platform)) {
        hash_cmd_id[ret]++;
        if(hash_cmd_id[ret] == 1){
            return ret;
        }else{
            printf("[Error] : cmd_id conflict in config.json\n");
            return -1;
        }
    }else{
        return -1;
    }
}

void query_version(cJSON *json, uint8_t *buff, uint8_t *send_buff, uint8_t cmd_id_1, uint16_t cmd_id_2, int fd){
    printf("version: %s\n\n", VERSION);
    switch (PROTOCOLS){
        case PROTOCOLS_1:{
            send_protocols_1(send_buff, (uint8_t*)VERSION, (uint8_t)strlen(VERSION), 3, cmd_id_1+1, fd);
            break;
        }
        case PROTOCOLS_2:{
            send_protocols_2(send_buff, (uint8_t*)VERSION, (uint8_t)strlen(VERSION), cmd_id_2, 1, 1, fd);
            break;
        }
    }
    
}

void query_platform(cJSON *json, uint8_t *buff, uint8_t *send_buff, uint8_t cmd_id_1, uint16_t cmd_id_2, int fd){
    printf("platform: %s\n\n", PLATFORM);
    switch (PROTOCOLS){
        case PROTOCOLS_1:{
            send_protocols_1(send_buff, (uint8_t*)PLATFORM, (uint8_t)strlen(PLATFORM), 3, cmd_id_1+1, fd);
            break;
        }
        case PROTOCOLS_2:{
            send_protocols_2(send_buff, (uint8_t*)PLATFORM, (uint8_t)strlen(PLATFORM), cmd_id_2, 1, 1, fd);
            break;
        }
    }
}

cmd_func_t cmd_functions[2] = {  
    query_version, 
    query_platform 
}; 