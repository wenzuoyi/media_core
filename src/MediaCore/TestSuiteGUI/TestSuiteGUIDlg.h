#ifndef TEST_SUITE_GUI_DLG_H_
#define TEST_SUITE_GUI_DLG_H_
#include <map>
#include <memory>
#include "afxwin.h"
#include "render_file_player.h"
#include "base_auto_layout_dialog.h"
#include "osd_config_dialog.h"
#include "mouse_locator.h"


class TestSuiteGUIDialog : public BaseAutoLayoutDialog , public core::RenderFilePlayerEvent {
public:
  TestSuiteGUIDialog(CWnd* pParent = nullptr); // 标准构造函数
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSUITEGUI_DIALOG };
  #endif
protected:
  HICON m_hIcon;
  void OnPlayerException(int error_code, const std::string& error_message) override;
  void OnVideoFrameInfo(core::VideoBaseInfoPtr frame_info) override;
  void OnAudioFrameInfo(core::AudioBaseInfoPtr frame_info) override;
  void OnPreDecodingPackage(core::VideoPackagePtr package) override;
  void OnPreDecodingPackage(core::AudioPackagePtr package) override;
  void OnPreRenderingFrame(core::VideoFramePtr video_frame) override;
  void OnPreRenderingFrame(core::AudioSamplePtr audio_sample) override;
  void OnPostRenderingFrame(core::VideoFramePtr video_frame) override;
  void OnPostRenderingFrame(core::AudioSamplePtr audio_sample) override;
  void OnExtraDisplay(HDC hdc) override;
  void OnEOF() override;
  void OnBOF() override;
  BOOL OnInitDialog() override;
  void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 支持
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg void OnRenderPlay();
  afx_msg void OnRenderStop();
  afx_msg void OnRenderOpenFile();
  afx_msg void OnRenderCloseFile();
  afx_msg void OnRenderOSDConfig();
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnRenderImageratioAdpater();
  afx_msg void OnRenderImageratio43();
  afx_msg void OnRenderImageratio169();
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
  afx_msg void OnRenderImageratioROI();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnHandlerMosaic();
  afx_msg void OnClose();
	afx_msg void OnHandlerFlipLeftToRight();
	afx_msg void OnHandlerFlipUpToDown();
	afx_msg void OnHandlerMirror();
DECLARE_MESSAGE_MAP()
private:
  void EnableRenderMenuItem(std::map<unsigned, bool>&& menu_items_map) const;
  void MutexPictureImageRatioMenuItems(unsigned ui_id);
  CStatic display_area_;
  CRect screen_rect_;
  OSDConfigDialog osd_config_dialog_;
  OSDConfigResultListPtr osd_config_result_list_;  
  MouseLocator mouse_locator_;
  core::RenderFilePlayerPtr render_file_player_{ nullptr };
};
#endif // TEST_SUITE_GUI_DLG_H_
