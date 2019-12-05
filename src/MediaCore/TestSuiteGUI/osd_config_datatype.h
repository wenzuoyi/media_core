#ifndef OSD_CONFIG_DATATYPE_H_
#define OSD_CONFIG_DATATYPE_H_
#include <string>
#include <vector>
#include <memory>
struct OSDConfigResult {
	unsigned index {0};
	bool enable {false};
	int x_pos {0};
	int y_pos {0};
	unsigned width{ 0 };
	unsigned height{ 0 };
	std::wstring content;
};
using OSDConfigResultPtr = std::shared_ptr<OSDConfigResult>;
using OSDConfigResultList = std::vector<OSDConfigResultPtr>;
using OSDConfigResultListPtr = std::shared_ptr<OSDConfigResultList>;
#endif // OSD_CONFIG_DATATYPE_H_
