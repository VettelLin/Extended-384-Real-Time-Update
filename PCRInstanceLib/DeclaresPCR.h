#pragma once

#include <math.h>

//****************************************************************************************************************
// 当前支持的仪器型号和版本
const CString PCR_MODEL_IAMP_PS96 = _T("PS96");
const CString PCR_MODEL_BW_PCR96 = _T("PCR 96"); // PCR_MODEL_BW_PCR96

const CString PCR_VESION_IAMPPS96_11 = _T("1100");
const CString PCR_CODE_IAMP_PS96 = _T("1001"); 
const CString PCR_CODE_QPCR_STANDARD = _T("1003"); // QPCR标准版
//const CString PCR_CODE_N96_STANDARD = _T("1004"); // N96标准版
const CString PCR_CODE_M16_XINYI = _T("1005"); // 新羿M16
const CString PCR_CODE_N16_STANDARD = _T("1006"); // nQ16
//const CString PCR_CODE_F8_STANDARD = _T("1007"); // F8新羿
 

// 当前数据文件的标识和版本
const WORD wFileMarkPCR = 0x6161;   // PCR实验数据文件标识
const WORD wPCRVersion100 =  0x0100;    // PCR实验数据文件版本1.00
const WORD wPCRVersion101 =  0x0101;    // PCR实验数据文件版本1.01，目标参数增加颜色保存
const WORD wPCRVersion102 =  0x0102;    // PCR实验数据文件版本1.02，浊度调整参数增加通道
const WORD wPCRVersion103 =  0x0103;    // PCR实验数据文件版本1.03，扩增分析参数增加DeltaRn默认阈值成员
const WORD wPCRVersion104 =  0x0104;    // PCR实验数据文件版本1.04，保存病人信息
const WORD wPCRVersion105 =  0x0105;    // PCR实验数据文件版本1.05，保存项目信息中的通道阴阳性判定阈值，保存样本在每个通道的判定结果
const WORD wPCRVersion106 =  0x0106;    // PCR实验数据文件版本1.06，保存硬件信息(PCR仪型号、产品序列号、PCR仪版本(类型)、硬件版本、嵌入式软件版本)到实验文件中
const WORD wPCRVersion107 =  0x0107;    // PCR实验数据文件版本1.07，修改温度数据的保存方式
const WORD wPCRVersion108 =  0x0108;    // PCR实验数据文件版本1.08，保存项目信息中的“所有通道（染料）名称”
const WORD wPCRVersion109 =  0x0109;    // PCR实验数据文件版本1.09，保存熔曲温度数据
const WORD wPCRVersion110 =  0x0110;    // PCR实验数据文件版本1.10，保存熔曲使用的串扰系数，坐标轴X/Y默认最大最小值
const WORD wPCRVersion111 =  0x0111;    // PCR实验数据文件版本1.11，保存用户名
const WORD wPCRVersion112 =  0x0112;    // PCR实验数据文件版本1.12，保存康研葆公用样本信息
const WORD wPCRVersion113 =  0x0113;    // PCR实验数据文件版本1.13，保存PS96的实验员和审核员
const WORD wPCRVersion114 =  0x0114;    // PCR实验数据文件版本1.14，保存仪器通道信息
const WORD wPCRVersion115 =  0x0115;    // PCR实验数据文件版本1.15，保存标准品浓度值输入时的小数位数
const WORD wPCRVersion116 =  0x0116;    // PCR实验数据文件版本1.16，增加实验日志
const WORD wPCRVersion117 =  0x0117;    // PCR实验数据文件版本1.17，将阈值上下限从通道移植到基因
const WORD wPCRVersion118 =  0x0118;    // PCR实验数据文件版本1.18，增加实验过程中仪器断开后重联状态，及断开次数
const WORD wPCRVersion119 =  0x0119;    // PCR实验数据文件版本1.19，增加实验过程中仪器断开后重联后使用零值填充因为中断而过去的循环数据

const WORD wFileMarkPCRItem = 0X6162;   // PCR项目文件标识
//const WORD wPCRItemVersion =  0X0100;    // PCR项目文件版本
//const WORD wPCRItemVersion =  0X0101;    // 保存归一化默认阈值和DeltaRn默认阈值
//const WORD wPCRItemVersion =  0X0102;    // 保存通道对应阴阳性判定阈值
const WORD wPCRItemVersion103 =  0X0103;    // 保存所有的通道（染料）名称
const WORD wPCRItemVersion104 =  0X0104;    // 增加基因扩增参数中阈值上下限


const CString PCR_DATA_FILE_SUFFIX = _T("qstd"); // 数据文件后缀
const CString PCR_ITEM_FILE_SUFFIX = _T("qprj"); // 项目文件后缀
const CString PCR_TEMPLATE_FILE_SUFFIX = _T("qsts"); // 模板文件后缀
const CString PCR_PLATE_FILE_SUFFIX = _T("qstp"); // 孔板文件后缀
const CString EXP_QSTS_ABSOLUTE = _T("ExpAbsolute.qsts"); // 新建实验默认绝对定量模板文件
const CString EXP_QSTS_MELTCURVE = _T("ExpMeltCurve.qsts"); // 新建实验默认熔解曲线模板文件

const CString PCR_DATA_FILE_SUFFIX_DOT = _T(".qstd"); // 数据文件后缀，带小数点
const CString PCR_TEMPLATE_FILE_SUFFIX_DOT = _T(".qsts"); // 模板文件后缀，带小数点

const WORD wFileReportTemplateMarkPCR = 0x9191;   // PCR实报告模板文件标识
const CString PCR_REPORT_TEMPLATE_FILE_SUFFIX = _T("qstr"); // 报告模板文件后缀，带小数点
const CString PCR_REPORT_TEMPLATE_FILE_SUFFIX_DOT = _T(".qstr"); // 报告模板文件后缀，带小数点

const UINT WM_EXPERIMENT_RUN = WM_USER + 501; // 实验开始
const UINT WM_EXPERIMENT_FINISH = WM_USER + 502; // 实验结束
const UINT WM_EXPERIMENT_REFRESH = WM_USER + 503; // 刷新实验监控界面
const UINT WN_EXPERIMENT_TEMP_CHANGED = WM_USER + 504; // 温度数据改变
const UINT WN_EXPERIMENT_FLU_CHANGED = WM_USER + 505; // 荧光数据改变

