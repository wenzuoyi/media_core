#pragma once
#include "afxcmn.h"
#include <vector>

// RotationSettingDialog 对话框

class RotationSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(RotationSettingDialog)
public:
	enum class RotationType {
		kDegree90 = 0,
		kDegree180 = 1,
		kDegree270 = 2,
    kDegreeUnknown
	};
	RotationSettingDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RotationSettingDialog();
	bool EnableRotation() const;
	bool IsUseCostomizeDegree() const;
	RotationType GetRotationType() const;
	int GetRotationDegree() const;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckRotationEnable();
	afx_msg void OnBnClickedCheckDegree90();
	afx_msg void OnBnClickedCheckDegree180();
	afx_msg void OnBnClickedCheckDegree270();
	afx_msg void OnBnClickedCheckCustomerdef();
	DECLARE_MESSAGE_MAP()
private:
	void EnableControl(BOOL enable, const std::vector<int>& id_list);
	void EnableCheckBox(BOOL enable, const std::vector<int>& id_list);
	BOOL enable_{ FALSE };
	BOOL degree90_{ FALSE };
	BOOL degree180_{ FALSE };
	BOOL degree270_{ FALSE };
	mutable BOOL degree_customize_{ FALSE };
	int customize_degree_{ 0 };
	CSpinButtonCtrl rotation_spin_;
};
