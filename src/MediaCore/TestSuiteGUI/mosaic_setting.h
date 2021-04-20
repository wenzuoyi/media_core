#ifndef MOSAIC_SETTING_H_
#define MOSAIC_SETTING_H_
#include "base_auto_layout_dialog.h"
#include "render_file_player.h"
class MosaicSetting : public BaseAutoLayoutDialog {
DECLARE_DYNAMIC(MosaicSetting)
  MosaicSetting(CWnd* pParent = NULL); // 标准构造函数
  virtual ~MosaicSetting();
  core::RegionPtr GetMosaicRegion() const;
  BOOL OnInitDialog() override;
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOSAICSETTING_DIALOG };
  #endif
  afx_msg void OnBnClickedOk();
protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持
DECLARE_MESSAGE_MAP()
private:
	int x_{ 100 };
	int y_{ 100 };
	int width_{ 100 };
	int height_{ 100 };
	core::RegionPtr region_;
};
#endif // MOSAIC_SETTING_H_