const UINT WN_EXPERIMENT_STEP_CHANGED = WM_USER + 506; // 实验程序步骤改变，包括程序段和循环数
const UINT WM_SEGMENT_OVER = WM_USER + 507; // 实验步骤循环已经结束，即将进入首尾处理过程（降温等待、释放热盖以及帕尔贴温控等）
const UINT WM_MELT_ONETEM_OVER = WM_USER + 508; //熔曲步骤下某个温度点已经执行完毕，用于向界面窗口实时报告当前已经执行完毕的温度点，主要用于断电保护信息记录

const UINT WN_EXPERIMENT_DISCONNECT = WM_USER + 509; // 设备连接断开，暂停剩余时间计算


//****************************************************************************************************************
// 下位机指令代码

const  int  SIZE_DEVICE_PARAM = 0xA00;

const int CMD_SEND_LEN5 = 5;
const int CMD_SEND_LEN6 = 6;
const int CMD_SEND_LEN7 = 7;
const int CMD_SEND_LEN9 = 9;
const int CMD_SEND_LEN10 = 10;
const int CMD_SEND_LEN11 = 11;
const int CMD_SEND_LEN14 = 14;
const int CMD_RECEIVE_LEN6 = 6; // 响应返回指令最短长度

const int RESOLUTION_TIME = 10; // 下位机时间数据分辨率
const int RESOLUTION_TEMP = 100; // 下位机温度数据分辨率
const float PCR_TEMP_RELEASE = 655.35f; // 发送最大值释放温度，降温

const  BYTE PCR_HELLO =	0x01; //握手

const  BYTE	PCR_HARDWARE_STATUS = 0x05; // 读取硬件状态数组
const  BYTE PCR_SET_TEMP	= 0x06; // 设置多路温度
const  BYTE PCR_MOTOR		= 0x07; // 电机类控制指令
const  BYTE PCR_LASER_CTRL	= 0x08; // 设置光路LED激光器关断
const  BYTE PCR_READ_TEMP	= 0x09; // 读取通道温度
const  BYTE PCR_LID_TEMP	= 0x0A; // 设置热盖温度 
const  BYTE PCR_STALED		= 0x0F; // 设置状态灯
const  BYTE PCR_READ_PCRTEMP = 0x10; // 读取PCR温度
const  BYTE PCR_RDEE		= 0x11;   // 读取EEPROM
const  BYTE PCR_WDEE		= 0x12;	// 写入EEPROM
const  BYTE PCR_READ_FLUDATA = 0x0C; // 读取扫描荧光数据
const  BYTE PCR_PMT_GAIN	= 0x0D;   // 输出PMT 控制电压AD5311

const  BYTE PCR_SETPWM     = 0x31; ///<设置3路帕尔帖PWM

const  BYTE PCR_XMOT_SCAN  = 0x15; ///<X电机扫描一行数据（//电机类辅助指令码）
const  BYTE PCR_XMOT_RESET = 0x13; ///<X电机复位（//电机类辅助指令码）
const  BYTE PCR_XMOT_FORWARD = 0x10; ///<X电机正传（//电机类辅助指令码）
const  BYTE PCR_XMOT_REVERSE = 0x11; ///<X电机反转（//电机类辅助指令码）
const  BYTE PCR_XMOT_MOVETO = 0x14; ///<X电机移动到（//电机类辅助指令码）

const  BYTE PCR_YMOT_RESET = 0x23; ///<Y电机复位（//电机类辅助指令码）
const  BYTE PCR_YMOT_FORWARD = 0x20; ///<Y电机正传（//电机类辅助指令码）
const  BYTE PCR_YMOT_REVERSE = 0x21; ///<Y电机反转（//电机类辅助指令码）
const  BYTE PCR_YMOT_MOVETO = 0x24; ///<Y电机移动到（//电机类辅助指令码）

const  BYTE PCR_TEMP_MODE = 0x25;   // 设置PCR控温模式
const  BYTE PCR_IN_BOX    = 0x26;   // 进仓指令
const  BYTE PCR_OUT_BOX   = 0x27;   // 出仓指令
const  BYTE PCR_DEVICE_STATUS = 0x28; // 读取状态标志位指令

const  BYTE PCR_READ_SIX_TEMP = 0x43; // 读取6个通道温度

const  BYTE PCR_ZMOT_RESET = 0x33; ///<Z压紧电机复位（//电机类辅助指令码）
const  BYTE PCR_ZMOT_FORWARD = 0x30; ///<Z压紧电机正传（//电机类辅助指令码）
const  BYTE PCR_ZMOT_REVERSE = 0x31; ///<Z压紧电机反转（//电机类辅助指令码）
const  BYTE PCR_ZMOT_MOVETO = 0x34; ///<Z压紧电机移动到（//电机类辅助指令码）

const  BYTE PCR_UMOT_RESET = 0x43; ///<U进出仓电机复位（//电机类辅助指令码）
const  BYTE PCR_UMOT_FORWARD = 0x40; ///<U进出仓电机正传（//电机类辅助指令码）
const  BYTE PCR_UMOT_REVERSE = 0x41; ///<U进出仓电机反转（//电机类辅助指令码）
const  BYTE PCR_UMOT_MOVETO = 0x44; ///<U进出仓电机移动到（//电机类辅助指令码）

const  BYTE PCR_LID_STATUS   = 0xE0;   ///<盖子状态

//****************************************************************************************************************
// 软件上位机需用相关参数

//PWM K/B值EEPROM地址
#define PWMK1 0X780
#define PWMB1 0X784
#define PWMK2 0X788
#define PWMB2 0X78C
#define PWMK3 0X790
#define PWMB3 0X794

//MPPC高中低增益EEPROM地址
#define ADDR_MPPCGAIN_HIGH	0X850
#define ADDR_MPPCGAIN_MIDDLE	0X852
#define ADDR_MPPCGAIN_LOW	0X854

//仪器默认的扫描时间
#define QPCR_SCANSEC	12.5
#define PS96_SCANSEC	11.5
#define N16_SCANSEC	2.6
#define XINYIM16_SCANSEC	2.6

#define QPCR_LIDHEATSEC	160
#define PS96_LIDHEATSEC	160
#define N16_LIDHEATSEC	120
#define XINYIM16_LIDHEATSEC	0

