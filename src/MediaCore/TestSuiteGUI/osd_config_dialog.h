#ifndef OSD_CONFIG_DIALOG_H_
#define OSD_CONFIG_DIALOG_H_
#include "afxwin.h"
#include "afxcmn.h"
#include "osd_config_datatype.h"
class OSDConfigDialog : public CDialogEx {
DECLARE_DYNAMIC(OSDConfigDialog)
public:
  OSDConfigDialog(CWnd* pParent = NULL); // 标准构造函数  
  virtual ~OSDConfigDialog(); // 对话框数据
  virtual BOOL OnInitDialog();
  OSDConfigResultListPtr GetConfigResultList() const;
  void SetConfigResultList(OSDConfigResultListPtr osd_config_result);
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedApplyButton();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnCbnSelchangeOsdIdsCombox();
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDOSDCONTENT_DIALOG };
  #endif
protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持
DECLARE_MESSAGE_MAP()
private:
	void AssignItemValue() const;
	int osd_enable_{ 0 };
	unsigned x_pos_{ 100 };
	unsigned y_pos_{ 100 };
	unsigned width_ {50};
	unsigned height_ {8};
	OSDConfigResultListPtr osd_config_result_list_;
	CComboBox osd_ids_;
	CSpinButtonCtrl xpos_spin_;
	CSpinButtonCtrl ypos_spin_;
	CSpinButtonCtrl width_spin_;
	CSpinButtonCtrl height_spin_;
	CString content;
public:
	afx_msg void OnBnClickedOsdEnableCheck();
};
#endif // OSD_CONFIG_DIALOG_H_
