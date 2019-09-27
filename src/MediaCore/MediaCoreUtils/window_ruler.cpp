#include "include/window_ruler.h"
#include "window_ruler_impl.h"
namespace utils {
  WindowRulerPtr WindowRuler::CreateInstance() {
	  return std::make_shared<WindowRulerImpl>();
  }
}

