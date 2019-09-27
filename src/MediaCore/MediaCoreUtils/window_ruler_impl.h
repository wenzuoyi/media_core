#pragma once
#include "include/window_ruler.h"
namespace utils {
	class WindowRulerImpl : 	public WindowRuler {
	public:
		WindowRulerImpl() = default;
		virtual ~WindowRulerImpl() = default;
	protected:
    void SetWindowSize(int width, int height) override;
    void SetAspectRatio(const std::pair<int, int>& ratio) override;
	  void EnableAutoAdaptFrom(bool enable);
	  bool IsAutoAdaptFrom() const;
    RECT GetRendingArea() const override;
	private:
		void CalculateFromSize();
		int window_width_{ 0 };
		int window_height_{ 0 };
    bool enable_{true};
		RECT result_;
		std::pair<int, int> ratio_;
	};
}

