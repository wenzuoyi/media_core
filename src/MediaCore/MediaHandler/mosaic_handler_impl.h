#ifndef MOSAIC_HANDLER_IMPL_H_
#define MOSAIC_HANDLER_IMPL_H_
#include "./include/mosaic_handler.h"
#include <utility>
#include <vector>
#include <mutex>
namespace handler {
  class MosaicHandlerImpl : public MosaicHandler , public  std::mutex {
  public:
    MosaicHandlerImpl();
    virtual ~MosaicHandlerImpl();
  protected:
    void SetEvent(MosaicHandlerEvent* event) override;
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void EnableMosaic(bool enable) override;
    bool IsSettingMosaic() const override;
    bool SetParam(MosaicParamPtr param) override;
    bool Clear() override;
  private:
	  using MatrixItem = std::pair<int, int>;
	  void SwapYUVBlock(int index, VideoFramePtr video_frame);
	  unsigned GetBufferOffset(const std::pair<int, int>& item, int stride) const;
	  MosaicHandlerEvent* event_{ nullptr };
	  bool enable_{ false };
	  MosaicParamPtr mosaic_param_;
	  std::vector<MatrixItem> matrix_a_;
	  std::vector<MatrixItem> matrix_b_;
  };
}
#endif // MOSAIC_HANDLER_IMPL_H_
