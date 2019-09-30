// TestSuiteGUIDlg.cpp : 实现文件
//
#include "stdafx.h"
#include <vector>
#include <chrono>
#include "TestSuiteGUI.h"
#include "TestSuiteGUIDlg.h"
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

const int TestSuiteGUIDialog::VIDEO_WIDTH = 320;
const int TestSuiteGUIDialog::VIDEO_HEIGHT = 180;

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP() // TestSuiteGUIDialog 对话框
TestSuiteGUIDialog::TestSuiteGUIDialog(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_TESTSUITEGUI_DIALOG, pParent) {
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TestSuiteGUIDialog::OnVideoOutputMediaExceptionEvent(unsigned error_code) {
}

void TestSuiteGUIDialog::OnCustomPainting(HDC hdc) {
}

void TestSuiteGUIDialog::OnTransmitDataEvent(output::VideoFramePtr video_frame) {
}

void TestSuiteGUIDialog::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STATIC_MAIN, display_area_);
}

BEGIN_MESSAGE_MAP(TestSuiteGUIDialog, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

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
  ShowWindow(SW_MINIMIZE);
  if (video_output_media_source_ != nullptr) {
	  video_output_media_source_->Init();
	  auto video_param = std::make_shared<output::VideoOutputParam>();
	  video_param->width = VIDEO_WIDTH;
	  video_param->height = VIDEO_HEIGHT;
	  video_param->render_wnd = display_area_.m_hWnd;
	  video_output_media_source_->SetVideoOutputMediaParam(video_param);
  }
  return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void TestSuiteGUIDialog::OnSysCommand(UINT nID, LPARAM lParam) {
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  } else {
    CDialogEx::OnSysCommand(nID, lParam);
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

void TestSuiteGUIDialog::OnBnClickedButtonOpen() {
  ifs_.open(LR"(D:\proj\simplest_media_play\test_yuv420p_320x180.yuv)", std::ios_base::binary);
  if (ifs_.fail()) {
	  AfxMessageBox(L"打开文件失败!");
  }
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

void TestSuiteGUIDialog::OnBnClickedButtonPlayctrl() {
  if (video_output_media_source_ == nullptr) {
    return;
  }
  if (!is_playing_) {
    if (video_output_media_source_->Play()) {
      //GetDlgItem(IDC_BUTTON_PLAYCTRL)->SetWindowText(L"Stop");
      StartReadMediaFile();
      is_playing_ = true;
    }
  } else {
    //GetDlgItem(IDC_BUTTON_PLAYCTRL)->SetWindowText(L"Playing");
    StopReadFile();
    video_output_media_source_->Stop();
    is_playing_ = false;
  }
}

void TestSuiteGUIDialog::OnBnClickedButtonClose() {
  ifs_.close();
  if (ifs_.fail()) {
    AfxMessageBox(L"关闭文件失败");
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
