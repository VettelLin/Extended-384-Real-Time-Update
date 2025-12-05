#pragma once

//字体类型
#define TEXTFONT_CHINESE _T("黑体")
#define TEXTFONT_ENGLISH _T("Arial")

#define CRVLYAXIS_LBDCLICKZOOMRATIO	1.2	//绘图控件左键双击缩放系数

#define TIEMRID 2
#define INSTRUSTATELAMPTWINKLE	3
#define TIMER_ALARMTWINKLE	6
//TIMER	7已被占用

#define MSGTEXTTAG_INT 10	//自检消息wParam

#define PI 3.1415926

#define SAMPLATE_WH	28

#define ONEDAYTIME_T	86400	//一天对应的time_t数值

#define FILEMANAGEDLG_UPSURPLUS_PX	1	//FileManageDlg顶部扩展像素数（盖住MiddleDlg底部）

#define MAX_FILEOPEN_NUM	10	//最大可打开的文件个数


#define DEFAULT_FLUMAX	5000	//荧光信号值默认最小值，曲线控件纵坐标使用

//用户管理
#define PASSWORD_LEN_MIN	6	//密码长度最小字符数


//仪器类型
//#define N96INSTRUMENT	//开关，当前是N96模式
#define PS96INSTRUMENT	//开关，当前是PS96模式

////新羿
//#define XINYI_THEME	//新羿配色方案

//康妍葆功能
//#define CAN_YOUNG

//欧德莱功能
// #define ODREI	

//不带logo版本
//#define	NOLOGO_VERSION
//#define NOLOGO_STARTUP_VERSION

#ifdef XINYI_THEME
	#define RIGHT_STRING _T("2022 新羿生物 All Rights Reserved")
#else
	#ifdef CAN_YOUNG
		#define RIGHT_STRING _T("2022 KYBioStem All Rights Reserved")
	#else 
		#ifdef ODREI
			#define RIGHT_STRING _T("2022 欧德莱 All Rights Reserved")
		#else
			#define RIGHT_STRING _T("2022 博维泰科 All Rights Reserved")
		#endif
	#endif 
#endif

//颜色
#ifdef CAN_YOUNG

#define CLR_SOFT_THEME	/*RGB(9,128,162)*/ RGB(2,45,81)
#define CLR_SOFT_LIGHT	/*RGB(0XA1,0XAF,0XC9)*/ RGB(227,233,238)
#define CLR_2LEVEL_THEME	RGB(70,103,131)
#define CLR_INSERTBT_TX_DISABLED	RGB(0XFF,0XFF,0XFF)
#define COLOR_SOFT_THEME	/*Color(9,128,162)*/ Color(2,45,81)
#define COLOR_SOFT_LIGHT	/*Color(0XA1,0XAF,0XC9)*/ Color(227,233,238)
#define COLOR_2LEVEL_THEME	Color(70,103,131)
#define COLOR_PAGEBT_RUNNING_UNSHOW_START Color(0X1D,0X3A,0X90)
#define COLOR_PAGEBT_RUNNING_UNSHOW_MIDDLE Color(127,0X1D,0X3A,0X90)
#define COLOR_PAGEBT_RUNNING_UNSHOW_END Color(0XFF,0XFF,0XFF)
#define COLOR_INSERTBT_TX_DISABLED	Color(0XFF,0XFF,0XFF)

#define ED_SELECT_BK	RGB(201,213,228)

	//按钮颜色
#define CLR_BTBK_NORMAL	RGB(2,45,81)
#define CLR_BTBK_HOVER	RGB(70,103,131)
#define CLR_BTBK_CLICKED	RGB(70,103,131)
#define CLR_BTBK_DISABLED	RGB(107,137,182)

#define COLOR_BTBK_NORMAL	Color(2,45,81)
#define COLOR_BTBK_HOVER	Color(70,103,131)
#define COLOR_BTBK_CLICKED	Color(70,103,131)
#define COLOR_BTBK_DISABLED	Color(107,137,182)

#define CLR_STARTPAGE_TX	RGB(240,242,246)

#else

