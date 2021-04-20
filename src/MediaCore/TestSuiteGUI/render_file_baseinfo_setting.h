#ifndef RENDER_FILE_BASEINFO_SETTING_H_
#define RENDER_FILE_BASEINFO_SETTING_H_
#include "base_auto_layout_dialog.h"
#include "afxwin.h"
#include "render_file_player_dataype.h"
class RenderFileBaseInfoSetting : public BaseAutoLayoutDialog {
DECLARE_DYNAMIC(RenderFileBaseInfoSetting)
  RenderFileBaseInfoSetting(CWnd* pParent = NULL);
  virtual ~RenderFileBaseInfoSetting();
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENDERFILESETTING_DIALOG };
  #endif
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	std::string GetResolution() const;
	core::RenderFormat GetFormat() const;
	bool EnableLoopPlayback() const;
protected:
  BOOL OnInitDialog() override;
  virtual void DoDataExchange(CDataExchange* pDX);
DECLARE_MESSAGE_MAP()
private:
	CComboBox ui_combox_format_;
	int width_{ 320 };
	int height_{ 180 };
	std::string resolution_;
	core::RenderFormat format_;
	BOOL enable_loop_playback_;
};
#endif // RENDER_FILE_BASEINFO_SETTING_H_
