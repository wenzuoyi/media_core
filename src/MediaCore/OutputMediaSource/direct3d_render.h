#ifndef DIRECT3D_RENDER_H_
#define DIRECT3D_RENDER_H_
#include <future>
#include <d3d9.h>
#include "../MediaCoreUtils/include/bound_circle_buffer.hpp"
#include "include/video_output_media_source.h"
namespace output {
	struct CustomVertex{
		FLOAT       x, y, z;
		FLOAT       rhw;
		D3DCOLOR    diffuse;
		FLOAT       tu, tv;
	};
	class Direct3DRender : public VideoOutputMediaSource {
	public:
		Direct3DRender();
		virtual ~Direct3DRender();
  protected:
    void Init() override;
    void Fini() override;
    void SetEvent(VideoOutputMediaSourceEvent* sink) override;
    void SetVideoOutputMediaParam(VideoOutputParamPtr video_output_param) override;
    bool Play() override;
    void Stop() override;
    void SetOSD(OSDParamListPtr osd_param_list) override;
    void SetDisplayRatio(DisplayRatio display_ratio) override;
    void Rotate(RotateType rotate_type) override;
    bool InputVideoFrame(VideoFramePtr video_frame) override;
	private:
    bool CreateD3dDevice();
    bool CreateTexture();
	  bool CreateVertexBuffer();
	  bool CreateRenderTask();
	  void Render(VideoFramePtr video_frame) const;
    VideoOutputParamPtr param_;
		VideoOutputMediaSourceEvent* sink_{ nullptr };
		std::future<void> render_task_;
		bool exit_{ false };
		LPDIRECT3D9  d3d_{ nullptr };
		LPDIRECT3DDEVICE9 device_{ nullptr };
		LPDIRECT3DTEXTURE9 texture_{ nullptr };
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer{ nullptr };
		long window_width_{ 0 };
		long window_height_{ 0 };
		std::array<CustomVertex, 4> custom_vertex_;
		utils::BoundedBuffer<VideoFramePtr> video_frames_list_;
		static const int D3DFVF_CUSTOMVERTEX;
	};
}
#endif // DIRECT3D_RENDER_H_
