#ifndef MOSAIC_HANDLER_IMPL_H_
#define MOSAIC_HANDLER_IMPL_H_
#include "./include/mosaic_handler.h"
#include <utility>
#include <vector>
namespace handler {
  class MosaicHandlerImpl : public MosaicHandler {
  public:
    MosaicHandlerImpl();
    virtual ~MosaicHandlerImpl();
  protected:
    void SetEvent(void* event) override;
    void Start() override;
    void Stop() override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void EnableMosaic(bool enable) override;
    bool SetParam(MosaicParamPtr param) override;
    bool Clear() override;
  private:
	  using MatrixItem = std::pair<int, int>;
	  void SwapYUVBlock(int index, VideoFramePtr video_frame);
	  unsigned GetBufferOffset(const std::pair<int, int>& item) const;
	  MosaicHandlerEvent* event_{ nullptr };
	  bool enable_{ false };
	  MosaicParamPtr mosaic_param_;
	  std::vector<MatrixItem> matrix_a_;
	  std::vector<MatrixItem> matrix_b_;
    unsigned block_width_{ 0 };
	  unsigned block_height_{ 0 };
  };
}
#endif // MOSAIC_HANDLER_IMPL_H_