//开关，添加与20221221	
#define WEAK_HOTLID_ON_TEMPERATURE	//热盖到温使用弱到温管理（当前热盖温度与设定的热盖温度差值大于5 即认为是热盖到温）

typedef CArray<long, long> CLongArray;
typedef CArray<int, int> CIntArray;

//扫描数据类型
typedef enum ScanDataType{
	ESDT_AMP,ESDT_MELT,ESDT_NONE
}ESCANDATATYPE;

// 实验样本类型
enum eSampleType
{
	SAMPLE_TYPE_NONE = 0, // 空
	SAMPLE_TYPE_UNKNOWN,   // 未知品
	SAMPLE_TYPE_STANDARD,  // 标准品
	SAMPLE_TYPE_NTC,     // 无模板对照
	SAMPLE_TYPE_NEGATIVE,   //阴性对照
	SAMPLE_TYPE_POSITIVE,   //阳性对照
	SAMPLE_TYPE_POS_CRITICAL, // 临界阳性对照
	SAMPLE_TYPE_CALIBRATOR, // 校正样本
	SAMPLE_TYPE_BUFFER,     //  缓冲液
	SAMPLE_TYPE_IC, // 内部质控 Internal Control

	SAMPLE_TYPE_MULLTI_DIFFERENT	//选中多个样本时，样本类型显示为多种不同的样本，20220624，孙文郎
};

// 性别
enum ePatientSexuality
{
	SEXUALITY_NONE = 0,
	SEXUALITY_MALE, // 男性
	SEXUALITY_FEMALE, // 女性
};

// 年龄单位
enum ePatientAgeUnit
{
	AGE_UNIT_YEAR = 1, // 年
	AGE_UNIT_MONTH,
	AGE_UNIT_DAY,
};

// 处理数据的顺序，先计算通道串扰、滤波、浊度调整，同时保存
enum eDataType
{
	FLU_ORIGINAL = 0,   // 原始荧光值 
	FLU_CROSSTALK = 1,  //  串扰修正后数据
	FLU_FILTER_MEDIAN = 2,		// 中值滤波数据
	FLU_BASELINE_ADJUST = 3,  // 基线修正后数据，使用浊度调整参数
	FLU_TURBIDITY = 4,  // 浊度调整后数据
	FLU_FILTER = 5, // 均值滤波数据

	FLU_NORMALIZATE = 6,  // 归一化数据，扩增曲线
	FLU_DELTARN = 7,  // DeltaRn数据

	//LOG_FLU_FILTER = 8,		// 滤波数据的对数值
	LOG_FLU_TURBIDITY = 9,  // 浊度调整后数据的对数值，如果无效，则等同于滤波数据的对数值

	LOG_FLU_NORMALIZATE = 10,  // 归一化数据的对数值
	LOG_FLU_DELTARN = 11,  // DeltaRn数据的对数值

	MELT_NORMALIZE = 12,	//归一化处理之后的熔曲数据
	MELT_FSTNEGRECIPROCAL = 13,	//一阶负导数处理之后的数据
	MELT_GAINDATA = 14,	//增益处理之后的熔曲数据

	//FLU_AFTER_CORRECT = 15,	//曲线矫正之后的数据

	//FLU_FLT_NOCP = 16,	//滤波方式为“尾端不补点”产生的滤波后数据
	//FLU_TUIB_NOCP = 17,	//滤波方式为“尾端不补点”产生的浊度校准后数据

	//FLU_NORM_NOCP = 18,	//滤波方式为“尾端不补点”产生的归一化处理后数据
	//FLU_RN_NOCP = 19	//滤波方式为“尾端不补点”产生的DeltaRn处理后数据
};

// 计算扩增曲线Ct的方法
enum eMethodCalculateCt
{
	METHOD_CT_USER = 0, // 根据用户项目定义使用
	METHOD_CT_SPLINE = 1, // 默认插值法
	METHOD_CT_SNDDEV = 2, // 最大二阶导数法
};

// 默认使用哪一种数据
enum eAnalysisDataType
{
	DEFAULT_DATA_NORMALIZE = 0, // 默认使用归一化数据
	DEFAULT_DATA_DELTARN, // 默认使用DeltaRn数据
};

// 插值法阈值类型
enum eSplineThresholdType
{
	SPLINE_THRESHOLD_AUTO = 0, // 自动阈值
	SPLINE_THRESHOLD_NORMALIZE, // 手动归一化阈值
	SPLINE_THRESHOLD_DELTARN, // 手动DeltaRn阈值
};

enum eAnalysisType
{
	ANALYSIS_QUALITATIVE = 0, // 定性分析
	NALYSIS_QUANTIFY, // 定量分析
};

// 染料目标分析参数
struct tagAnaAmplifyPara
{
	CString		strTarget; //目标信息
	CString		strTubeFlag; // 试管对应样本标记
	int			iChannelNo;	 // 通道
	CString		strDye; // 染料
	eAnalysisType nAnaType;	//分析类型
	BOOL		bAutoBaseline;	//是否选择自动基线
	int			iBaselineStart;	//基线起点
	int			iBaselineEnd;	//基线终点
	eMethodCalculateCt nMethodCt; // Ct计算方法，界面暂不处理，使用默认值，20220317
	eAnalysisDataType nDefaultDataType; // 默认使用数据类型
	eSplineThresholdType nThresholdType;	//是否选择自动阈值
	double	dCalcNormalizeThreshold; // 计算后归一化阈值
	double  dCalcDeltaRnThreshold;	// 计算后DeltaRn归一化自动阈值
	double	dNormalizeThreshold; // 手动归一化阈值
	double	dDeltaRnThreshold; // 手动DeltaRn阈值
	BOOL	bUseFilter;	//是否选择数字滤波，界面暂不处理，使用默认值，20220317
	double dLowerThreshold; // 阈值下限
	double dUpperThreshold; // 阈值上限
	tagAnaAmplifyPara()
	{
		strTarget.Empty();
		strTubeFlag.Empty();
		iChannelNo = 0;
		strDye.Empty();
		nAnaType = NALYSIS_QUANTIFY;
		bAutoBaseline = TRUE;
		iBaselineStart = 3;
		iBaselineEnd = 15;
		nMethodCt = METHOD_CT_SPLINE;
		nDefaultDataType = DEFAULT_DATA_DELTARN;
		nThresholdType = SPLINE_THRESHOLD_DELTARN;
		dCalcNormalizeThreshold = 0.1;
		dCalcDeltaRnThreshold = 1000;
		dNormalizeThreshold = 0.1;
		dDeltaRnThreshold = 0.1;
		bUseFilter = FALSE;
		dLowerThreshold = 7;
		dUpperThreshold = 50;
	}
};

