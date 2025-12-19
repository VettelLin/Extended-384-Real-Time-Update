#pragma once

//实验编辑模式--包括无限制、IVD（只能修改样本信息）模式、运行中、运行结束
typedef enum ExpEditMode{
	EEEM_NOLIMIT,EEEM_IVD,EEEM_RUNNING,EEEM_END
}EEXPEDITMODE;