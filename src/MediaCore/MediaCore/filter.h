#ifndef FILTER_H_
#define FILTER_H_
#include <memory>
struct BaseFilter {
	void* user_data;
};
using BaseFilterPtr = std::shared_ptr<BaseFilter>;

struct BaseVideoFilter : public BaseFilter{
	int channel_id;
};
using BaseVideoPlayerFilterPtr = std::shared_ptr<BaseVideoFilter>;
#endif // FILTER_H_
