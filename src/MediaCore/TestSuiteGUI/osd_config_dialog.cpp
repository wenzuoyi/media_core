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
END_MESSAGE_MAP()

BOOL OSDConfigDialog::OnInitDialog() {
  if (!CDialogEx::OnInitDialog()) {
    return FALSE;
  }
  osd_config_result_list_ = std::make_shared<OSDConfigResultList>();
  for (auto i = 0U; i < 8; ++i) {
    std::wostringstream woss;
    woss << i;
    auto temp_ids = woss.str();
    osd_ids_.AddString(temp_ids.c_str());
	  auto osd_config_item = std::make_shared<OSDConfigResult>();
	  osd_config_item->index = i;
	  osd_config_result_list_->emplace_back(osd_config_item);
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