#ifdef XINYI_THEME
#define CLR_SOFT_THEME	/*RGB(9,128,162)*/ RGB(6,67,156)
#define CLR_SOFT_LIGHT	/*RGB(0XA1,0XAF,0XC9)*/ RGB(211,237,251)
#define CLR_2LEVEL_THEME	RGB(0,37,94)
#define CLR_INSERTBT_TX_DISABLED	RGB(0XFF,0XFF,0XFF)
#define COLOR_SOFT_THEME	/*Color(9,128,162)*/ Color(6,67,156)
#define COLOR_SOFT_LIGHT	/*Color(0XA1,0XAF,0XC9)*/ Color(211,237,251)
#define COLOR_2LEVEL_THEME	Color(0,37,94)
#define COLOR_PAGEBT_RUNNING_UNSHOW_START Color(0X1D,0X3A,0X90)
#define COLOR_PAGEBT_RUNNING_UNSHOW_MIDDLE Color(127,0X1D,0X3A,0X90)
#define COLOR_PAGEBT_RUNNING_UNSHOW_END Color(0XFF,0XFF,0XFF)
#define COLOR_INSERTBT_TX_DISABLED	Color(0XFF,0XFF,0XFF)

	//按钮颜色
#define CLR_BTBK_NORMAL	RGB(6,69,157)
#define CLR_BTBK_HOVER	RGB(10,88,197)
#define CLR_BTBK_CLICKED	RGB(0,39,95)
#define CLR_BTBK_DISABLED	RGB(107,137,182)

#define COLOR_BTBK_NORMAL	Color(6,69,157)
#define COLOR_BTBK_HOVER	Color(10,88,197)
#define COLOR_BTBK_CLICKED	Color(0,39,95)
#define COLOR_BTBK_DISABLED	Color(107,137,182)

#define CLR_STARTPAGE_TX	RGB(0X39,0X45,0X65)

#else

#define CLR_SOFT_THEME	RGB(0X06,0X91,0X9D)
#define CLR_SOFT_LIGHT	RGB(0XE6,0XF4,0XF5)
#define CLR_2LEVEL_THEME	RGB(0,0X57,0X5F)
#define CLR_INSERTBT_TX_DISABLED	RGB(0X97,0X97,0X97)
#define COLOR_SOFT_THEME	Color(0X06,0X91,0X9D)
#define COLOR_SOFT_LIGHT	Color(0XE6,0XF4,0XF5)
#define COLOR_2LEVEL_THEME	Color(0,0X57,0X5F)
#define COLOR_PAGEBT_RUNNING_UNSHOW_START Color(255,0X06,0X91,0X9D)
#define COLOR_PAGEBT_RUNNING_UNSHOW_MIDDLE Color(127,0X06,0X91,0X9D)
#define COLOR_PAGEBT_RUNNING_UNSHOW_END Color(0,0XFF,0XFF,0XFF)
#define COLOR_INSERTBT_TX_DISABLED	Color(0X97,0X97,0X97)

	//按钮颜色
#define CLR_BTBK_NORMAL	RGB(0X06,0X91,0X9D)
#define CLR_BTBK_HOVER	RGB(0X0A,0XB6,0XC5)
#define CLR_BTBK_CLICKED	RGB(0,0X57,0X5F)
#define CLR_BTBK_DISABLED	RGB(0X6B,0XAF,0XB6)

#define COLOR_BTBK_NORMAL	Color(0X06,0X91,0X9D)
#define COLOR_BTBK_HOVER	Color(0X0A,0XB6,0XC5)
#define COLOR_BTBK_CLICKED	Color(0,0X57,0X5F)
#define COLOR_BTBK_DISABLED	Color(0X6B,0XAF,0XB6)

	//渐变色背景
#define GRADIENT_MAIN_BK	//开关，软件标题栏使用渐变色背景
#define BIOWET_BK	//开关，软件使用博维配色风格
#define GRADIENT_SOME_BTBK	//开关，部分按钮在特定配色下使用渐变色背景

#define CLR_STARTPAGE_TX	RGB(0X39,0X45,0X65)

#endif

#endif


#define CLR_WAITTOTESTSAM	RGB(140,140,141)
#define CLR_STD RGB(209,56,204)
#define CLR_POSCOMPARE RGB(27,125,245)
#define CLR_CRITICALPOSCOMPARE RGB(31,181,88)
#define CLR_NONETEMCOMPARE RGB(19,191,191)
#define CLR_NEGCOMPARE RGB(250,144,22)
#define CLR_CALIBRATOR RGB(142,90,215)
#define CLR_RETESTSAM RGB(250,55,65)

