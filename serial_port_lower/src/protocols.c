#include "common.h"
#include "protocols.h"

int recv_protocols_1(uint8_t *recv, uint8_t *proto, uint8_t *cmd_id, int n){
    uint32_t length = 0, check_sum = 0;

    if(recv[0] == 0xAA && recv[1] == 0xBB){
        length = recv[2];
        for(int i=2; i<length-1; i++){
            check_sum += recv[i];
        }

        if((0x100 - check_sum) == recv[length - 1]){
            *proto  = recv[3];
            *cmd_id = recv[4];
            return 1;
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}

void send_protocols_1(uint8_t *send, uint8_t *payload, uint8_t length, uint8_t proto, uint8_t cmd_id, int fd){
    uint8_t idx = 0, check_sum = 0;
    uint8_t *start = send + 2;

    send[idx++] = 0xAA;
    send[idx++] = 0xBB;
    send[idx++] = length + 6;
    send[idx++] = proto;
    send[idx++] = cmd_id;
    for(int i=0; i<length; i++){
        send[idx++] = payload[i];
    }
    for(int i=2; i<idx; i++){     //check_sum
        check_sum += send[i];
    }
    send[idx++] = 0x100 - check_sum;
    write(fd, send, length + 6); 
}

int recv_protocols_2(uint8_t *recv, uint8_t *buff, uint8_t *length, uint16_t *cmd_id, int n){
    uint8_t check_sum = 0;
    static uint8_t idx = 0;
    uint16_t frame_id = 0, frame_cnt = 0;

    if(recv[0] == 0x27 && recv[1] == 0x01){    // PC --> TV
        *cmd_id = (recv[2] << 8) | recv[3];
        *length = recv[4];

        frame_id = recv[5 + *length];           // frame_id
        frame_cnt = recv[6 + *length];          // frame_cnt

        for(int i=5; i<5+*length; i++){
            buff[idx++] = recv[i];
        }
        for(int i=1; i<7+*length; i++){
            check_sum ^= recv[i];
        }

        if(recv[7 + *length] == check_sum && recv[8 + *length] == 0xFB){
            if(frame_id == frame_cnt){
                idx = 0;
                return 2;                     // finish
            }else{
                return 1;                     // not finish
            }
        }else{
            return -1;
        }
    }else{
        return -1;
    }
}

void send_protocols_2(uint8_t *send, uint8_t *payload, uint8_t length, uint32_t cmd_id, uint8_t frame_id, uint8_t frame_cnt, int fd){
    uint8_t idx = 0, check_sum = 0;
    uint8_t *start = send;

    send[idx++] = 0x27;
    send[idx++] = 0x02;               // TV --> PC
    send[idx++] = cmd_id >> 8;        // high 8 bits
    send[idx++] = cmd_id & 0xFF;      // low  8 bits
    send[idx++] = length;             // length
    for(int i=0; i<length; i++){
        send[idx++] = payload[i];
    }
    send[idx++] = frame_id;           //frame_id
    send[idx++] = frame_cnt;          //frame_cnt
    for(int i=1; i<idx; i++){
        check_sum ^= send[i];
    }
    send[idx++] = check_sum;
    send[idx++] = 0xFB;
    write(fd, send, length + 9); 
}


void parse_recv_buff(uint8_t *recv_buff, uint8_t *send_buff, cJSON *json, 
                     int cmd_id_version, int cmd_id_platform, int n, int fd){
    uint8_t  ret = 0;
    uint8_t  cmd_id_1 = 0, proto_1 = 0, length = 0, buffer[256]; 
    uint16_t cmd_id_2 = 0;
    
    switch (PROTOCOLS){
        case PROTOCOLS_1:{

            ret = recv_protocols_1(recv_buff, &proto_1, &cmd_id_1, n);
            if(ret == -1){
                printf("This is Not protocols 1 \n\n");
            }else{
                if(proto_1 == 3){ 
                    if(cmd_id_version != -1 && cmd_id_1 == cmd_id_version){
                        cmd_functions[0](json, buffer, send_buff, cmd_id_1, cmd_id_2, fd);
                    }
                    if(cmd_id_platform != -1 && cmd_id_1 == cmd_id_platform){
                        cmd_functions[1](json, buffer, send_buff, cmd_id_1, cmd_id_2, fd);
                    }
                }
            }
            break;
        }
        case PROTOCOLS_2:{
            ret = recv_protocols_2(recv_buff, buffer, &length, &cmd_id_2, n);
            if(ret == -1){
                printf("This is Not protocols 2 \n\n");
            }else if(ret == 1){    // not finish
                printf("protocols 2 is not finished\n");
            }else if(ret == 2){    // finish
                printf("protocols 2 is finished\n");

                if(cmd_id_version != -1 && cmd_id_2 == cmd_id_version){
                    cmd_functions[0](json, buffer, send_buff, cmd_id_1, cmd_id_2, fd);
                }
                if(cmd_id_platform != -1 && cmd_id_2 == cmd_id_platform){
                    cmd_functions[1](json, buffer, send_buff, cmd_id_1, cmd_id_2, fd);
                }
            }
            break;
        }
        default:
            break;
    }

    proto_1 = 0, length = 0, cmd_id_1 = 0, cmd_id_2 = 0;
    memset(buffer, 0, sizeof(buffer));
    memset(recv_buff, 0, sizeof(recv_buff));
    memset(send_buff, 0, sizeof(send_buff));
}