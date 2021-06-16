// rotation_setting_dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSuiteGUI.h"
#include "rotation_setting_dialog.h"
#include "afxdialogex.h"


// RotationSettingDialog 对话框

IMPLEMENT_DYNAMIC(RotationSettingDialog, CDialogEx)

RotationSettingDialog::RotationSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROTATION_DIALOG, pParent), enable_(FALSE), degree90_(FALSE), degree180_(FALSE), degree270_(FALSE), degree0_(FALSE) {

}

RotationSettingDialog::~RotationSettingDialog()
{
}

bool RotationSettingDialog::EnableRotation() const {
  return enable_ == TRUE;
}

void RotationSettingDialog::SetEnableRotation(bool enable) {
	enable_ = enable;
}

RotationSettingDialog::RotationType RotationSettingDialog::GetRotationType() const {
  auto result = RotationType::kDegreeUnknown;
  if (degree0_) {
	  result = RotationType::kDegree0;
  } else if (degree90_) {
	  result = RotationType::kDegree90;
  } else if (degree180_) {
	  result = RotationType::kDegree180;
  } else if (degree270_) {
	  result = RotationType::kDegree270;
  }
  return  result;
}

void RotationSettingDialog::SetRotationType(RotationSettingDialog::RotationType rotation_type) {
  if (RotationType::kDegree0 == rotation_type) {
	  EnableCheckBox(TRUE, { IDC_CHECK_DEGREE0 }, FALSE);
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 }, FALSE);
  } else if (RotationType::kDegree90 == rotation_type) {
	  EnableCheckBox(TRUE, { IDC_CHECK_DEGREE90 }, FALSE);
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 }, FALSE);
  } else if (RotationType::kDegree180 == rotation_type) {
	  EnableCheckBox(TRUE, { IDC_CHECK_DEGREE180 }, FALSE);
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE270 }, FALSE);
  } else if (RotationType::kDegree270 == rotation_type) {
	  EnableCheckBox(TRUE, { IDC_CHECK_DEGREE270 }, FALSE);
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180 }, FALSE);
  } else {
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180,IDC_CHECK_DEGREE270 }, FALSE);
  }
}

void RotationSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ROTATION_ENABLE, enable_);
	DDX_Check(pDX, IDC_CHECK_DEGREE0, degree0_);
	DDX_Check(pDX, IDC_CHECK_DEGREE90, degree90_);
	DDX_Check(pDX, IDC_CHECK_DEGREE180, degree180_);
	DDX_Check(pDX, IDC_CHECK_DEGREE270, degree270_);
}


BEGIN_MESSAGE_MAP(RotationSettingDialog, CDialogEx)

	ON_BN_CLICKED(IDCANCEL, &RotationSettingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &RotationSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ROTATION_ENABLE, &RotationSettingDialog::OnBnClickedCheckRotationEnable)
	ON_BN_CLICKED(IDC_CHECK_DEGREE90, &RotationSettingDialog::OnBnClickedCheckDegree90)
	ON_BN_CLICKED(IDC_CHECK_DEGREE180, &RotationSettingDialog::OnBnClickedCheckDegree180)
	ON_BN_CLICKED(IDC_CHECK_DEGREE270, &RotationSettingDialog::OnBnClickedCheckDegree270)
	ON_BN_CLICKED(IDC_CHECK_DEGREE0, &RotationSettingDialog::OnBnClickedCheckDegree0)
END_MESSAGE_MAP()

void RotationSettingDialog::OnBnClickedCancel() {
	CDialogEx::OnCancel();
}


void RotationSettingDialog::OnBnClickedOk() {
	UpdateData();
	CDialogEx::OnOK();
}


BOOL RotationSettingDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();	
	EnableControl(enable_, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 });
	UpdateData(FALSE);
	return TRUE;  
}

void RotationSettingDialog::EnableControl(BOOL enable, const std::vector<int>& id_list) {
	for (const auto& id : id_list) {
    auto item = GetDlgItem(id);
		item->EnableWindow(enable);
	}
	UpdateData(FALSE);
}

void RotationSettingDialog::EnableCheckBox(BOOL enable, const std::vector<int>& id_list, BOOL invalid ) {
  for (const auto& id : id_list) {
    if (id == IDC_CHECK_DEGREE90) {
      degree90_ = enable;
    } else if (id == IDC_CHECK_DEGREE180) {
      degree180_ = enable;
    } else if (id == IDC_CHECK_DEGREE270) {
      degree270_ = enable;
    } else if (id == IDC_CHECK_DEGREE0) {
      degree0_ = enable;
    }
  }
  if (invalid) {
    UpdateData(FALSE);    
  }
}

void RotationSettingDialog::OnBnClickedCheckRotationEnable() {
	UpdateData();
	EnableControl(enable_, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270});
	SetDlgItemText(IDC_CHECK_ROTATION_ENABLE, enable_ != TRUE ? L"开启" : L"关闭");
  if (enable_) {
	  EnableCheckBox(TRUE, { IDC_CHECK_DEGREE0 });
  } else {
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE0, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 });
  }
  UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree90() {
	UpdateData();
  if (degree90_) {
    EnableCheckBox(FALSE, { IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270, IDC_CHECK_DEGREE0 });
  }
  UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree180() {
	UpdateData();
	if (degree180_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE270, IDC_CHECK_DEGREE0 });
	}
	UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree270() {
	UpdateData();
	if (degree270_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE0 });
	}
	UpdateData(FALSE);
}

void RotationSettingDialog::OnBnClickedCheckDegree0() {
	UpdateData();
	if (degree0_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 });
	}
	UpdateData(FALSE);
}
