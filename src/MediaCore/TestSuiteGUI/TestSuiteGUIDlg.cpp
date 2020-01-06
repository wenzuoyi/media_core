// TestSuiteGUIDlg.cpp : 实现文件
//
#include "stdafx.h"
#include <vector>
#include <sstream>
#include <chrono>
#include "TestSuiteGUI.h"
#include "TestSuiteGUIDlg.h"
#include "character_set_convertor.h"
#include "afxdialogex.h"
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

const int TestSuiteGUIDialog::VIDEO_WIDTH = 320;
const int TestSuiteGUIDialog::VIDEO_HEIGHT = 180;

TestSuiteGUIDialog::TestSuiteGUIDialog(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_TESTSUITEGUI_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TestSuiteGUIDialog::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STATIC_MAIN, display_area_);
}

BEGIN_MESSAGE_MAP(TestSuiteGUIDialog, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_CREATE()
  ON_WM_DESTROY()
  ON_COMMAND(ID_RENDER_OPEN, &TestSuiteGUIDialog::OnRenderOpenFile)
  ON_COMMAND(ID_RENDER_CLOSE, &TestSuiteGUIDialog::OnRenderCloseFile)
  ON_COMMAND(ID_RENDER_PLAY, &TestSuiteGUIDialog::OnRenderPlay)
  ON_COMMAND(ID_RENDER_STOP, &TestSuiteGUIDialog::OnRenderStop)
	ON_COMMAND(ID_RENDER_OSD, &TestSuiteGUIDialog::OnRenderOSDConfig)
	ON_COMMAND(ID_RENDER_IMAGERATIO_ADPATER, &TestSuiteGUIDialog::OnRenderImageratioAdpater)
	ON_COMMAND(ID_RENDER_IMAGERATIO_43, &TestSuiteGUIDialog::OnRenderImageratio43)
	ON_COMMAND(ID_RENDER_IMAGERATIO_169, &TestSuiteGUIDialog::OnRenderImageratio169)
	ON_WM_SIZING()
END_MESSAGE_MAP()

void TestSuiteGUIDialog::OnVideoOutputMediaExceptionEvent(unsigned error_code) {
}

void TestSuiteGUIDialog::OnCustomPainting(HDC hdc) {
}

void TestSuiteGUIDialog::OnTransmitDataEvent(output::VideoFramePtr video_frame) {
}

BOOL TestSuiteGUIDialog::OnInitDialog() {
  CDialogEx::OnInitDialog();
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
  if (video_output_media_source_ != nullptr) {
	  video_output_media_source_->Init();
	  auto video_param = std::make_shared<output::VideoOutputParam>();
	  video_param->width = VIDEO_WIDTH;
	  video_param->height = VIDEO_HEIGHT;
	  video_param->render_wnd = display_area_.m_hWnd;
	  video_output_media_source_->SetVideoOutputMediaParam(video_param);
  }
  EnableRenderMenuItem({
    {ID_RENDER_OPEN, true}, {ID_RENDER_PLAY, false}, {ID_RENDER_STOP, false}, {ID_RENDER_CLOSE, false}
  });
  EnableRenderMenuItem({
    {ID_RENDER_OSD, false}, {ID_RENDER_IMAGERATIO_ADPATER, false}, { ID_RENDER_IMAGERATIO_43, false},
    {ID_RENDER_IMAGERATIO_169, false}, {ID_RENDER_IMAGERATIO_ROI, false}
  });
  InitControlAnchorsBaseInfo();
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
		  UpdateControlAnchorsInfo();
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

void TestSuiteGUIDialog::StartReadMediaFile() {
  exit_ = false;
  read_file_task_ = std::async(std::launch::async, [this]() {
	  ifs_.seekg(0, std::ios_base::end);
	  auto left_read_bytes = ifs_.tellg();
	  ifs_.seekg(0, std::ios_base::beg);
    while (!exit_) {
      std::vector<char> buffer(GetYUVFrameSize(), 0);
      ifs_.read(&buffer[0], buffer.size());
      if (ifs_.fail()) {
        return;
      }
      if (ifs_.gcount() != buffer.size()) {
        buffer.resize(ifs_.gcount());
      }
      left_read_bytes -= buffer.size();
      PostVideoFrame(buffer);
      std::this_thread::sleep_for(40ms);
      if (left_read_bytes <= 0) {
        left_read_bytes = ifs_.tellg();
        ifs_.seekg(0, std::ios_base::beg);
      }
    }
  });
}

void TestSuiteGUIDialog::StopReadFile() {
  exit_ = true;
  read_file_task_.wait();
}

void TestSuiteGUIDialog::PostVideoFrame(const std::vector<char>& buffer) const {
  auto offset = 0;
  const auto y_stride = VIDEO_WIDTH;
  const auto y_size = (y_stride * VIDEO_HEIGHT);
  auto y_data = std::make_shared<output::Buffer>(buffer.begin() + offset, buffer.begin() + offset + y_size);
  offset += y_size;
  const auto u_stride = y_stride / 2;
  const auto u_size = (u_stride * VIDEO_HEIGHT / 2);
  auto u_data = std::make_shared<output::Buffer>(buffer.begin() + offset, buffer.begin() + offset + u_size);
  offset += u_size;
  const auto v_stride = y_stride / 2;
  const auto v_size = (v_stride * VIDEO_HEIGHT / 2);
  auto v_data = std::make_shared<output::Buffer>(buffer.begin() + offset, buffer.begin() + offset + v_size);
  auto video_frame = std::make_shared<output::VideoFrame>();
  video_frame->width = VIDEO_WIDTH;
  video_frame->height = VIDEO_HEIGHT;
  video_frame->data[0] = y_data;
  video_frame->line_size[0] = y_stride;
  video_frame->data[1] = u_data;
  video_frame->line_size[1] = u_stride;
  video_frame->data[2] = v_data;
  video_frame->line_size[2] = v_stride;
  if (video_output_media_source_ != nullptr) {
    video_output_media_source_->InputVideoFrame(video_frame);
  }
}

int TestSuiteGUIDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) {
  if (CDialogEx::OnCreate(lpCreateStruct) == -1)
    return -1;
  video_output_media_source_ = output::VideoOutputMediaSource::CreateInstance(output::RenderMode::kD3D);
  if (video_output_media_source_ == nullptr) {
    return -1;
  }
  video_output_media_source_->SetEvent(this);
  return 0;
}

void TestSuiteGUIDialog::OnDestroy() {
  if (video_output_media_source_ != nullptr) {
    video_output_media_source_->Fini();
    video_output_media_source_ = nullptr;
  }
  CDialogEx::OnDestroy();
}


void TestSuiteGUIDialog::OnRenderOpenFile() {
  CFileDialog file_dialog(TRUE, _T("*.yuv"), nullptr, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
                          _T("渲染文件(*.yuv)|*.yuv| All Files (*.*) |*.*||"), nullptr); // 打开文件对话框的标题名
  file_dialog.m_ofn.lpstrTitle = _T("选择渲染文件");
  if (file_dialog.DoModal() == IDOK) {
	  const auto open_file_path = file_dialog.GetPathName();
    auto open_file_path_string = std::wstring(static_cast<LPCTSTR>(open_file_path));
	  ifs_.open(open_file_path_string, std::ios_base::binary);
	  if (ifs_.fail()) {
	    AfxMessageBox(L"打开文件失败!");
	  }
	  EnableRenderMenuItem({
		  { ID_RENDER_OPEN, false },{ ID_RENDER_PLAY, true },{ ID_RENDER_STOP, false },{ ID_RENDER_CLOSE, true }
	  });
	  EnableRenderMenuItem({
		  { ID_RENDER_OSD, false },{ ID_RENDER_IMAGERATIO_ADPATER, false },{ ID_RENDER_IMAGERATIO_43, false },
		  { ID_RENDER_IMAGERATIO_169, false },{ ID_RENDER_IMAGERATIO_ROI, false }
	  });
  }
}

void TestSuiteGUIDialog::OnRenderCloseFile() {
	ifs_.close();
	EnableRenderMenuItem({
		{ ID_RENDER_OPEN, true },{ ID_RENDER_PLAY, false },{ ID_RENDER_STOP, false },{ ID_RENDER_CLOSE, false }
	});
	EnableRenderMenuItem({
		{ ID_RENDER_OSD, false },{ ID_RENDER_IMAGERATIO_ADPATER, false },{ ID_RENDER_IMAGERATIO_43, false },
		{ ID_RENDER_IMAGERATIO_169, false },{ ID_RENDER_IMAGERATIO_ROI, false }
	});
}

void TestSuiteGUIDialog::OnRenderPlay() {
	if (video_output_media_source_ == nullptr) {
	  return;
	}
	if (!is_playing_) {
	  if (video_output_media_source_->Play()) {
	    StartReadMediaFile();
	    is_playing_ = true;
      EnableRenderMenuItem({
        {ID_RENDER_OPEN, false}, {ID_RENDER_PLAY, false}, {ID_RENDER_STOP, true}, {ID_RENDER_CLOSE, false}
      });
      EnableRenderMenuItem({
        {ID_RENDER_OSD, true}, {ID_RENDER_IMAGERATIO_ADPATER, true }, {ID_RENDER_IMAGERATIO_43, true },
        {ID_RENDER_IMAGERATIO_169, true }, {ID_RENDER_IMAGERATIO_ROI, true }
      });
	  }
	}
}

void TestSuiteGUIDialog::OnRenderStop() {
	if (video_output_media_source_ == nullptr) {
		return;
	}
  if (is_playing_) {
    StopReadFile();
    video_output_media_source_->Stop();
    is_playing_ = false;
    EnableRenderMenuItem({
      {ID_RENDER_OPEN, false}, {ID_RENDER_PLAY, true}, {ID_RENDER_STOP, false}, {ID_RENDER_CLOSE, true}
    });
    EnableRenderMenuItem({
      {ID_RENDER_OSD, false}, {ID_RENDER_IMAGERATIO_ADPATER, false }, {ID_RENDER_IMAGERATIO_43, false },
      {ID_RENDER_IMAGERATIO_169, false }, {ID_RENDER_IMAGERATIO_ROI, false }
    });
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
  if (osd_config_dialog_.DoModal() == IDOK && video_output_media_source_ != nullptr) {
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
    video_output_media_source_->SetOSD(osd_param);
  }
}


void TestSuiteGUIDialog::OnRenderImageratioAdpater() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_ADPATER);
	video_output_media_source_->SetDisplayRatio(output::DisplayRatio::kAdapter);
}


