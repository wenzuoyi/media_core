#ifndef BASE_INPUT_MEDIA_SOURCE_DATATYPE_H_
#define BASE_INPUT_MEDIA_SOURCE_DATATYPE_H_
#include <vector>
#include <memory>
namespace input {
	enum class InputMediaType {
		kMP4File,
		kRenderFile
	};
	enum class VideoPackageType {
		kH264,
		kH265,
		kYUV420,
		kYUV422,
		kRGB
	};
	enum class AudioPackageType {
		kACC,
		kG711,
		kG726
	};
	using Data = std::vector<unsigned char>;
	using DataPtr = std::shared_ptr<Data>;
	struct VideoBaseInfo {
		VideoPackageType type{ VideoPackageType::kH264 };
		int width{ 720 };
		int height{ 576 };
	};
	using VideoBaseInfoPtr = std::shared_ptr<VideoBaseInfo>;
	struct AudioBaseInfo {
		AudioPackageType type{ AudioPackageType::kACC };
		unsigned sample_rate{ 8000 };
		unsigned bits_per_sample{ 16 };
		unsigned channels{ 2 };
	};
	using AudioBaseInfoPtr = std::shared_ptr<AudioBaseInfo>;
	struct Package {
	  DataPtr data;
	  uint64_t time_stamp{ 0 };
	};
	struct VideoPackage : public Package {
		VideoBaseInfo base_info;
	};
	using VideoPackagePtr = std::shared_ptr<VideoPackage>;
	struct AudioPackage : public Package {
		AudioBaseInfo base_info;
	};
	using AudioPackagePtr = std::shared_ptr<AudioPackage>;
}
#endif // BASE_INPUT_MEDIA_SOURCE_DATATYPE_H_
