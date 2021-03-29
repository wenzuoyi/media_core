#include "stdafx.h"
#include "mouse_locator.h"

MouseLocator::MouseLocator() {
}

MouseLocator::~MouseLocator() {
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
  if (enable_) {
    begin_point_ = end_point_ = point;
  }
}

void MouseLocator::Update(const CPoint& point) {
  if (enable_) {
    end_point_ = point;
  }
}

void MouseLocator::Transform(std::function<void(const CPoint& left_top, const CPoint& right_bottom)> transformer) const {
  if (!enable_) {
	  return;
  }
  CPoint left_top_corner(MinValue(begin_point_.x, end_point_.x), MinValue(begin_point_.y, end_point_.y));
  CPoint right_bottom_corner(MaxValue(begin_point_.x, end_point_.x), MaxValue(begin_point_.y, end_point_.y));
  transformer(left_top_corner, right_bottom_corner);
}

CRect MouseLocator::GetRegion() const {
	if (!enable_) {
		return CRect();
	}
  CPoint left_top_corner(MinValue(begin_point_.x, end_point_.x), MinValue(begin_point_.y, end_point_.y));
  CPoint right_bottom_corner(MaxValue(begin_point_.x, end_point_.x), MaxValue(begin_point_.y, end_point_.y));
  CRect rect(left_top_corner.x, left_top_corner.y, right_bottom_corner.x, right_bottom_corner.y);
  return rect;
}
