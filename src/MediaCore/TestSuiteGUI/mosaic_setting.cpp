#include "stdafx.h"
#include "TestSuiteGUI.h"
#include "mosaic_setting.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(MosaicSetting, CDialogEx)

MosaicSetting::MosaicSetting(CWnd* pParent /*=NULL*/) : BaseAutoLayoutDialog(IDD_MOSAICSETTING_DIALOG, pParent), x_(50), y_(50), width_(50), height_(50) {
}

MosaicSetting::~MosaicSetting() {
}

core::RegionPtr MosaicSetting::GetMosaicRegion() const {
	return region_;
}

BOOL MosaicSetting::OnInitDialog() {
	const auto retvalue = BaseAutoLayoutDialog::OnInitDialog();
	region_ = std::make_shared<core::Region>();
  if (region_ == nullptr) {
	  return false;
  }
	return retvalue;
}

void MosaicSetting::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT_X, x_);
  DDV_MinMaxInt(pDX, x_, 0, 1920);
  DDX_Text(pDX, IDC_EDIT_Y, y_);
  DDV_MinMaxInt(pDX, y_, 0, 1080);
  DDX_Text(pDX, IDC_EDIT_WIDTH, width_);
  DDV_MinMaxInt(pDX, width_, 0, 1920);
  DDX_Text(pDX, IDC_EDIT_HEIGHT, height_);
  DDV_MinMaxInt(pDX, height_, 0, 1080);
}

BEGIN_MESSAGE_MAP(MosaicSetting, CDialogEx)
ON_BN_CLICKED(IDOK, &MosaicSetting::OnBnClickedOk)
END_MESSAGE_MAP()

void MosaicSetting::OnBnClickedOk() {
	UpdateData();
	region_->left = x_;
	region_->top = y_;
	region_->right = x_ + width_;
	region_->bottom = y_ + height_;
  CDialogEx::OnOK();
}