#define CLR_THREVALUE1	Color(0,0,255)	//通道1阈值线默认颜色
#define CLR_THREVALUE2	Color(255,0,0)	//通道2阈值默认颜色
#define CLR_THREVALUE3	Color(0,255,0)	//通道3阈值默认颜色
 
#ifdef PS96INSTRUMENT
#define CLR_THREVALUE4	Color(138,0,0)	//通道4阈值默认颜色
#define CHANNEL4COLOR	RGB(138,0,0)	//通道4曲线默认颜色
#define CHANNEL5COLOR	RGB(225,199,21)	//通道5曲线默认颜色
#else
#define CLR_THREVALUE4	Color(225,199,21)	//通道4阈值默认颜色
#define CHANNEL4COLOR	RGB(225,199,21)	//通道4曲线默认颜色
#define CHANNEL5COLOR	RGB(138,0,0)	//通道5曲线默认颜色
#endif

#define CLR_THREVALUE5	Color(225,199,21)	//通道5阈值默认颜色
#define CLR_THREVALUE6	Color(0XFA,0X91,0X91)	//通道6阈值默认颜色

#define CHANNEL1COLOR	RGB(0,0,255)	//通道1曲线默认颜色
#define CHANNEL2COLOR	RGB(255,0,0)	//通道2曲线默认颜色
#define CHANNEL3COLOR	RGB(0,255,0)	//通道3曲线默认颜色

#define CHANNEL6COLOR	RGB(0XFA,0X91,0X91)	//通道6曲线默认颜色

//透明度
#define CLRALPHAVALUE	32	//遮罩窗口透明度，默认值130

//项目基本信息默认基因名
#define DEFAULT_TARGETNAME _T("Actin")

//流程编辑控件参数
#define RGB_SEGEDITSTEP	RGB(2,91,199)	//步骤编辑字体颜色
#define RGB_SEGEDITSEG	RGB(0X56,0X56,0X56)	//程序段（段名称、循环数）编辑字体颜色

//流程编辑控件相关项最大最小值
#define MAXSTEPTEM	100	//流程可设置的最大温度
#define MINSTEPTEM	4	//流程可设置的最小温度

#define MAXGRADIENTTEM	90	//梯度温度可设置的最大温度
#define MINGRADIENTTEM	40	//梯度温度可设置的最小温度

#define MAXSTEPHOLDSEC	3600	//步骤可设置的最大延时时间
#define MINSTEPHOLDSEC	1	//步骤可设置的最小延时时间

#define MAXSEGCYCLENUM	99	//步骤可设置的最大循环数
#define MINSEGCYCLENUM	1	//步骤可设置的最小循环数

#ifdef N96INSTRUMENT
	#define AMLTEMRATE_MAXVALUE	6	//扩增段升降温速率最大值
#else
	#define AMLTEMRATE_MAXVALUE	12	//扩增段升降温速率最大值,N16使用
#endif

//实验体积范围
#define REACTVOLUME_MINVALUE	10
#define REACTVOLUME_MAXVALUE	50

//梯度参数
#define GRADIENT_MINVALUE	40	//梯度最低温度
#define GRADIENT_MAXVALUE	90	//梯度最高温度

//字数限制
#define TEXTNUMLIMIT1	50

//熔曲分析
#define MELT_INTERPOLATION_MINDOTNUM	20	//多项式拟合进一步反算Tm值，插补的数据点数最小值，设置改最小值的原因是“防止出现小于或者等于0的值，导致内存申请失败，提示out of memory”

//Ct最小值
#define CT_MINVALUE	 2	//计算出的Ct值小于该值时，认为是N/A
 
//临时
#define MINIMUM_CHANNUM	1	//最小通道数
#define MINIMUM_TUBENUM	1	//最小试管数
#define CHANNELNUM	4
#define REALTUBENUM	96
#define CURVE_X_DEFMAXVALUE	41//曲线绘制控件X轴默认最大值
#define CT_COLUMN_INDEX	6	//Ct值在结果列表框中的下标
#define CALCDENSITY_COLUMN_INDEX	9	//反算浓度值值在结果列表框中的下标
#define STR_NOCT	_T("N/A")	//无Ct时输出的字符串

