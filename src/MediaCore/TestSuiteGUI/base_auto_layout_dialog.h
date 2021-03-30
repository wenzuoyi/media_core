#ifndef BASE_AUTO_LAYOUT_DIALOG_H_
#define BASE_AUTO_LAYOUT_DIALOG_H_
#include "afxdialogex.h"
#include <memory>
#include <map>
struct AnchorBaseInfo {
	double top;
	double left;
	double right;
	double bottom;
};
using AnchorBaseInfoPtr = std::shared_ptr<AnchorBaseInfo>;
using AnchorBaseMap = std::map<unsigned long, AnchorBaseInfoPtr>;

class BaseAutoLayoutDialog :	public CDialogEx {
public:
	BaseAutoLayoutDialog(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~BaseAutoLayoutDialog();
protected:
  void Resize();
	bool GetControlLayoutInfo(int control_id, CRect* rect);
  BOOL OnInitDialog() override;
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
private:
  void Init();
  AnchorBaseMap anchor_base_map_;
};
#endif // BASE_AUTO_LAYOUT_DIALOG_H_