void TestSuiteGUIDialog::OnRenderImageratio43() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_43);
	video_output_media_source_->SetDisplayRatio(output::DisplayRatio::kRatio43);
}


void TestSuiteGUIDialog::OnRenderImageratio169() {
	MutexPictureImageRatioMenuItems(ID_RENDER_IMAGERATIO_169);
	video_output_media_source_->SetDisplayRatio(output::DisplayRatio::kRatio169);
}

void TestSuiteGUIDialog::MutexPictureImageRatioMenuItems(unsigned ui_id) {
  auto menu = GetMenu();
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_ADPATER, (ui_id == ID_RENDER_IMAGERATIO_ADPATER ? MF_CHECKED : MF_UNCHECKED));
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_43, (ui_id == ID_RENDER_IMAGERATIO_43 ? MF_CHECKED : MF_UNCHECKED));
  menu->CheckMenuItem(ID_RENDER_IMAGERATIO_169, (ui_id == ID_RENDER_IMAGERATIO_169 ? MF_CHECKED : MF_UNCHECKED));
  UpdateData(FALSE);
}


void TestSuiteGUIDialog::OnSizing(UINT fwSide, LPRECT pRect) {
	CDialogEx::OnSizing(fwSide, pRect);
	UpdateControlAnchorsInfo();
}