// 熔解曲线参数
struct tagAnaMeltCurvePara
{
	CString		strTarget; //目标信息
	CString		strTubeFlag; // 试管对应样本标记
	int			iChannelNo;	 //通道
	CString		strDye; // 染料
	double		dMinTemp; // 最小温度
	double		dMaxTemp; // 最大温度
	double		dThreshold; // 峰高阈值
	tagAnaMeltCurvePara(){
		strTarget.Empty();
		strTubeFlag.Empty();
		iChannelNo = 0;
		strDye.Empty();
		dMinTemp = 60;
		dMaxTemp = 90;
		dThreshold = 10;
	}
};

// 项目定量检测范围
struct tagAnaInspectScope
{
	CString		strTarget; //目标信息
	int			nChannelNo;	 //通道
	CString		strDye; // 染料
	double		dLowerLimit; //检测下限
	double		dTopLimit;	//检测上限
	tagAnaInspectScope(){
		strTarget.Empty();
		nChannelNo = -1;
		strDye.Empty();
		dLowerLimit = 0.0; // 如果接近0，则表示没有设置
		dTopLimit = 0.0;
	}
};

// 基本判定规则类型
enum eAnaBasicRuleType
{
	RULE_TYPE_QC = 0, // 实验质控/质控品质控?
	RULE_TYPE_SAMPLE, // 样品质控（待测样本/重测样本）
	RULE_TYPE_RESULT, // 结果判定
};
// 项目数据类型
enum eAnaDataType
{
	DATA_TYPE_CT = 0,
	DATA_TYPE_RN,
	DATA_TYPE_CN,
};
// 项目比较符类型
enum eItemOperatorType
{
	OPERATOR_LESS_THAN = 0, // 小于
	OPERATOR_LESS_EQUAL, // 小于等于
	OPERATOR_MORE_THAN, // 大于
	OPERATOR_MORE_EQUAL, // 大于等于
	OPERATOR_EQUAL_TO, // 等于
	OPERATOR_UNEQUAL_TO, // 不等于
};

// 基本判定规则
struct tagAnaBasicRule
{
	eAnaBasicRuleType	nRuleType; // 判定规则类型
	eSampleType			nSampleType; // 样本类型
	CString			strTarget; // 指标
	int				nChannelNo;	 //通道
	CString			strDye; // 染料
	eAnaDataType	nDataType; // 数据类型（Ct/Rn/Cn）
	double			dValueFront; // 前置数值
	eItemOperatorType nOperFront; // 前置数值比较符，只有小于和小于等于，界面显示和计算正好相反
	double			dValueBack; // 后置数值
	eItemOperatorType nOperBack; // 后置数值比较符
	CString			strConclusion; // 结论
	COLORREF		clrResult; // 结论颜色

	BOOL		bUseHighRule; // 是否使用高级规则
	CString		strRuleFile; // 保存高级规则的文件
	CString		strHighRule; // 高级规则的解释

	tagAnaBasicRule(){
		nRuleType = RULE_TYPE_RESULT;
		nSampleType = SAMPLE_TYPE_UNKNOWN;
		strTarget.Empty();
		nChannelNo = -1;
		strDye.Empty();
		nDataType = DATA_TYPE_CT;
		dValueFront = 0.0;
		nOperFront = OPERATOR_LESS_THAN;
		dValueBack = 0.0;
		nOperBack = OPERATOR_LESS_THAN;
		strConclusion.Empty();
		clrResult = RGB(0,0,0);

		bUseHighRule = FALSE;
		strRuleFile.Empty();
		strHighRule.Empty();
	}
};

//****************************************************************************************************************
// 硬件下位机PCR仪需用相关配置参数

// 表明PCR仪的温度模式
enum ePCRTempMode
{
	TEMP_MODE_UPDOWN = 0, // 升降温
	TEMP_MODE_CONSTANT, // 恒温PCR仪
};

// 温度控制方式
enum eTempCtrlMode
{
	TEMP_CTRL_NONE = 0, // 不控温
	TEMP_CTRL_TUBE = 1, // 管控温
	TEMP_CTRL_BLOCK = 2, // 模块控温
};

///温度控制类型
enum eTempCtrlType
{
	TEMP_CTRL_NORMAL = 0,    // 正常PCR控温类型
	TEMP_CTRL_MELT   = 1,    // 溶解曲线控温类型
	TEMP_CTRL_GRADIENT = 2,  // 梯度PCR控温类型
	TEMP_CTRL_OTHER = 3      // 其它调试测试类型
};

///温度各通道说明
enum eTempChannelType
{
	TEMP_CHANNEL_ENVIRONMENT = 0,// 环境温度
	TEMP_CHANNEL_POWER = 1, // 电源温度
	TEMP_CHANNEL_SCAN = 3, // 扫描温度
	TEMP_CHANNEL_LID = 4, // 热盖
	TEMP_CHANNEL_HEATSINK = 5,// 热沉散热器 
	TEMP_CHANNEL_PET1 = 6, // 帕尔贴3个通道
	TEMP_CHANNEL_PET2 = 7,
	TEMP_CHANNEL_PET3 = 8
};

// MPPC模块控制电压档位
enum eMPPCVoltageGear
{
	MPPC_VOLGEAR_HIGH = 0,   // 高档位
	MPPC_VOLGEAR_MIDDLE = 1,   // 中间档位
	MPPC_VOLGEAR_LOW = 2,   // 低档位
};

// 状态灯闪烁模式
enum eStateLEDWorkMode
{
	STALED_RED_ERROR = 0, // 错误指示灯，有错误时红灯亮 
	STALED_GREEN_IDLE  = 1, // 系统空闲时绿灯常亮
	STALED_GREEN_BUSY  = 2, // 系统忙时绿灯均速闪烁
	STALED_GREEN_GETIN_GETOUT = 3, // 进仓、出仓时绿灯低频闪烁
	STALED_GREEN_BTNDOWN = 4,  // 有效键按下时绿灯低频闪三下
	STALED_YELLOW_PCR_BTNDOWN = 5  // 系统PCR过程中，有键按下时黄灯亮，用户确认后取消黄灯状态并修改系统工作状态
};

