// rotation_setting_dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TestSuiteGUI.h"
#include "rotation_setting_dialog.h"
#include "afxdialogex.h"


// RotationSettingDialog 对话框

IMPLEMENT_DYNAMIC(RotationSettingDialog, CDialogEx)

RotationSettingDialog::RotationSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROTATION_DIALOG, pParent), enable_(FALSE), degree90_(FALSE), degree180_(FALSE), degree270_(FALSE), customize_degree_(0), degree_customize_(FALSE) {

}

RotationSettingDialog::~RotationSettingDialog()
{
}

bool RotationSettingDialog::EnableRotation() const {
  if (!enable_) {
	  return false;
  }
  if (!degree90_ && !degree180_ && !degree270_ && !degree_customize_) {
	  return false;
  }
  return true;
}

bool RotationSettingDialog::IsUseCostomizeDegree() const {
  if (!enable_) {
	  return false;
  }
  return degree_customize_ == TRUE;
}

RotationSettingDialog::RotationType RotationSettingDialog::GetRotationType() const {
  if (!enable_ || degree_customize_) {
	  return RotationType::kDegreeUnknown;
  }
  auto result = RotationType::kDegreeUnknown;
  if (degree90_) {
	  result = RotationType::kDegree90;
  } else if (degree180_) {
	  result = RotationType::kDegree180;
  } else if (degree270_) {
	  result = RotationType::kDegree270;
  }
  return  result;
}

int RotationSettingDialog::GetRotationDegree() const {
	if (!enable_ || !degree_customize_) {
		return 0;
	}
	return customize_degree_;
}

void RotationSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_ROTATION_ENABLE, enable_);
	DDX_Check(pDX, IDC_CHECK_DEGREE90, degree90_);
	DDX_Check(pDX, IDC_CHECK_DEGREE180, degree180_);
	DDX_Check(pDX, IDC_CHECK_DEGREE270, degree270_);
	DDX_Text(pDX, IDC_EDIT_DEGREE, customize_degree_);
	DDV_MinMaxInt(pDX, customize_degree_, 0, 359);
	DDX_Control(pDX, IDC_SPIN_ROTATION, rotation_spin_);
	DDX_Check(pDX, IDC_CHECK_CUSTOMERDEF, degree_customize_);
}


BEGIN_MESSAGE_MAP(RotationSettingDialog, CDialogEx)

	ON_BN_CLICKED(IDCANCEL, &RotationSettingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &RotationSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_ROTATION_ENABLE, &RotationSettingDialog::OnBnClickedCheckRotationEnable)
	ON_BN_CLICKED(IDC_CHECK_DEGREE90, &RotationSettingDialog::OnBnClickedCheckDegree90)
	ON_BN_CLICKED(IDC_CHECK_DEGREE180, &RotationSettingDialog::OnBnClickedCheckDegree180)
	ON_BN_CLICKED(IDC_CHECK_DEGREE270, &RotationSettingDialog::OnBnClickedCheckDegree270)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMERDEF, &RotationSettingDialog::OnBnClickedCheckCustomerdef)
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
	rotation_spin_.SetRange(0, 359);
	EnableControl(FALSE, { IDC_CHECK_CUSTOMERDEF, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270, IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
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

void RotationSettingDialog::EnableCheckBox(BOOL enable, const std::vector<int>& id_list) {
  for (const auto& id : id_list) {
    if (id == IDC_CHECK_DEGREE90) {
      degree90_ = enable;
    } else if (id == IDC_CHECK_DEGREE180) {
      degree180_ = enable;
    } else if (id == IDC_CHECK_DEGREE270) {
      degree270_ = enable;
    } else if (id == IDC_CHECK_CUSTOMERDEF) {
      degree_customize_ = enable;
    }
  }
  UpdateData(FALSE);
}

void RotationSettingDialog::OnBnClickedCheckRotationEnable() {
	UpdateData();
	EnableControl(enable_, { IDC_CHECK_CUSTOMERDEF, IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270, IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
	SetDlgItemText(IDC_CHECK_ROTATION_ENABLE, enable_ != TRUE ? L"开启" : L"关闭");
  if (!enable_) {
	  EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270, IDC_CHECK_CUSTOMERDEF });
	  customize_degree_ = 0;
  } else {
	  EnableControl(FALSE, {IDC_EDIT_DEGREE, IDC_SPIN_ROTATION});
  }
  UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree90() {
	UpdateData();
  EnableControl(!degree90_, { IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
  if (degree90_) {
    EnableCheckBox(FALSE, { IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270, IDC_CHECK_CUSTOMERDEF });
	  customize_degree_ = 0;
  }
  UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree180() {
	UpdateData();
	EnableControl(!degree180_, { IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
	if (degree180_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE270, IDC_CHECK_CUSTOMERDEF });
		customize_degree_ = 0;
	}
	UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckDegree270() {
	UpdateData();
	EnableControl(!degree270_, { IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
	if (degree270_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_CUSTOMERDEF });
		customize_degree_ = 0;
	}
	UpdateData(FALSE);
}


void RotationSettingDialog::OnBnClickedCheckCustomerdef() {
	UpdateData();
	EnableControl(degree_customize_, { IDC_EDIT_DEGREE, IDC_SPIN_ROTATION });
	if (degree_customize_) {
		EnableCheckBox(FALSE, { IDC_CHECK_DEGREE90, IDC_CHECK_DEGREE180, IDC_CHECK_DEGREE270 });
	}
	UpdateData(FALSE);
}
