#include "stdafx.h"
#include "TestSuiteGUI.h"
#include "render_file_baseinfo_setting.h"
#include "afxdialogex.h"
#include <sstream>
IMPLEMENT_DYNAMIC(RenderFileBaseInfoSetting, CDialogEx)

RenderFileBaseInfoSetting::RenderFileBaseInfoSetting(CWnd* pParent /*=NULL*/) : BaseAutoLayoutDialog(IDD_RENDERFILESETTING_DIALOG, pParent), width_(0), height_(0), enable_loop_playback_(FALSE) {
}

RenderFileBaseInfoSetting::~RenderFileBaseInfoSetting() {
}

std::string RenderFileBaseInfoSetting::GetResolution() const {
	return resolution_;
}

core::RenderFormat RenderFileBaseInfoSetting::GetFormat() const {
  return format_;
}

bool RenderFileBaseInfoSetting::EnableLoopPlayback() const {
	return enable_loop_playback_ == TRUE;
}

BOOL RenderFileBaseInfoSetting::OnInitDialog() {
  const auto retvalue = BaseAutoLayoutDialog::OnInitDialog();
  ui_combox_format_.AddString(L"YUV420P");
  ui_combox_format_.AddString(L"YUV422P");
  ui_combox_format_.SetCurSel(0);
  width_ = 320;
  height_ = 180;
  enable_loop_playback_ = TRUE;
  UpdateData(FALSE);
  return retvalue;
}

void RenderFileBaseInfoSetting::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FORMAT, ui_combox_format_);
	DDX_Text(pDX, IDC_EDIT_WIDTH, width_);
	DDV_MinMaxInt(pDX, width_, 0, 1920);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, height_);
	DDV_MinMaxInt(pDX, height_, 0, 1080);
	DDX_Check(pDX, IDC_CHECK_ENABLELOOP, enable_loop_playback_);
}

BEGIN_MESSAGE_MAP(RenderFileBaseInfoSetting, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &RenderFileBaseInfoSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CANCEL, &RenderFileBaseInfoSetting::OnBnClickedCancel)
END_MESSAGE_MAP()

void RenderFileBaseInfoSetting::OnBnClickedOk() {
	UpdateData();
	std::stringstream oss;
	oss << width_ << "x" << height_;
	resolution_ = oss.str();
	const auto selected_item = ui_combox_format_.GetCurSel();	
	if (selected_item == 0) {
		format_ = core::RenderFormat::kYUV420;
	} else if (selected_item == 1) {
		format_ = core::RenderFormat::kYUV422;
	}	
	BaseAutoLayoutDialog::OnOK();
}

void RenderFileBaseInfoSetting::OnBnClickedCancel() {
	BaseAutoLayoutDialog::OnCancel();
}
