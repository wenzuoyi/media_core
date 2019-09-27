#ifndef WINDOW_RULER_H_
#define WINDOW_RULER_H_
#include <memory>
#include <map>
#include <Windows.h>
#include "global_media_utils_source.h"
namespace utils {
	class WindowRuler;
	using WindowRulerPtr = std::shared_ptr<WindowRuler>;
	class MEDIA_CORE_UTILS_API_HEADER WindowRuler {
	public:
		WindowRuler() = default;
		virtual ~WindowRuler() = default;
		static WindowRulerPtr CreateInstance();
		virtual void SetWindowSize(int width, int height) = 0;
		virtual void EnableAutoAdaptFrom(bool enable) = 0;
		virtual bool IsAutoAdaptFrom() const = 0;
		virtual void SetAspectRatio(const std::pair<int, int>& ratio) = 0 ;
		virtual RECT GetRendingArea() const  = 0;
	};
}
#endif // WINDOW_RULER_H_
