#ifndef BASE_PLAYER_DATATYPE_H_
#define BASE_PLAYER_DATATYPE_H_
#include <memory>
namespace core {
  struct Region {
	  int top;
	  int left;
	  int right;
	  int bottom;
  };
  using RegionPtr = std::shared_ptr<Region>;
}
#endif // BASE_PLAYER_DATATYPE_H_
