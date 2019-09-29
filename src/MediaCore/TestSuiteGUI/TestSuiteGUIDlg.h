#ifndef TEST_SUITE_GUI_DLG_H_
#define TEST_SUITE_GUI_DLG_H_
#include <future>
#include <fstream>
#include "afxwin.h"
#include "video_output_media_source.h"
class CTestSuiteGUIDlg : public CDialogEx, public output::VideoOutputMediaSourceEvent {
public:
	CTestSuiteGUIDlg(CWnd* pParent = NULL);	// 标准构造函数
	afx_msg void OnBnClickedButtonOpen();
  afx_msg void OnBnClickedButtonPlayctrl();
	afx_msg void OnBnClickedButtonClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSUITEGUI_DIALOG };
#endif
protected:
	HICON m_hIcon;
	void OnVideoOutputMediaExceptionEvent(unsigned error_code) override;
	void OnCustomPainting(HDC hdc) override;
	void OnTransmitDataEvent(output::VideoFramePtr video_frame) override;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void StartReadMediaFile();
	void StopReadFile();
  static inline int GetYUVFrameSize() { return VIDEO_WIDTH * VIDEO_HEIGHT * 3 / 2;}
	bool is_playing_{ false };
	std::ifstream ifs_;
	bool exit_{ false };
	std::future<void> read_file_task_;
	CStatic display_area_;
	output::VideoOutputMediaSourcePtr video_output_media_source_;
	static const int VIDEO_WIDTH;
	static const int VIDEO_HEIGHT;
};


#endif // TEST_SUITE_GUI_DLG_H_
