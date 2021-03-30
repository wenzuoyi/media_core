#include "stdafx.h"
#include "mouse_locator.h"

MouseLocator::MouseLocator() {
	cross_style_cursor_ = LoadCursor(NULL, IDC_CROSS);
	arrow_style_cursor_ = LoadCursor(NULL, IDC_ARROW);
	pen_.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	empty_brush_ = CBrush::FromHandle(static_cast<HBRUSH>(GetStockObject(NULL_BRUSH)));
}

MouseLocator::~MouseLocator() {
	DestroyCursor(cross_style_cursor_);
	DestroyCursor(arrow_style_cursor_);
}

inline int MouseLocator::MinValue(int left, int right) {
  return left < right ? left : right;
}

inline int MouseLocator::MaxValue(int left, int right) {
  return left > right ? left : right;
}

void MouseLocator::Enable(bool enable) {
  enable_ = enable;
  if (!enable) {
	  visible_ = false;
  }
}

bool MouseLocator::IsEnable() const {
  return enable_;
}

void MouseLocator::Visible(bool visible) {
	visible_ = visible;
}

bool MouseLocator::IsVisible() const {
  return visible_;
}

void MouseLocator::Start(const CPoint& point) {
  if (enable_ && !visible_) {
    begin_point_ = end_point_ = point;
    visible_ = true;
	  SetCursor(cross_style_cursor_);
  }
}

void MouseLocator::Update(const CPoint& point, std::function<bool()> condition) {
  if (enable_ && visible_) {
    end_point_ = point;
    if (condition()) {     
      SetCursor(cross_style_cursor_);
    } else {
      SetCursor(arrow_style_cursor_);
    }
  }
}

void MouseLocator::Stop(const CPoint& point) {
  if (enable_ && visible_) {
	  visible_ = false;
	  SetCursor(arrow_style_cursor_);
  }
}

void MouseLocator::Draw(HDC hdc, std::function<CRect(const CPoint& left_top, const CPoint& right_bottom)> transformer) const {
  if (enable_ && visible_) {
    auto client_dc = CDC::FromHandle(hdc);
    client_dc->SelectObject(pen_);
    auto previous_brush = client_dc->SelectObject(empty_brush_);
    auto previous_mode = client_dc->SetROP2(R2_NOTXORPEN);
    CPoint left_top_corner(MinValue(begin_point_.x, end_point_.x), MinValue(begin_point_.y, end_point_.y));
    CPoint right_bottom_corner(MaxValue(begin_point_.x, end_point_.x), MaxValue(begin_point_.y, end_point_.y));
    auto rect = transformer(left_top_corner, right_bottom_corner);
    client_dc->Rectangle(&rect);
    client_dc->SelectObject(previous_brush);
    client_dc->SetROP2(previous_mode);
  }
}

void MouseLocator::Transform(std::function<void(const CPoint& left_top, const CPoint& right_bottom)> transformer) const {
  if (enable_) {
    CPoint left_top_corner(MinValue(begin_point_.x, end_point_.x), MinValue(begin_point_.y, end_point_.y));
    CPoint right_bottom_corner(MaxValue(begin_point_.x, end_point_.x), MaxValue(begin_point_.y, end_point_.y));
    transformer(left_top_corner, right_bottom_corner);
  }
}