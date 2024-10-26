#include "common.h"
#include "serial_port.h"


void setup_serial(int fd) {
    struct termios tty;

    // 获取当前串口设置
    if (tcgetattr(fd, &tty) != 0) {
        perror("Failed to get serial attributes");
        exit(EXIT_FAILURE);
    }

    // 设置波特率
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // 设置其他参数
    tty.c_cflag |= (CLOCAL | CREAD); // 启用接收器，忽略调制解调器控制线
    tty.c_cflag &= ~PARENB; // 无校验位
    tty.c_cflag &= ~CSTOPB; // 1 个停止位
    tty.c_cflag &= ~CSIZE; // 清除字符大小位
    tty.c_cflag |= CS8; // 8 个数据位

    // 应用设置
    tcsetattr(fd, TCSANOW, &tty);
}