// 进出仓状态
enum eSysBoxStatus{
	BOXSTATE_IN = 0, // 进仓状态
	BOXSTATE_OUT = 1,  // 出仓状态
	BOXSTATE_OTHER = 2,  // 其他明确位置
	BOXSTATE_UNKNOW = 3  // 未初始化，未知位置
};

// 通道扫描模式
enum eChannelScanlMode
{
	CHANNEL_SCAN_ALL = 0, // 全通道扫描
	CHANNEL_SCAN_SEL, // 按照选择通道扫描
};

// 熔曲扫描模式
enum eMeltScanMode
{
	MELT_SCAN_FAST = 0,   ///< 快速模式
	MELT_SCAN_STANDARD = 1,  ///< 标准模式
};

// 扫描开始位置
enum eScanStartPos
{
	SCAN_START_LT = 0, // 扫描从孔板的左上角开始
	SCAN_START_RT, // 右上开始扫描
};

// 实验各阶段类型
enum eExpStageType
{
	STAGE_LID_HEATER = 0, // 热盖加热阶段，可以不在程序段中
	STAGE_AMPLIFY = 1, // 扩增段
	STAGE_MELT = 2, // 熔解段
	STAGE_HOLD = 3, // 保持阶段
};


// 实验步骤检测方式
enum eStepCheckType
{
	CHECK_NONE = 1, // 不检测
	CHECK_END_POINT = 2, // 末点检测，对应扩增程序段
	CHECK_ALL_POINT = 3, // 全点检测。对应熔解程序段
};

// 实验类型/分析类型/项目类型
enum eExperimentType
{
	EXP_TYPE_CUSTOM = 0, // 通用类型
	EXP_TYPE_ABSOLUTE, // 绝对定量
	EXP_TYPE_MELT, // 熔解曲线
	EXP_TYPE_RELATIVE, // 相对定量
	EXP_TYPE_ALLELE, // 等位基因
};

// 反应管类型
enum eTubeType
{
	TUBE_TRANSPARENT= 0, // 透明管
	TUBE_WHITE, // 白管
};

// 滤色片信息
struct tagScanChannelInfo
{
	int iType; // 光路类型, 0：不可用， 1：基于MPPC检测的PCR荧光光路 2： 基于PMT检测的PCR荧光光路 3： 基于APD检测的PCR荧光光路 4:校准光路
	int iExPosition; // 激发光位置
	int iExWaveLen; // 激发光中心波长
	int iExRange; // 激发光波长有效范围
	int iEmPosition; // 发射光位置
	int iEmWaveLen; // 发射光中心波长
	int iEmRange; // 发射光波长有效范围
};

// 染料目标通道信息
struct tagDyeChannelInfo
{
	int		iChannel;
	CString strTarget;
	CString strDye;
	CString	strQuencher;
	COLORREF clrTarget;
	BOOL	bDefault;
};

// 使用孔板类型
enum ePlateModeType
{
	PLATE_TYPE_PORE = 0, // 通用孔板
	PLATE_TYPE_BLOCK, // 模块
};

// 孔板排列方向
enum ePlateSortMode
{
	PLATE_SORT_HORZ = 0, // 横向
	PLATE_SORT_VERT, // 纵向
};

// 校准算法类型
enum eCalibrateMethod
{
	CALIBRATE_NONE = 0,
	CALIBRATE_TURBIDITY, // 浊度校准
};

struct tagPlateModeInfo
{
	ePlateModeType	nPlateType; // 孔板类型，是孔板还是模块
	int				iPlateCount; // 模块数
	ePlateSortMode	nSortMode; // 模块横向还是纵向排列
	int				iHoriCount; // 横向孔数
	int				iVertCount; // 纵向孔数
	tagPlateModeInfo()
	{
		nPlateType = PLATE_TYPE_PORE;
		iPlateCount = 1;
		nSortMode = PLATE_SORT_HORZ;
		//lzh
		//iHoriCount = 12;
		//iVertCount = 8;
		iHoriCount = 6;
		iVertCount = 3;
	}
};


// 设备硬件状态码定义
struct tagHardwareStatus
{
	BYTE nDevRun;          ///< 系统状态+错误号	1

	BYTE nXMotState;       ///< X轴初始化状态+工作状态	1
	WORD wXMotPos;		   ///<	X轴当前位置	2
	BYTE nXMotDirection;   ///< X轴当前运行方向	1

	BYTE nYMotState;	   ///< Y轴初始化状态+工作状态	1
	WORD wYMotPos;		   ///< Y轴当前位置	2
	BYTE nYMotDirection;   ///< Y轴当前运行方向	1

	BYTE nZMotState;			///< Z轴初始化状态+工作状态	1
	WORD wZMotPos;				///< Z轴当前位置	2
	BYTE nZMotDirection;		///< Z轴当前运行方向	1

	BYTE nUMotState;			///< U轴进出仓初始化状态+工作状态	1
	WORD wUMotPos;				///< U轴进出仓当前位置	2
	BYTE nUMotDirection;		///< U轴进出仓当前运行方向	1

	BYTE nMotSensor;			///< 光电开关电源状态 
	///< X轴光电开关状态	1
	///< Y轴光电开关状态	
	///< Z轴光电开关状态	
	///< U轴光电开关状态	