void TestSuiteGUIDialog::InitControlAnchorsBaseInfo() {
  CRect origin_window_size;
  GetClientRect(&origin_window_size);
  const auto origin_window_width = origin_window_size.Width();
  const auto origin_window_height = origin_window_size.Height();
  auto sub_window_handler = ::GetWindow(m_hWnd, GW_CHILD);
  while (sub_window_handler != nullptr) {
    CRect child_control_rect;
    const auto child_control_id = ::GetDlgCtrlID(sub_window_handler);
    GetDlgItem(child_control_id)->GetWindowRect(child_control_rect);
    ScreenToClient(child_control_rect);
    auto anchor_base_info = std::make_shared<AnchorBaseInfo>();
    anchor_base_map_.insert(std::make_pair(child_control_id, anchor_base_info));
    anchor_base_info->left = static_cast<double>(child_control_rect.left) / static_cast<double>(origin_window_width);
    anchor_base_info->top = static_cast<double>(child_control_rect.top) / static_cast<double>(origin_window_height);
    anchor_base_info->right = static_cast<double>(child_control_rect.right) / static_cast<double>(origin_window_width);
    anchor_base_info->bottom = static_cast<double>(child_control_rect.bottom) / static_cast<double>(origin_window_height);
    sub_window_handler = ::GetWindow(sub_window_handler, GW_HWNDNEXT);
  }
}

void TestSuiteGUIDialog::UpdateControlAnchorsInfo() {
	CRect current_layout;
	GetClientRect(&current_layout);
	const auto width = static_cast<double>(current_layout.Width());
	const auto height = static_cast<double>(current_layout.Height());
  auto sub_control_handler = ::GetWindow(m_hWnd, GW_CHILD);
	while (sub_control_handler != nullptr) {
		auto child_control_id = ::GetDlgCtrlID(sub_control_handler);
		auto origin_rect = anchor_base_map_[child_control_id];
		CRect  target_rect;
		target_rect.left = static_cast<long>(origin_rect->left * width);
		target_rect.top = static_cast<long>(origin_rect->top * height);
		target_rect.right = static_cast<long>(origin_rect->right * width);
		target_rect.bottom = static_cast<long>(origin_rect->bottom* height);
		GetDlgItem(child_control_id)->MoveWindow(target_rect, TRUE);
		GetDlgItem(child_control_id)->Invalidate();
		sub_control_handler = ::GetWindow(sub_control_handler, GW_HWNDNEXT);
	}
}


