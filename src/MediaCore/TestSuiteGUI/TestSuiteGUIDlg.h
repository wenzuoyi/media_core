#ifndef TEST_SUITE_GUI_DLG_H_
#define TEST_SUITE_GUI_DLG_H_
#include <future>
#include <fstream>
#include <map>
#include <memory>
#include "afxwin.h"
#include "base_auto_layout_dialog.h"
#include "osd_config_dialog.h"
#include "video_output_media_source.h"
#include "mosaic_handler.h"
#include "mouse_locator.h"


class TestSuiteGUIDialog : public BaseAutoLayoutDialog, public output::VideoOutputMediaSourceEvent, public handler::MosaicHandlerEvent {
public:
  TestSuiteGUIDialog(CWnd* pParent = nullptr); // 标准构造函数
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSUITEGUI_DIALOG };
  #endif
protected:
  HICON m_hIcon;
  void OnVideoOutputException(unsigned error_code) override;
  void OnVideoCustomPainting(HDC hdc) override;
  void OnVideoTransmitFrame(output::VideoFramePtr video_frame) override;
  void OnTransmitVideoFrame(handler::VideoHandlerType video_handler_type, handler::VideoFramePtr video_frame) override;
  void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 支持
  BOOL OnInitDialog() override;
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
DECLARE_MESSAGE_MAP()
private:
	inline int GetYUVFrameSize();
  void StartReadMediaFile();
  void StopReadFile();
  void PostVideoFrame(const std::vector<char>& buffer) const;
  void EnableRenderMenuItem(std::map<unsigned, bool>&& menu_items_map) const;
  void MutexPictureImageRatioMenuItems(unsigned ui_id);
  bool is_playing_{false};
  std::ifstream ifs_;
  bool exit_{false};
  std::future<void> read_file_task_;
  CStatic display_area_;
  CRect screen_rect_;
  OSDConfigDialog osd_config_dialog_;
  OSDConfigResultListPtr osd_config_result_list_;  
  MouseLocator mouse_locator_;
  output::VideoOutputMediaSourcePtr video_output_media_source_;
  handler::MosaicHandlerPtr mosaic_handler_;
  static const int VIDEO_WIDTH;
  static const int VIDEO_HEIGHT;
public:
	afx_msg void OnHandlerMosaic();
};
#endif // TEST_SUITE_GUI_DLG_H_
