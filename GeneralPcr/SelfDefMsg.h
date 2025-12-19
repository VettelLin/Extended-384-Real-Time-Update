
#pragma once

#define WM_REPORT_SELFCHECK_RESULT	WM_USER + 6000 
#define WM_UPDATE_CURVE	WM_USER + 6001
#define WM_SWITCHTOWORKPAGE	WM_USER + 6002	//点击“新建”按钮，切换到工作页面
#define WM_MONITORCHAN_CHANGE  WM_USER + 6004	//监控页面当前光路改变
#define WM_LSTSELCHANGED	WM_USER + 6007	
#define WM_CLOSEPAGE	WM_USER + 6008	//文件管理窗口，关闭页面按钮被点击

// WM_USER + 6009 已被占用
// WM_USER + 6010 已被占用
#define WM_NEWAXIS_MAXMIN	WM_USER + 6011	//更改坐标轴最大值最小值
#define	WM_FUNCEDITKILLFOCUS	WM_USER + 6012

// WM_USER + 6013 已被占用
#define WM_PRJINFOCHANGED	WM_USER + 6014	//项目信息改变，wParam为0表示新建，1表示删除，2表示保存
#define WM_RUNINTERRUPT	WM_USER + 6015	//断电，重启软件后重新运行流程消息
#define WM_TARGET_CHANGED	WM_USER + 6016	//目标名称改变

#define WM_RESULT_FIND	WM_USER + 6017	//结果查询搜索到新项
#define WM_PRJTYPECHANGED	WM_USER + 6018	//项目类型变更
#define WM_PRJBTDBCLICKED	WM_USER + 6019	//历史文件信息按钮双击消息
#define WM_PRJBTCLICKED	WM_USER + 6020	//历史文件信息按钮单击消息
#define WM_DATECTRLCHANGED	WM_USER + 6021	//日历控件时间改变
#define WM_BOXOUTINTDRESULT	WM_USER + 6022	//进出仓按线程返回的结果
#define WM_ROWSELECTCHANGE	WM_USER + 6023	//列表框选中行改变，通知父窗口
#define WM_SETLISDCONNECTSTATE		WM_USER + 6024	//设置LIS连接状态
#define WM_EDITENTERPUSHED	WM_USER + 6025	//编辑框Enter按键被按下
#define WM_TELLPARENTROWCHANGED	WM_USER + 6026	//编辑框行信息改变，通知父窗口
#define WM_TELLPARENTSITESELECTCHANGED	WM_USER + 6027	//孔板孔位选择改变，通知父窗口
#define WM_HIDESONCTRL	WM_USER + 6028	//通知父窗口去隐藏子窗口
//WM_USER + 6029	//编辑框文本改变，通知父窗口
//WM_USER + 6030	//SetText导致编辑框文本改变，通知父窗口
//WM_USER + 6031	//列表框使用，占位

#define WM_CHANBTCLICKED WM_USER + 6032	//通知GeneralDlg 通道切换按钮已经按下

//WM_USER + 6033	//通知列表框颜色块改变了，占位
//WM_USER + 6034	//通知父窗口Delete按键被按下，占位
//WM_USER + 6035	//启动页线程反馈消息	

#define WM_RUNPOWEROFFPROTECT WM_USER + 6036	//通知CMiddleDlg窗口运行断电保护实验

#define WM_PRJ_CHANNEL_DYENAME_CHANGED WM_USER + 6037	//通知项目管理窗口染料名称改变了
 





