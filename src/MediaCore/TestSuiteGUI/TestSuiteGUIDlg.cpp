// TestSuiteGUIDlg.cpp : 实现文件
//
#include "stdafx.h"
#include <vector>
#include <chrono>
#include "TestSuiteGUI.h"
#include "TestSuiteGUIDlg.h"
#include "character_set_convertor.h"
#include "render_file_player_dataype.h"
#include "afxdialogex.h"
#include "render_file_baseinfo_setting.h"
using namespace std::chrono_literals;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx {
public:
  CAboutDlg(); // 对话框数据
  #ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
  #endif
protected:
  virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

TestSuiteGUIDialog::TestSuiteGUIDialog(CWnd* pParent /*=NULL*/) : BaseAutoLayoutDialog(IDD_TESTSUITEGUI_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TestSuiteGUIDialog::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STATIC_MAIN, display_area_);
}

void TestSuiteGUIDialog::OnPlayerException(int error_code, const std::string& error_message) {
}

void TestSuiteGUIDialog::OnVideoFrameInfo(core::VideoBaseInfoPtr frame_info) {
}

void TestSuiteGUIDialog::OnAudioFrameInfo(core::AudioBaseInfoPtr frame_info) {
}

void TestSuiteGUIDialog::OnPreDecodingPackage(core::VideoPackagePtr package) {
}

void TestSuiteGUIDialog::OnPreDecodingPackage(core::AudioPackagePtr package) {
}

void TestSuiteGUIDialog::OnPreRenderingFrame(core::VideoFramePtr video_frame) {
}

void TestSuiteGUIDialog::OnPreRenderingFrame(core::AudioSamplePtr audio_sample) {
}

void TestSuiteGUIDialog::OnPostRenderingFrame(core::VideoFramePtr video_frame) {
}

void TestSuiteGUIDialog::OnPostRenderingFrame(core::AudioSamplePtr audio_sample) {
}

void TestSuiteGUIDialog::OnExtraDisplay(HDC hdc) {
	mouse_locator_.Draw(hdc, [this](const CPoint& left_top, const CPoint& right_bottom) {
		CRect rect(left_top, right_bottom);
		rect.left = (left_top.x * render_file_player_->Width() / screen_rect_.Width());
		rect.top = (left_top.y * render_file_player_->Height() / screen_rect_.Height());
		rect.right = (right_bottom.x * render_file_player_->Width() / screen_rect_.Width());
		rect.bottom = (right_bottom.y * render_file_player_->Height() / screen_rect_.Height());
		return rect;
	});
}

void TestSuiteGUIDialog::OnEOF() {
}

void TestSuiteGUIDialog::OnBOF() {
}

BEGIN_MESSAGE_MAP(TestSuiteGUIDialog, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_CREATE()
	ON_WM_SIZING()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
  ON_WM_DESTROY()
  ON_COMMAND(ID_RENDER_OPEN, &TestSuiteGUIDialog::OnRenderOpenFile)
  ON_COMMAND(ID_RENDER_CLOSE, &TestSuiteGUIDialog::OnRenderCloseFile)
  ON_COMMAND(ID_RENDER_PLAY, &TestSuiteGUIDialog::OnRenderPlay)
  ON_COMMAND(ID_RENDER_STOP, &TestSuiteGUIDialog::OnRenderStop)
	ON_COMMAND(ID_RENDER_OSD, &TestSuiteGUIDialog::OnRenderOSDConfig)
	ON_COMMAND(ID_RENDER_IMAGERATIO_ADPATER, &TestSuiteGUIDialog::OnRenderImageratioAdpater)
	ON_COMMAND(ID_RENDER_IMAGERATIO_43, &TestSuiteGUIDialog::OnRenderImageratio43)
	ON_COMMAND(ID_RENDER_IMAGERATIO_169, &TestSuiteGUIDialog::OnRenderImageratio169)
	ON_COMMAND(ID_RENDER_IMAGERATIO_ROI, &TestSuiteGUIDialog::OnRenderImageratioROI)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_HANDLER_MOSAIC, &TestSuiteGUIDialog::OnHandlerMosaic)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL TestSuiteGUIDialog::OnInitDialog() {
	BaseAutoLayoutDialog::OnInitDialog();
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);
  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL) {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
    ASSERT(bNameValid);
    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }
  SetIcon(m_hIcon, TRUE); // 设置大图标
  SetIcon(m_hIcon, FALSE); // 设置小图标
  GetControlLayoutInfo(IDC_STATIC_MAIN, &screen_rect_);
  auto basic_param = std::make_shared<core::BasicPlayerParam>();
  basic_param->hwnd = display_area_.m_hWnd;  
  render_file_player_->Init(basic_param);
  EnableRenderMenuItem({
    {ID_RENDER_OPEN, true}, {ID_RENDER_PLAY, false}, {ID_RENDER_STOP, false}, {ID_RENDER_CLOSE, false}
  });
  EnableRenderMenuItem({
    {ID_RENDER_OSD, false}, {ID_RENDER_IMAGERATIO_ADPATER, false}, { ID_RENDER_IMAGERATIO_43, false},
    {ID_RENDER_IMAGERATIO_169, false}, {ID_RENDER_IMAGERATIO_ROI, false},{ ID_HANDLER_MOSAIC , false }
  });  
  return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void TestSuiteGUIDialog::OnSysCommand(UINT nID, LPARAM lParam) {
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  } else {
    if (nID == 0xF012 || nID == 0xF032 || nID == 0xF122) {
      return;
    }
    CDialogEx::OnSysCommand(nID, lParam);
    if (nID == SC_MAXIMIZE || nID == SC_RESTORE) {
      Resize();
	    GetControlLayoutInfo(IDC_STATIC_MAIN, &screen_rect_);
      //if (video_output_media_source_ != nullptr) {
      //  video_output_media_source_->ResizeWindow();
      //}
    }
	  TRACE("command:0x%x", nID);
  }
}

