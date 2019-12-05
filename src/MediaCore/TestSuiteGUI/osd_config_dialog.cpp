#include "stdafx.h"
#include <sstream>
#include "TestSuiteGUI.h"
#include "osd_config_dialog.h"
#include "afxdialogex.h"
IMPLEMENT_DYNAMIC(OSDConfigDialog, CDialogEx)

OSDConfigDialog::OSDConfigDialog(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_ADDOSDCONTENT_DIALOG, pParent), content(_T("")) {
}

OSDConfigDialog::~OSDConfigDialog() {
}

void OSDConfigDialog::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OSD_IDS_COMBO, osd_ids_);
	DDX_Control(pDX, IDC_OSD_XPOS_SPIN, xpos_spin_);
	DDX_Control(pDX, IDC_OSD_YPOS_SPIN, ypos_spin_);
	DDX_Text(pDX, IDC_OSD_XPOS_EDIT, x_pos_);
	DDX_Text(pDX, IDC_OSD_YPOS_EDIT, y_pos_);
	DDX_Check(pDX, IDC_OSD_ENABLE_CHECK, osd_enable_);

	DDX_Text(pDX, IDC_OSD_CONTENT_EDIT, content);
	DDX_Text(pDX, IDC_OSD_WIDTH_EDIT, width_);
	DDX_Control(pDX, IDC_OSD_WIDTH_SPIN, width_spin_);
	DDX_Text(pDX, IDC_OSD_HEIGHT_EDIT, height_);
	DDX_Control(pDX, IDC_OSD_HEIGHT_SPIN, height_spin_);
}

BEGIN_MESSAGE_MAP(OSDConfigDialog, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &OSDConfigDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, &OSDConfigDialog::OnBnClickedApplyButton)
	ON_BN_CLICKED(IDC_CANCEL, &OSDConfigDialog::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_OSD_IDS_COMBO, &OSDConfigDialog::OnCbnSelchangeOsdIdsCombox)
	ON_BN_CLICKED(IDC_OSD_ENABLE_CHECK, &OSDConfigDialog::OnBnClickedOsdEnableCheck)
END_MESSAGE_MAP()

BOOL OSDConfigDialog::OnInitDialog() {
  if (!CDialogEx::OnInitDialog()) {
    return FALSE;
  }
  if (osd_config_result_list_ != nullptr) {
    for (auto item : *osd_config_result_list_) {
      std::wostringstream woss;
      woss << item->index;
      auto temp_ids = woss.str();
      osd_ids_.AddString(temp_ids.c_str());
    }
  }
  osd_ids_.SetCurSel(0);
  x_pos_ = y_pos_ = 0;
  xpos_spin_.SetRange32(0, 1920);
  ypos_spin_.SetRange32(0, 1080);
  width_ = height_ = 0;
  width_spin_.SetRange32(0, 1920);
  height_spin_.SetRange32(0, 1080);
  return TRUE;
}

OSDConfigResultListPtr OSDConfigDialog::GetConfigResultList() const {
	return osd_config_result_list_;
}

void OSDConfigDialog::SetConfigResultList(OSDConfigResultListPtr osd_config_result) {
  if (osd_config_result != nullptr && osd_config_result != osd_config_result_list_) {
	  osd_config_result_list_ = osd_config_result;
  }
}

void OSDConfigDialog::OnBnClickedOk() {
	UpdateData();
	AssignItemValue();
	CDialogEx::OnOK();
}


void OSDConfigDialog::OnBnClickedApplyButton() {
	UpdateData();
	AssignItemValue();
}

void OSDConfigDialog::AssignItemValue() const {
  if (osd_config_result_list_ == nullptr) {
	  return;
  }
	auto current_select = osd_ids_.GetCurSel();
	auto item = osd_config_result_list_->at(current_select);
	item->enable = (osd_enable_ != 0);
	item->x_pos = x_pos_;
	item->y_pos = y_pos_;
	item->width = width_;
	item->height = height_;
	item->content = static_cast<LPCTSTR>(content);
}

void OSDConfigDialog::OnBnClickedCancel() {
	UpdateData();
	CDialogEx::OnCancel();
}

void OSDConfigDialog::OnCbnSelchangeOsdIdsCombox() {
	auto current_select = osd_ids_.GetCurSel();
  if (current_select < osd_config_result_list_->size()) {
	  auto item = osd_config_result_list_->at(current_select);
	  osd_enable_ = item->enable;
	  x_pos_ = item->x_pos;
	  y_pos_ = item->y_pos;
	  width_ = item->width;
	  height_ = item->height;
	  content = CString(item->content.c_str());	  
	  UpdateData(FALSE);
  }
}


void OSDConfigDialog::OnBnClickedOsdEnableCheck() {
	UpdateData();
	auto current_select = osd_ids_.GetCurSel();
  if (!osd_enable_ && current_select < osd_config_result_list_->size()) {
	  auto item = osd_config_result_list_->at(current_select);
	  osd_enable_ = item->enable = false;
	  x_pos_ = item->x_pos = 0;
	  y_pos_ = item->y_pos = 0;
	  width_ = item->width = 0;
	  height_ = item->height = 0;
	  content = CString();
	  UpdateData(FALSE);
  }
}
