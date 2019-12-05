#ifndef TESTSUITEGUI_H_
#define TESTSUITEGUI_H_
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
class TestSuiteGUIApp : public CWinApp
{
public:
	TestSuiteGUIApp();
	virtual BOOL InitInstance();
protected:
	DECLARE_MESSAGE_MAP()
};
extern TestSuiteGUIApp theApp;
#endif // TESTSUITEGUI_H_