void TestSuiteGUIDialog::OnPaint() {
  if (IsIconic()) {
    CPaintDC dc(this); // 用于绘制的设备上下文
    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0); // 使图标在工作区矩形中居中
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2; // 绘制图标
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialogEx::OnPaint();
  }
}

HCURSOR TestSuiteGUIDialog::OnQueryDragIcon() {
  return static_cast<HCURSOR>(m_hIcon);
}

int TestSuiteGUIDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CDialogEx::OnCreate(lpCreateStruct) == -1)
    return -1;
  render_file_player_ = core::RenderFilePlayer::CreateInstance();
  if (render_file_player_ == nullptr) {
	  return -1;
  }
  render_file_player_->SetEvent(this);
  return 0;
}

void TestSuiteGUIDialog::OnDestroy() {
  if (render_file_player_ != nullptr) {
	  render_file_player_->Fini();
  }
	render_file_player_ = nullptr;
  CDialogEx::OnDestroy();
}

void TestSuiteGUIDialog::OnClose() {
  if (render_file_player_->Status() == core::RenderFilePlayer::PlayerStatus::kPause) {
	  render_file_player_->Resume();
  }
  if (render_file_player_->Status() == core::RenderFilePlayer::PlayerStatus::kRun) {
	  render_file_player_->Stop();
  }
  if (render_file_player_->Status() == core::RenderFilePlayer::PlayerStatus::kStop) {
	  render_file_player_->Close();
  }
  CDialogEx::OnClose();
}

void TestSuiteGUIDialog::OnRenderOpenFile() {
  CFileDialog file_dialog(TRUE, _T("*.yuv"), nullptr, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
                          _T("渲染文件(*.yuv)|*.yuv| All Files (*.*) |*.*||"), nullptr); // 打开文件对话框的标题名
  file_dialog.m_ofn.lpstrTitle = _T("选择渲染文件");
  if (file_dialog.DoModal() == IDOK) {
    const auto open_file_path = file_dialog.GetPathName();
    auto open_file_path_string = std::wstring(static_cast<LPCTSTR>(open_file_path));
    auto uri = utils::CharacterSetConvertor::Instance()->UnicodeToGBK(open_file_path_string);
    if (render_file_player_ != nullptr && render_file_player_->Open(uri)) {
		  RenderFileBaseInfoSetting dlg;
      if (dlg.DoModal() == IDOK) {
        render_file_player_->SetResolution(dlg.GetResolution());
        render_file_player_->SetFormat(dlg.GetFormat());
        render_file_player_->EnableLoopPlayback(dlg.EnableLoopPlayback());        
      } else {
        AfxMessageBox(L"未设置对应的渲染文件参数，打开文件失败!");
        return;
      }
    }
	  EnableRenderMenuItem({
		  { ID_RENDER_OPEN, false },{ ID_RENDER_PLAY, true },{ ID_RENDER_STOP, false },{ ID_RENDER_CLOSE, true }
	  });
	  EnableRenderMenuItem({
		  { ID_RENDER_OSD, false },{ ID_RENDER_IMAGERATIO_ADPATER, false },{ ID_RENDER_IMAGERATIO_43, false },
		  { ID_RENDER_IMAGERATIO_169, false },{ ID_RENDER_IMAGERATIO_ROI, false },{ ID_HANDLER_MOSAIC , false }
	  });
  }
}

