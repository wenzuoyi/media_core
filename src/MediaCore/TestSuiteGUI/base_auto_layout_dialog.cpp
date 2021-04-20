#include "stdafx.h"
#include "base_auto_layout_dialog.h"

#include <memory>

BaseAutoLayoutDialog::BaseAutoLayoutDialog(UINT nIDTemplate, CWnd *pParen) : CDialogEx(nIDTemplate, pParen) {
}


BaseAutoLayoutDialog::~BaseAutoLayoutDialog() {
}

bool BaseAutoLayoutDialog::GetControlLayoutInfo(int control_id, CRect* rect) {
  if (anchor_base_map_.find(control_id) == anchor_base_map_.end()) {
	  return false;
  }
  GetDlgItem(control_id)->GetWindowRect(rect);
  ScreenToClient(rect);
  return true;
}

BOOL BaseAutoLayoutDialog::OnInitDialog() {
	auto retvalue = CDialogEx::OnInitDialog();
  if (!retvalue) {
	  return false;
  }
  Init();
	return retvalue;
}

void BaseAutoLayoutDialog::OnSizing(UINT fwSide, LPRECT pRect) {
	CDialogEx::OnSizing(fwSide, pRect);
	Resize();
}

void BaseAutoLayoutDialog::Init() {
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

void BaseAutoLayoutDialog::Resize() {
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