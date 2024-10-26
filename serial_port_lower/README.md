## 下位机
- 1、可在编译时传参选择对应的平台进行编译
- 2、通过json配置文件的方式，灵活配置指令与对应的功能
- 3、根据不同的平台灵活适配多个串口协议
- 4、配置文件指令自检功能(防止指令发送冲突)
- 5、通信协议1和通信协议2的收发

## 工程框架
- bin：二进制文件
- config：json配置文件
- lib：cJSON第三方库
- src：源文件

### 编译流程: 
```bash
1、mkdir build
2、cd build
3、cmake ..
4、make
```