void TestSuiteGUIDialog::OnRenderCloseFile() {
	if (render_file_player_ != nullptr) {
		render_file_player_->Close();
	}
	EnableRenderMenuItem({
		{ ID_RENDER_OPEN, true },{ ID_RENDER_PLAY, false },{ ID_RENDER_STOP, false },{ ID_RENDER_CLOSE, false }
	});
	EnableRenderMenuItem({
		{ ID_RENDER_OSD, false },{ ID_RENDER_IMAGERATIO_ADPATER, false },{ ID_RENDER_IMAGERATIO_43, false },
		{ ID_RENDER_IMAGERATIO_169, false },{ ID_RENDER_IMAGERATIO_ROI, false },{ ID_HANDLER_MOSAIC , false }
	});
}

void TestSuiteGUIDialog::OnRenderPlay() {
  if (render_file_player_ != nullptr) {
    render_file_player_->Play();
	  EnableRenderMenuItem({
	    {ID_RENDER_OPEN, false}, {ID_RENDER_PLAY, false}, {ID_RENDER_STOP, true}, {ID_RENDER_CLOSE, false}
	  });
	  EnableRenderMenuItem({
	    {ID_RENDER_OSD, true}, {ID_RENDER_IMAGERATIO_ADPATER, true }, {ID_RENDER_IMAGERATIO_43, true },
	    {ID_RENDER_IMAGERATIO_169, true }, {ID_RENDER_IMAGERATIO_ROI, true },{ ID_HANDLER_MOSAIC , true }
	  });
  }
}

void TestSuiteGUIDialog::OnRenderStop() {
  if (render_file_player_ != nullptr) {
	  render_file_player_->Stop();
    EnableRenderMenuItem({
      {ID_RENDER_OPEN, false}, {ID_RENDER_PLAY, true}, {ID_RENDER_STOP, false}, {ID_RENDER_CLOSE, true}
    });
    EnableRenderMenuItem({
      {ID_RENDER_OSD, false}, {ID_RENDER_IMAGERATIO_ADPATER, false}, {ID_RENDER_IMAGERATIO_43, false},
      {ID_RENDER_IMAGERATIO_169, false}, {ID_RENDER_IMAGERATIO_ROI, false},{ ID_HANDLER_MOSAIC , false }
    });
	  UpdateWindow();
  }
}

void TestSuiteGUIDialog::EnableRenderMenuItem(std::map<unsigned, bool>&& menu_items_map) const {
	auto menu = GetMenu();
  for (const auto& item : menu_items_map) {
	  menu->EnableMenuItem(item.first, item.second ? MF_ENABLED : MF_GRAYED);
  }
}

void TestSuiteGUIDialog::OnRenderOSDConfig() {
  if (osd_config_result_list_ == nullptr) {
	  osd_config_result_list_ = std::make_shared<OSDConfigResultList>();
	  for (auto i = 0U; i < 8; ++i) {
		  auto osd_config_item = std::make_shared<OSDConfigResult>();
		  osd_config_item->index = i;
		  osd_config_result_list_->emplace_back(osd_config_item);
	  }
  }
  osd_config_dialog_.SetConfigResultList(osd_config_result_list_);
  if (osd_config_dialog_.DoModal() == IDOK) {
    auto result = osd_config_dialog_.GetConfigResultList();
    auto osd_param = std::make_shared<output::OSDParamList>();
    for (auto i = 0U; i < result->size(); ++i) {
      auto item = (*result)[i];
      auto& param_item = (*osd_param)[item->index];
      param_item.height = item->height;
      param_item.width = item->width;
      param_item.x_pos = item->x_pos;
      param_item.y_pos = item->y_pos;
      param_item.enable = item->enable;
	    param_item.content = utils::CharacterSetConvertor::Instance()->UnicodeToUTF8(item->content);
    }
    render_file_player_->OSD(osd_param);
  }
}


