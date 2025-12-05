#pragma once

#include "resource.h"

// 处理当前程序的资源，需要在使用当前程序资源时先声明一个变量
class CBWGenUILibResource  
{
public:
	CBWGenUILibResource();
	virtual ~CBWGenUILibResource();

protected:
	HINSTANCE m_hInstOld;
};
