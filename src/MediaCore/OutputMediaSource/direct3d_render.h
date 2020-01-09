#ifndef DIRECT3D_RENDER_H_
#define DIRECT3D_RENDER_H_
#include <future>
#include <d3d9.h>
#include <d3dx9tex.h>
#include "bound_circle_buffer.hpp"
#include "window_ruler.h"
#include "include/video_output_media_source.h"
namespace output {
	class Direct3DRender : public VideoOutputMediaSource {
	public:
		Direct3DRender();
    virtual ~Direct3DRender() = default;
  protected:
    void Init() override;
    void Fini() override;
    void SetEvent(VideoOutputMediaSourceEvent* sink) override;
    void SetVideoOutputMediaParam(VideoOutputParamPtr video_output_param) override;
    bool Play() override;
    void Stop() override;
    void SetOSD(OSDParamListPtr osd_param_list) override;
    void SetDisplayRatio(DisplayRatio display_ratio) override;
    void InputVideoFrame(VideoFramePtr video_frame) override;
    void EnableROI(bool enable) override;
    void UpdateROI(const RECT& roi) override;
    void ResizeWindow() override;
    bool IsValidRendingArea(const POINT& point) const override;
	private:
    bool CreateD3dDevice();
	  bool CreateD3dSurface();
	  bool CreateRenderTask();
	  bool CreateOSDFont();
	  void Render(VideoFramePtr video_frame) const;
	  void SetOSDContent() const;
	  void TransformCoordinateSystem();
    static void CopyBufferToSurface(VideoFramePtr video_frame, D3DLOCKED_RECT* surface);
    static void CopyYData(VideoFramePtr video_frame, unsigned char* target_data, int target_lines_size, int i);
	  static void CopyUVData(VideoFramePtr video_frame, unsigned char* target_data, int target_lines_size, int i);
    VideoOutputParamPtr param_;
		VideoOutputMediaSourceEvent* sink_{ nullptr };
		OSDParamListPtr osd_param_list_;
		bool exit_{ false };
		std::future<void> render_task_;
		POINT left_top_corner_, right_bottom_corner_;
		LPDIRECT3D9  d3d_{ nullptr };
		LPDIRECT3DDEVICE9 device_{ nullptr };
		LPDIRECT3DSURFACE9 source_surface_{ nullptr };
		LPDIRECT3DSURFACE9 customize_surface_{ nullptr };
		LPD3DXFONT font_{nullptr};    
		utils::BoundedBuffer<VideoFramePtr> video_frames_list_;
		utils::WindowRulerPtr window_ruler_;
		long window_width_{ 0 };
		long window_height_{ 0 };
		bool enable_roi_{ false };
		bool update_roi_{ false };
		bool is_playing_{ false };
		RECT roi_;
	};
}
#endif // DIRECT3D_RENDER_H_