void TestSuiteGUIDialog::OnRenderImageratioAdpater() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_ADPATER);
	render_file_player_->ImageScale(output::AspectRatio::kAdapter);
}


void TestSuiteGUIDialog::OnRenderImageratio43() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_43);
	render_file_player_->ImageScale(output::AspectRatio::kRatio43);
}


void TestSuiteGUIDialog::OnRenderImageratio169() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_169);
	render_file_player_->ImageScale(output::AspectRatio::kRatio169);
}

void TestSuiteGUIDialog::MutexPictureImageRatioMenuItems(unsigned ui_id) {
  auto menu = GetMenu();
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_ADPATER, (ui_id == ID_RENDER_IMAGERATIO_ADPATER ? MF_CHECKED : MF_UNCHECKED));
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_43, (ui_id == ID_RENDER_IMAGERATIO_43 ? MF_CHECKED : MF_UNCHECKED));
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_169, (ui_id == ID_RENDER_IMAGERATIO_169 ? MF_CHECKED : MF_UNCHECKED));
  UpdateData(FALSE);
}

void TestSuiteGUIDialog::OnSizing(UINT fwSide, LPRECT pRect) {
	BaseAutoLayoutDialog::OnSizing(fwSide, pRect);
	GetControlLayoutInfo(IDC_STATIC_MAIN, &screen_rect_);
	//if (video_output_media_source_ != nullptr) {
	//	video_output_media_source_->ResizeWindow();
	//}
}

void TestSuiteGUIDialog::OnRenderImageratioROI() {
  const auto status = render_file_player_->IsZoom();
  auto menu = GetMenu();
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_ROI, !status ? MF_CHECKED : MF_UNCHECKED);
  mouse_locator_.Enable(!status);
  if (status) {
    render_file_player_->Zoom(nullptr);
  }
}

void TestSuiteGUIDialog::OnMouseMove(UINT nFlags, CPoint point) {
  CDialogEx::OnMouseMove(nFlags, point);
  const auto temp = point - screen_rect_.TopLeft();
  mouse_locator_.Update(temp, [this, point]() {
    POINT current_pos{point.x, point.y};
    return render_file_player_->IsValidRegion(current_pos);
  });
}

void TestSuiteGUIDialog::OnLButtonUp(UINT nFlags, CPoint point) {
  CDialogEx::OnLButtonUp(nFlags, point);
  POINT current_pos{ point.x, point.y };
  if (!render_file_player_->IsValidRegion(current_pos)) {
	  return;
  }
  point  -= screen_rect_.TopLeft();
  mouse_locator_.Stop(point);
  mouse_locator_.Transform([this](const CPoint& left_top, const CPoint& right_bottom) {
    if (left_top == right_bottom) {
		  return;
    }
    auto rect = std::make_shared<RECT>();
	  rect->left = (left_top.x * render_file_player_->Width() / screen_rect_.Width());
	  rect->top = (left_top.y * render_file_player_->Height() / screen_rect_.Height());
	  rect->right = (right_bottom.x * render_file_player_->Width() / screen_rect_.Width());
	  rect->bottom = (right_bottom.y * render_file_player_->Height() / screen_rect_.Height());
	  render_file_player_->Zoom(rect);
  });
}

void TestSuiteGUIDialog::OnLButtonDown(UINT nFlags, CPoint point) {
  CDialogEx::OnLButtonDown(nFlags, point);
  point -= screen_rect_.TopLeft();
  mouse_locator_.Start(point);  
}

void TestSuiteGUIDialog::OnLButtonDblClk(UINT nFlags, CPoint point) {
  CDialogEx::OnLButtonDblClk(nFlags, point);
  if (render_file_player_->IsZoom()) {
    OnRenderImageratioROI();
  }
}


void TestSuiteGUIDialog::OnHandlerMosaic() {
	//mosaic_check_status_ = !mosaic_check_status_;
	//auto menu = GetMenu();
	//menu->CheckMenuItem(ID_HANDLER_MOSAIC, roi_check_status_ ? MF_CHECKED : MF_UNCHECKED);
	//if (mosaic_handler_ != nullptr) {
	//	mosaic_handler_->EnableMosaic(mosaic_check_status_);
	//	is_mosaic_playing_ = true;
	//}
}
