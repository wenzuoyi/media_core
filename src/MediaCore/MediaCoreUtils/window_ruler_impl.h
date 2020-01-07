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
    static inline auto Ratio(const int data1, const int data2) -> double { return static_cast<double>(data1) / static_cast<double>(data2); }
		void CalculateFromSize();
		int window_width_{ 0 };
		int window_height_{ 0 };
    bool enable_{true};
		RECT result_;
		std::pair<int, int> ratio_;
	};
}