#define DEFAULT_USER_IVD	_T("user")
#define DEFAULT_USER_REAGENT	_T("admin")
#define DEFAULT_PASSWORD	_T("123456")
#define DEFAULT_DEVELOPER _T("BWRD")

#ifdef PS96INSTRUMENT
#define SOFRVERION_STRING	_T("PS96 V2.1.18")
#else 

#ifdef N96INSTRUMENT
	#ifdef XINYI_THEME
		#define SOFRVERION_STRING	_T("C6-Analyzer V1.0.0")
	#else
		#ifdef NOLOGO_VERSION
			#define SOFRVERION_STRING	_T("QPCR V1.0.1")
		#else
			#define SOFRVERION_STRING	_T("QPCR V1.0.11")
		#endif
	#endif
	
#else
	#ifdef XINYI_THEME
		#define SOFRVERION_STRING	_T("C1-Analyzer V1.0.0")
	#else
		#ifdef CAN_YOUNG
			#define SOFRVERION_STRING	_T("KYBioFastQuant V1.1")
		#else
			#ifdef ODREI
				#define SOFRVERION_STRING	_T("OFQ16 V1.0.13")
			#else
				#ifdef NOLOGO_VERSION
					#define SOFRVERION_STRING	_T("PCR Analyzer V1.1.0")
				#else
					#define SOFRVERION_STRING	_T("nQ16 V1.1.0")
				#endif
			#endif
		#endif
	#endif
	
#endif

#endif


#define MAXTEMPERATURE	100.0	//可设置的最大温度
#define MINTEMPERATURE	4.0	//可设置的最小温度
#define MAXCYCLENUM	999	//可设置的最大循环数
#define MINCYCLENUM	1	//可设置的最小循环数

#define PASS_TEXT	_T("Pass")
#define FAIL_TEXT	_T("Fail")
#define POSITIVE_TEXT _T("Positive")
#define NEGATIVE_TEXT _T("Negative")
#define INVALID_TEXT _T("Invalid")

//PS96 IVD模式，旧的导出报告名称后缀
#define PS96IVD_OLD_RESULT_SUFFIX	_T("_Old")
#define PS96IVD_DATA_RESULT_SUFFIX	_T("_Data")

//开关
#define LBDBCLICK_ZOOM	//开关，左键双击后弹出缩放窗口

//#define SELFADD_PRJLSTINFO	//开关，自定义项目列表按钮信息

#define FILE_ICON_NEGTIVE_SEQUENCE	//开关，主页文件图标按照倒序显示

#define NEW_SEGMENT_INSERTBT	//开关，插入程序段、步骤使用新样式

#define ONEPROC_ONEAMLSTEP	//开关，设置“实验流程应只能将一个步骤设置为扩增步骤”

#define POPDLG_USESHADEBK	//开关，弹窗使用遮罩效果

#ifdef XINYI_THEME
	#define ALLOW_LOADDATA_FROMEXCELFILE	//允许从Excel文件中加载数据进行分析
#endif

//#define USEMENUFORMAT	//开关，整个程序使用菜单风格，20220427，孙文郎

#define TUBE_USESAMEDYEANDCORLOR	//开关，所用孔位使用同样的染料名和颜色,20220621,孙文郎

#define N16USENEWSAMPLETYPE //开关，N16仪器使用新的样本类型组合

#define DELETE_PART_TUBEINFO	//开关，删除样本信息时，只删除该页面对应信息

#define IVDALLOWCHANGECOLOR	//开关，IVD模式下允许修改样本每个通道的颜色信息

//#define HIDE_PROJECTTYPE	//开关，隐藏项目类型
#define HIDE_PROJECTUNIT	//开关，隐藏项目基本信息页面的单位

//#define HIDE_PROJECT	//开关，隐藏项目

#define HIDE_PROJECT_RESULTCHECK_PAGE	//开关，隐藏项目结果判断页面

#define ADJUST_ANALYSEDLG_THRESHOLD_ED_SIZE_POSITION	//开关，调整分析页面阈值编辑框大小和位置

#define AXISZOOMDLG_MINMAX_INMIDDLE	//坐标轴参数编辑窗口“最大值、最小值标识”水平居中显示

#ifdef PS96INSTRUMENT
	#define SHIELDMELTFUNC	//开关，屏蔽熔曲功能
	#define SHIELD_COLOR_BLOCK	//开关，屏蔽孔板颜色显示 