	BYTE nLED;					///< 光源FAM1状态	1
	///< 光源CY5状态	
	///< 光源VIC状态	
	///< 光源FAM4状态	
	///< 光源ROX状态	
	///< 光源CY3状态		
	BYTE nPmtAnalogState;		///< PMT模拟电路状态	1
	BYTE nEeprom;				///< EEPROM读写状态	1
	BYTE nSMBUart;				///< SMB-UART状态	1
	WORD wPMT7512Out;			///< PMT当前增益	2
	WORD wSMBBackground;		///< SMB电路背景噪声	2
	BYTE nSysBox;				///< 系统进仓出仓状态	1
	WORD wScannertTemp;				///< 扫描头当前温度	2
	BYTE nSMBParameterInit;				///< SMB参数传递状态	1
	BYTE nWKBParaInit;			///< WKB参数传递状态	1
	BYTE nSysStateLed;			///< 状态灯状态	1
	BYTE nCurtTempCtrlMode_TubeBlockMode;	///< 当前温控模式+ tube block加热模式	1	
	BYTE nPETUnit1State;		///< 帕尔贴1当前状态	1
	WORD wPETUnit1Set;			///< 帕尔贴1当前设置	2
	WORD wPETUnit1tTemp;		///< 帕尔贴1当前温度	2
	BYTE nPETUnit2State;		///< 帕尔贴2当前状态	1
	WORD wPETUnit2Set;			///< 帕尔贴2当前设置	2
	WORD wPETUnit2tTemp;		///< 帕尔贴2当前温度	2
	BYTE nPETUnit3State;		///< 帕尔贴3当前状态	1
	WORD wPETUnit3Set;			///< 帕尔贴3当前设置	2
	WORD wPETUnit3tTemp;		///< 帕尔贴3当前温度	2
	BYTE nCapState;				///< 热盖当前状态	1
	WORD wCapSet;				///< 热盖当前设置	2
	WORD wCaptTemp;				///< 热盖当前温度	2
	BYTE nAuxerState;			///< 辅助加热当前状态	1
	WORD wAuxerSet;				///< 辅助加热当前设置	2
	WORD wFanTemp;			///< 散热器当前温度	2
	BYTE nFanState;			///< 散热器风扇当前状态	1
	WORD wFanSet;				///< 散热器风扇当前设置	2
	WORD wSysFanSet;			///< 系统风扇当前设置	2
	WORD wSysFantTemp;			///< 系统环境当前温度	2
	BYTE nA4940Rst;				///< 功率板工作状态	1
	WORD wWKBBackground;		///< WKB电路背景噪声	2
	BYTE nAuxerHState;          ///< 辅助加热H当前状态
	WORD wAuxerHSet;            ///< 辅助加热H当前设置
	BYTE nTransportlockState;    ///< 运输锁状态
	WORD wPET1PWMOut;           ///< 帕尔帖1PWM输出值
	WORD wPET2PWMOut;           ///< 帕尔帖2PWM输出值
	WORD wPET3PWMOut;           ///< 帕尔帖3PWM输出值
	WORD wReserve2;             ///< 预留2个Byte尚未用到   2
};

//设备硬件状态位定义
struct tagDeviceStatus
{
	BYTE cXMot;		// X轴状态，0-正常；1-错误；
	BYTE cYMot;		// Y轴状态，0-正常；1-错误；
	BYTE cZMot;		// Z轴状态，0-正常；1-错误；
	BYTE cUMot;		// U轴状态，0-正常；1-错误；
	BYTE cMotSense;	// 光电开关，0-正常；1-错误；
	BYTE cEeprom;	// EEPROM状态，0-正常；1-错误；
	BYTE cPMT;		// PMT状态，0-正常；1-错误；

	BYTE cPETUnit1;	// 帕尔贴1状态，0-正常；1-错误；
	BYTE cPETUnit2;	// 帕尔贴2状态，0-正常；1-错误；
	BYTE cPETUnit3;	// 帕尔贴3状态，0-正常；1-错误；
	BYTE cCap;		// 热盖状态，0-正常；1-错误；
	BYTE cFanState;	// 散热器风扇状态，0-正常；1-错误；
	BYTE cAuxerState; // 辅助加热状态，0-正常；1-错误	
	BYTE cA4940Rst;	// 功率板状态，0-正常；1-错误；
	BYTE cSysBox;	// 进出仓按钮状态，0-正常；1-有按下

	BYTE cLid; // 盖子状态 0：关闭 1：打开
};

// 设备参数集
#pragma pack(1)
struct tagDeviceParams //当前读取总范围（0x000 - 0x9FF）,共2560个字节
{
	// 仪器硬件信息页（0x000 - 0x1FF）
	char cDeviceModel[16]; // 设备型号
	char cDeviceSN[16];  // SN号
	char cDeviceVersion[16]; // 设备硬件版本号
	char cFirmware[16];  // 设备固件版本号
	char cReserved1[160];  // 设备固件版本号
	char cKeyFilter1[32]; // 第1光路信息，如T505,ET470/30x,ET520/19m
	char cKeyFilter2[32]; // 第2光路信息
	char cKeyFilter3[32];
	char cKeyFilter4[32];
	char cKeyFilter5[32];
	char cKeyFilter6[32];
	char cKeyFilter7[32];
	char cKeyFilter8[32];
	char cFilterConfig[8];
	char cReserved2[24];

	// 电机参数页（0x200 - 0x2FF）
	WORD wXMotorBackPulse;	// Xscan电机回退脉冲
	WORD wXMotorMaxFreq; // Xscan电机最大速度
	WORD wXMotorFreq0;   // Xscan电机起跳速度
	WORD wXMotorScanFreq;// Xscan电机扫描速度
	WORD wXMotorHomeFreq;   // Xscan电机复位速度
	WORD wXMotorAccPulse;  // Xscan电机加减速脉冲总个数
	WORD wXMotorMaxPls;  // Xscan电机最大行程
	WORD wXMotorScanPulse; // Xscan电机扫描行脉冲数
	WORD wXMotorOutDir;    // Xscan电机远离光电方向设置
	WORD wXMotorBackFreq;	// Xscan电机回退速度
	WORD wYMotorBackPulse;	// Y电机回退脉冲
	WORD wYMotorMaxFreq; // Y电机最大速度
	WORD wYMotorFreq0;   // Y电机起跳速度
	WORD wYMotorScanFreq;// Y电机扫描速度
	WORD wYMotorHomeFreq;   // Y电机复位速度
	WORD wYMotorAccPulse;  // Y电机加减速脉冲总个数
	WORD wYMotorMaxPls;  // Y电机最大行程
	WORD wYMotorScanPulse; // Y电机扫描行脉冲数
	WORD wYMotorOutDir;    // Y电机远离光电方向设置
	WORD wYMotorBackFreq;	// Y电机回退速度
	WORD wYMotorMoveToFreq;	// Y电机移动到速度
	char cReserved3[214];

	// 温控参数页（0x300 - 0x4FF）
	char cReserved4[512]; 

	// 温控辅助类参数页（0x500 - 0x5FF）
	char cReserved5[256]; 

	// 孔位参数页（0x600 - 0x6FF）
	char cReserved6[256]; 

