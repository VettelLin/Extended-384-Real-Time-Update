#pragma once


// 定义所有跟仪器连接和控制相关的错误类别，方便所有的仪器错误说明的对外描述一致化

// 与串口通讯相关错误
#define ERR_COM_INIT_FAIL			1101 // 通讯端口初始化错误
#define ERR_COM_OPEN_FAIL			1102 // 通讯端口打开错误
#define ERR_COM_CLOSE_FAIL			1103 // 通讯端口关闭错误
#define ERR_COM_READ_FAIL			1104 // 通讯端口读取数据错误
#define ERR_COM_WRITE_FAIL			1105 // 通讯端口写入数据错误
#define ERR_CMD_FEEDBACK_FAIL		1106 // 命令反馈错误
#define ERR_CMD_RESPONSE_TIMEOUT	1107 // 命令响应超时，参数1保存执行指令
#define ERR_CMD_EXECUTE_TIMEOUT		1108 // 命令执行超时，参数1保存执行指令

#define ERR_HELLO_FAIL				1120 // 仪器握手失败
#define ERR_HELLO_TIMEOUTL			1121 // 仪器握手超时

#define ERR_READ_EEPROM_FAIL		1125 // 读取EEPROM参数错误
#define ERR_READ_EEPROM_TIMEOUT		1126 // 读取EEPROM参数错误
#define ERR_WRITE_EEPROM_FAIL		1127 // 写入EEPROM参数错误
#define ERR_WRITE_EEPROM_TIMEOUT	1128 // 写入EEPROM参数错误

#define ERR_MOTOR_RESET_FAIL		1130 // 电机复位失败
#define ERR_MOTOR_RESET_TIMEOUT		1131 // 电机复位超时
#define ERR_MOTOR_MOVE_FAIL			1132 // 电机移动失败
#define ERR_MOTOR_MOVE_TIMEOUT		1133 // 电机移动超时

#define ERR_BOX_IN_FAIL			1134 // 进仓失败
#define ERR_BOX_IN_TIMEOUT		1135 // 进仓超时
#define ERR_BOX_OUT_FAIL		1134 // 出仓失败
#define ERR_BOX_OUT_TIMEOUT		1135 // 出仓超时


#define ERR_LED_SET_FAIL			1140 // 设置LED灯失败 
#define ERR_LED_SET_TIMEOUT			1141 // 设置LED灯超时
#define ERR_GAIN_SET_FAIL			1142 // 设置电压增益失败 
#define ERR_GAIN_SET_TIMEOUT		1143 // 设置电压增益超时

#define ERR_TEMP_SET_FAIL			1144 // 设置通道温度失败
#define ERR_TEMP_SET_TIMEOUT		1145 // 设置通道温度超时
#define ERR_TEMP_READ_FAIL			1146 // 读取通道温度失败
#define ERR_TEMP_READ_TIMEOUT		1147 // 读取通道温度超时

#define ERR_LASER_SET_FAIL			1148 // 设置LED激光器失败
#define ERR_LASER_SET_TIMEOUT		1149 // 设置LED激光器超时

// PCR仪相关错误码
#define ERR_PCR_TEMP_SET_FAIL			3101 // 设置PCR仪温度失败
#define ERR_PCR_TEMP_SET_TIMEOUT		3102 // 设置PCR仪温度超时
#define ERR_PCR_TEMP_READ_FAIL			3103 // 读取PCR仪温度数据失败
#define ERR_PCR_TEMP_READ_TIMEOUT		3104 // 读取PCR仪温度数据超时
#define ERR_PCR_FLU_READ_FAIL			3103 // 读取PCR仪通道荧光数据失败
#define ERR_PCR_FLU_READ_TIMEOUT		3104 // 读取 PCR仪通道荧光数据超时
#define ERR_PCR_STATUS_READ_FAIL		3105 // 读取PCR仪状态失败
#define ERR_PCR_STATUS_READ_TIMEOUT		3106 // 读取 PCR仪状态超时

#define ERR_PCR_		3121 // 读取 PCR仪状态超时
