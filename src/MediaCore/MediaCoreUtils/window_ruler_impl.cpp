#include "window_ruler_impl.h"
namespace utils {
  void WindowRulerImpl::SetWindowSize(int width, int height) {
    if (window_width_ != width) {
      window_width_ = width;
    }
    if (window_height_ != height) {
      window_height_ = height;
    }
    CalculateFromSize();
  }

  void WindowRulerImpl::EnableAutoAdaptFrom(bool enable) {
    if (enable_ != enable) {
      enable_ = enable;
    }
  }

  bool WindowRulerImpl::IsAutoAdaptFrom() const {
	  return enable_;
  }

  void WindowRulerImpl::SetAspectRatio(const std::pair<int, int>& ratio) {
	  ratio_ = ratio;
	  enable_ = false;
	  CalculateFromSize();
  }

  RECT WindowRulerImpl::GetRendingArea() const {
    return result_;
  }

  void WindowRulerImpl::CalculateFromSize() {
    if (enable_) {
      result_ = RECT{0, 0, window_width_, window_height_};
	    return;
    }
    if (window_width_ < window_height_) {
		  result_.left = (window_width_ - window_height_ * ratio_.first / ratio_.second) / 2;
		  result_.top = 0;
		  result_.right = result_.left + (window_height_ * ratio_.first / ratio_.second);
		  result_.bottom = window_height_;
    } else {
      result_.left = 0;
      result_.top = (window_height_ - window_width_ * ratio_.second / ratio_.first) / 2;
      result_.right = window_width_;
      result_.bottom = result_.top + (window_width_ * ratio_.second / ratio_.first);
    }
  }
}