	// 荧光信号硬件类参数页（0x700-0x8FF）
	WORD wPMTGainCh1; //通道1PMT增益
	char cReserved7[94]; 
	WORD wCrtCoef1To1; // 1通道串扰1通道，数值/1000使用
	WORD wCrtCoef2To1; // 2通道串扰1通道，数值/1000使用
	WORD wCrtCoef3To1; // 3通道串扰1通道，数值/1000使用
	WORD wCrtCoef4To1; // 4通道串扰1通道，数值/1000使用
	WORD wCrtCoef5To1; // 5通道串扰1通道，数值/1000使用
	WORD wCrtCoef6To1; // 6通道串扰1通道，数值/1000使用
	WORD wCrtCoef7To1; // 7通道串扰1通道，数值/1000使用
	WORD wCrtCoef8To1; // 8通道串扰1通道，数值/1000使用
	WORD wCrtCoef1To2; // 1通道串扰2通道，数值/1000使用
	WORD wCrtCoef2To2; // 2通道串扰2通道，数值/1000使用
	WORD wCrtCoef3To2; // 3通道串扰2通道，数值/1000使用
	WORD wCrtCoef4To2; // 4通道串扰2通道，数值/1000使用
	WORD wCrtCoef5To2; // 5通道串扰2通道，数值/1000使用
	WORD wCrtCoef6To2; // 6通道串扰2通道，数值/1000使用
	WORD wCrtCoef7To2; // 7通道串扰2通道，数值/1000使用
	WORD wCrtCoef8To2; // 8通道串扰2通道，数值/1000使用
	WORD wCrtCoef1To3; // 1通道串扰3通道，数值/1000使用
	WORD wCrtCoef2To3; // 2通道串扰3通道，数值/1000使用
	WORD wCrtCoef3To3; // 3通道串扰3通道，数值/1000使用
	WORD wCrtCoef4To3; // 4通道串扰3通道，数值/1000使用
	WORD wCrtCoef5To3; // 5通道串扰3通道，数值/1000使用
	WORD wCrtCoef6To3; // 6通道串扰3通道，数值/1000使用
	WORD wCrtCoef7To3; // 7通道串扰3通道，数值/1000使用
	WORD wCrtCoef8To3; // 8通道串扰3通道，数值/1000使用
	WORD wCrtCoef1To4; // 1通道串扰4通道，数值/1000使用
	WORD wCrtCoef2To4; // 2通道串扰4通道，数值/1000使用
	WORD wCrtCoef3To4; // 3通道串扰4通道，数值/1000使用
	WORD wCrtCoef4To4; // 4通道串扰4通道，数值/1000使用
	WORD wCrtCoef5To4; // 5通道串扰4通道，数值/1000使用
	WORD wCrtCoef6To4; // 6通道串扰4通道，数值/1000使用
	WORD wCrtCoef7To4; // 7通道串扰4通道，数值/1000使用
	WORD wCrtCoef8To4; // 8通道串扰4通道，数值/1000使用
	WORD wCrtCoef1To5; // 1通道串扰5通道，数值/1000使用
	WORD wCrtCoef2To5; // 2通道串扰5通道，数值/1000使用
	WORD wCrtCoef3To5; // 3通道串扰5通道，数值/1000使用
	WORD wCrtCoef4To5; // 4通道串扰5通道，数值/1000使用
	WORD wCrtCoef5To5; // 5通道串扰5通道，数值/1000使用
	WORD wCrtCoef6To5; // 6通道串扰5通道，数值/1000使用
	WORD wCrtCoef7To5; // 7通道串扰5通道，数值/1000使用
	WORD wCrtCoef8To5; // 8通道串扰5通道，数值/1000使用
	WORD wCrtCoef1To6; // 1通道串扰6通道，数值/1000使用
	WORD wCrtCoef2To6; // 2通道串扰6通道，数值/1000使用
	WORD wCrtCoef3To6; // 3通道串扰6通道，数值/1000使用
	WORD wCrtCoef4To6; // 4通道串扰6通道，数值/1000使用
	WORD wCrtCoef5To6; // 5通道串扰6通道，数值/1000使用
	WORD wCrtCoef6To6; // 6通道串扰6通道，数值/1000使用
	WORD wCrtCoef7To6; // 7通道串扰6通道，数值/1000使用
	WORD wCrtCoef8To6; // 8通道串扰6通道，数值/1000使用
	char cReserved8[64]; 

	// 未定义（0x0800-0x09FF）
	char cReserved9[512]; 
};

struct tagStructureParams
{
	BYTE cFrameLength; // 指令返回帧长 默认1，表示帧长用1个字节表示，2用2个字节
	BYTE cUseBox; // 默认0，无进出仓 1：有进出仓
	WORD wReserved1; // 保留
	WORD wReserved2; // 保留

	BYTE cTempMode; // 温控类型 0：变温 1：恒温
	BYTE cTempCtrl; // 控温模式 0：不控温 1：管控温 2：模块控温
	BYTE cTempModuleCount; // 控温模块数量 默认1
	BYTE cAutoCooling; // 是否支持自动降温 0：不支持 1：支持，恒温仪使用
	BYTE cReserved3; // 保留

	BYTE cPlateType; // 孔板类型 0：标准孔板 1：模块  2：自定义
	BYTE cPlateCount; // 孔板或模块数 默认1
	BYTE cPlateSortType; // 孔板或模块排列方式 0：横向 1：纵向
	BYTE cPlateRowCount; // 单一孔板或模块行数 默认8
	BYTE cPlateColCount; // 单一孔板或模块列数 默认12
	BYTE cReserved4; // 保留

	BYTE cScanMode; // 扫描方式 0：全通道扫描 1：按照选择通道扫描
	BYTE cScanOrigin; // 扫描模式 0：从左上开始U型扫描 1：从右上开始U型扫描
	BYTE cScanPathCount; // 扫描光路数 1/2，与电机移动扫描相关(扫描头光路行数)
	BYTE cReserved5; // 保留
	
	BYTE cSupportMelt; // 是否支持熔曲 0：不支持 1：支持（默认）
	BYTE cSupportMultiMelt; // 是否支持多色熔曲 0：不支持 1：支持（默认）
	BYTE cMeltPreDelay; // 熔曲开始前必须稳定时间（秒）默认60秒，包括前面步骤的等待时间
	BYTE cMeltBeginTemp; // 熔曲开始温度 默认60度
	BYTE cMeltBeginPreTemp; // 熔曲开始温度的前置降低量 默认3度
	BYTE cMeltMinIncrement; // 熔曲最小温度梯度（0.01度）默认50，表示0.5度
	BYTE cMeltStepDelay; // 熔曲梯度升温后稳定时间（0.1秒）默认60，表示6秒
	BYTE cReserved6; // 保留

