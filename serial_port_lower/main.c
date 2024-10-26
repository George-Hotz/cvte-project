#include "common.h"
#include "serial_port.h"
#include "protocols.h"
#include "json.h"


int main() {
    int fd, n;
    uint8_t recv_buff[256];
    uint8_t send_buff[256];

    const char *filename = "config/config.json";
    char *json_string = load_json(filename);

    // 解析 JSON 字符串
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON\n");
        free(json_string);
        return 1;
    }

    int cmd_id_version = get_cmd_id_version(json);
    int cmd_id_platform = get_cmd_id_platform(json);
    
    if(cmd_id_version == -1 || cmd_id_platform == -1){
        return -1;
    }

    // 打开串口
    fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    setup_serial(fd);
    printf("setup serial port successful! \n\n");

    while (1) {
        // 读取数据
        n = read(fd, recv_buff, sizeof(recv_buff) - 1);
        if (n > 0) {
            // 以16进制格式输出
            printf("Recv in Hex : ");
            for (int i = 0; i < n; i++) {
                printf("%02X ", recv_buff[i]);
            }
            
            printf("\n");
            printf("n = %d\n", n);

            // 解析数据并回复
            parse_recv_buff(recv_buff, send_buff, json, cmd_id_version, cmd_id_platform, n, fd);
        }
    }

    // 释放 JSON 对象和字符串
    cJSON_Delete(json);
    free(json_string);
    close(fd);

    return 0;
}