#endif

#define MONITOR_USE_AMP_DELTARN	//开关，监控页面，扩增数据显示DeltaRn数据类型

//#define BASESETPAGE_HIDE_SAVEBT	//开关，基本设置页面隐藏保存相关按钮

#define HIDE_ADDCYCLE_FUNC	//开关，隐藏运行过程中添加循环数功能

#define ONLY_ONE_DEFAULT_ACCOUNT	//开关，只有一个默认用户

#define DYE_COLOR_NEVER_DISABLE	//开关，永远不禁用孔板编辑页面的染料名和颜色选项

#define NEWPROJECT_INITALL	//开关，新建项目时，初始化项目的所有信息，不是只初始化通道选择信息

#define IVD_SHOW_NEWBT	//开关，PS96IVD模式，显示新建按钮

#define PC_NC_RESULT_MERGE	//开关，PS96,PC NC 的检测结果合并显示
//#define PS96_IGNORE_IC	//开关，PS96忽略IC功能

#define USE_SELFDEFINED_COLORSELECTDLG	//开关，使用自定义颜色选择窗口

#define DISABLE_FIXED_METLSTEPNUM	//开关，禁用“熔曲程序段使用固定步骤个数（默认2个）”	

//#ifndef PS96INSTRUMENT
	#define USE_STARTPAGE	//开关，启用启动页
//#endif

//#define ORDINARY_USER_CHECK_FILEUSERNAME	//开关，普通用户权限，打开文件时，检查文件里的用户名和当前用户名是否一致，不一致的话不允许打开

#ifndef PS96INSTRUMENT
	#define DEFAULT_ADMIN	//开关，默认用户是admin，和ONLY_ONE_DEFAULT_ACCOUNT组合使用

#else
	#define IVD_USE_TEMPALTE_LIST	//开关，IVD模式使用模板列表文件，不使用Project
	#define IVD_SELFSELECT_SAVEPATH	//开关，IVD模式下，用户自己选择实验文件和报告文件保存路径
#endif


#ifndef PS96INSTRUMENT

#ifndef N96INSTRUMENT
	#define SAMPLATE_16SITE	//开关，16孔PCR
#endif

#endif

//#define DEFFILEPATH_USE_MYDOCUMENT_DIR	//开关，默认文件保存路径使用PC本地文档路径

#ifdef N96INSTRUMENT
	#ifdef XINYI_THEME
		#define DEF_FILEDIR_NAME	_T("\\C6\\")
	#else
		#define DEF_FILEDIR_NAME	_T("\\N96\\")
	#endif
#else 
	#ifdef PS96INSTRUMENT
		#define DEF_FILEDIR_NAME	_T("\\PS96\\")
	#else
		#ifdef XINYI_THEME
			#define DEF_FILEDIR_NAME	_T("\\C1\\")
		#else
			#ifdef CAN_YOUNG
				#define DEF_FILEDIR_NAME	_T("\\FastQuantData\\")
			#else
				#define DEF_FILEDIR_NAME	_T("\\nQ16\\")
			#endif
		#endif
	#endif
#endif

#ifdef SAMPLATE_16SITE
	#define LIDTEMMAX	120	//热盖温度最大值
	#define LIDTEMMIN	25	//热盖温度最小值
#else
	#define LIDTEMMAX	105	//热盖温度最大值
	#define LIDTEMMIN	25	//热盖温度最小值
#endif

#define N96DEFAULT_LIDTEM	100	//N96默认的热盖温度

#ifdef PS96INSTRUMENT
#define CTTEXTTAG	_T("Tt")	//默认显示的Ct值标识
#else
#define CTTEXTTAG	_T("Ct")
#endif

//文件识别
#define USEROPENEDTEMPLATEFILENAME	_T("template.tl")
#define USEROPENEDTEMPLATEFILTAG	0X12345678

#define DEFAULTTEMPLATEFILENAME	_T("templateDefault.tl")
#define DEFAULTTEMPLATEFILTAG	0X1234567A

#define CYINFO_FILENAME	_T("CanYoung.set")
#define CYINFO_FILETAG	0X1234567B

#define SYSANAPARAMFILENAME	_T("a.s")
#define SYSANAPARAMFILETAG	0X1234567C

#define DEFGENE_FN	_T("dg.n")
#define DEFGENE_FT	0X1234567D

