	BYTE cTempChannelCount; // 温度数据通道数 通道数：1...N，包括热盖通道
	WORD  wTempChannelFlag; // 温度通道是否使用标志 1-16通道，位0/1标记
	BYTE cTempCtrlTop; // 控温最高温度 默认90
	BYTE cTempCtrlLower; // 控温最低温度 默认30
	BYTE cTempCtrlMaxTime; // 最长控温时间 默认60秒
	BYTE cTempLegalGap; // 控温允许偏差范围(0.1度) 默认50，表示5度
	BYTE cTempPeltierLower; // 热盖升温过程中帕尔贴保持温度 默认15度	
	BYTE cLidHeader; // 是否支持热盖 0：不支持 1：支持
	BYTE cLidTempTop; // 热盖最高温度 默认105
	BYTE cLidTempLower; // 热盖最低温度 默认30
	BYTE cLidTempMaxTime; // 热盖加热最长时间 默认15分钟，单位：分钟
	BYTE cLidTempLegalGap; // 热盖控温允许偏差范围(0.1度) 默认50，表示5度
	WORD wReserved7; // 保留

	WORD wScanOnceTime; // 整机扫描一遍所需时间（0.01秒）
	BYTE cScanChangeRowTime; // 电机换行所需时间（毫秒) 默认0，暂不考虑
	BYTE cUseCrosstailk; // 通道串扰系数使用方式 0：不使用 1：仪器配置 2：用户定义 3：仪器和用户定义
	BYTE cCalibratePara; // 校准算法参数，0：无 1：浊度调整
	BYTE cReserved8; // 保留
	WORD wReserved9; // 保留

	BYTE cUseFilter; // 是否使用仪器定义滤波参 0：不使用 1：使用
	BYTE cFilterAmplifyMethod; // 扩增曲线数据滤波方式 0，不滤波，定义1-255种滤波方法
	BYTE cFilterAmplifyShowMethod; // 显示扩增曲线使用滤波方式 0，不滤波，定义1-255种滤波方法
	BYTE cFilterMeltMethod; // 熔解曲线数据滤波方式 0，不滤波，定义1-255种滤波方法

	char cReserved10[454]; 
};

#pragma pack()

//****************************************************************************************************************
// 算法相关参数

#define FILTERSELECTNUM	5	//提供的最大滤波选项个数


// 归一化曲线显示调整比率
struct tagCurveNormalizeShowPara
{
	TCHAR strDyeName[256]; ///< 染料名称
	BOOL bCurveAdjust; // 是否进行曲线显示调整
	double dAdjustRatio; // 曲线显示调整比例

	tagCurveNormalizeShowPara()
	{
		bCurveAdjust = FALSE;
		dAdjustRatio = 1.0;
	}
};
typedef CArray <tagCurveNormalizeShowPara*,tagCurveNormalizeShowPara*> CCurveNormalizeShowParaArray;

// 滤波类型
enum eFilterType
{
	FILTER_NONE = 0, // 无滤波
	FILTER_MEDIAN = 1, // 中值滤波
	FILTER_FM_BN = 2, // 前M后N滤波
	FILTER_TRIANGLE_29 = 4,///< 29点三角滤波
	FILTER_GAUSS_31 = 5  ///< 31点伪高斯
};

// 滤波参数设置信息
struct tagFilterItem
{
	int iFilterUser; // 滤波参数使用者，分为0：不使用，1：扩增数据、2：熔解数据 3：扩增显示数据
	eFilterType nType; // 滤波类型
	int iTimes; // 滤波次数
	int iPara1; // 参数1，在中值滤波时表示窗口，在前M后N滤波是表示M
	int iPara2; // 参数2，在中值滤波时无效，在前M后N滤波是表示N 
};

//****************************************************************************************************************
// 软件界面需用相关参数

typedef enum PermissionType
{
	EPT_ORDINARY = 0, // 普通用户
	EPT_ADMIN,	//管理员
	EPT_REAGENT, // 试剂研发人员，科研人员
	EPT_DEVELOPER, // 仪器研发人员
	EPT_REPAIR, // 维修人员
	EPT_VERIFIER, // 审核者
	EPT_IVD	//IVD模式用户，比普通用户层级低，只能打开模板文件或者实验文件运行实验
}EPERMISSIONTYPE;

// 样品设置标题栏用户界面参数
struct tagExpSampleTitleUI
{
	UINT			nColCount; ///< 栏数
	CUIntArray		arrayColWidth; ///< 栏宽度
	CStringArray	arrayName; ///< 栏名称
};

// 实验线程运行状态
enum eExpRunStatus
{
	STATUS_STOP = 0, // 正常结束
	STATUS_RUNNING, // 运行
	STATUS_ASK_STOP, // 要求结束
	STATUS_STOP_ERROR, // 因错误结束
};

typedef enum AnalyseResult{
	EAR_POSITVE,EAR_NEGATIVE,EAR_SUSPEND,EAR_UNHANDLED
}EANALYSERESULT;

enum eInstanceStatus
{
	INSTANCE_READY = 0,
	INSTANCE_USING,
	INSTANCE_ERROR,
	INSTANCE_SELFCHECK_FAIL,
	INSTANCE_TEMP_HIGH, // 温度过高不能运行
	INSTANCE_TEMP_LOW, // 温度过低不能运行
	INSTANCE_FLUO_HIGH, // 荧光值过高不能运行
	INSTANCE_FLUO_LOW, // 荧光值过低不能运行
};

//****************************************************************************************************************
// 公用函数

#define SAFE_DELETE(pointer) {if(pointer != NULL){delete pointer;pointer=NULL;}}
#define SAFE_DELETE_MULTI(pointer) {if(NULL != pointer){delete [] pointer;pointer = NULL;}}

template<typename T>
void MyDeletePtrPcrCtrlLib(T** pData)
{
	if((NULL != pData) && (NULL != *pData))
	{
		delete *pData;
		*pData = NULL;
	}
}

template<typename T>
void MyDeletePtrPcrCtrlLibMulti(T** pData)
{
	if((NULL != pData) && (NULL != *pData))
	{
		delete [] *pData;
		*pData = NULL;
	}
}
