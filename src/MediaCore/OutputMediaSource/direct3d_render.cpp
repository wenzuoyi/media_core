#include "direct3d_render.h"
#include <cassert>
namespace output {
	const int Direct3DRender::D3DFVF_CUSTOMVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  #define  DIRECTXOPERATION(opt) \
    if (FAILED(opt)) { \
	    return; \
	  } \

  Direct3DRender::Direct3DRender() : video_frames_list_(3) {
  }

  Direct3DRender::~Direct3DRender() {
  }

  void Direct3DRender::Init() {
	  d3d_ = Direct3DCreate9(D3D_SDK_VERSION);
	  assert(d3d_ != nullptr);
  }

  void Direct3DRender::Fini() {
	  if (d3d_ != nullptr) {
		  d3d_->Release();
	  }
  }

  void Direct3DRender::SetEvent(VideoOutputMediaSourceEvent* sink) {
    if (sink != nullptr && sink_ != sink) {
      sink_ = sink;
    }
  }

  void Direct3DRender::SetVideoOutputMediaParam(VideoOutputParamPtr video_output_param) {
    if (video_output_param != nullptr && video_output_param != param_) {
      param_ = video_output_param;
    }
    RECT rect;
    GetClientRect(param_->render_wnd, &rect);
    window_width_ = rect.right - rect.left;
    window_height_ = rect.bottom - rect.top;
    custom_vertex_[0] = {0.0f, 0.0f, 0.0f, 1.0f,D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 0.0f};
    custom_vertex_[1] = {static_cast<FLOAT>(window_width_), 0.0f, 0.0f, 1.0f,D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0.0f};
    custom_vertex_[2] = {static_cast<FLOAT>(window_width_), static_cast<FLOAT>(window_height_), 0.0f, 1.0f,D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 1.0f};
    custom_vertex_[3] = {0.0f, static_cast<FLOAT>(window_height_), 0.0f, 1.0f,D3DCOLOR_ARGB(255, 255, 255, 255), 0.0f, 1.0f};
  }

  bool Direct3DRender::CreateD3dDevice() {
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth = window_width_;
    d3dpp.BackBufferHeight = window_height_;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.BackBufferCount = 1;
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    d3dpp.hDeviceWindow = param_->render_wnd;
    d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = FALSE;
    d3dpp.Flags = D3DPRESENTFLAG_VIDEO;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    const auto result = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
                                           D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp,
                                           &device_);
    return SUCCEEDED(result);
  }

  bool Direct3DRender::CreateTexture() {
    if (device_ == nullptr) {
      return false;
    }
    const auto result = device_->CreateTexture(window_width_, window_height_, 1, D3DUSAGE_SOFTWAREPROCESSING,
                                               static_cast<D3DFORMAT>(MAKEFOURCC('Y', 'V', '1', '2')), D3DPOOL_MANAGED,
                                               &texture_, nullptr);
    return SUCCEEDED(result);
  }

  bool Direct3DRender::CreateVertexBuffer() {
    const auto result = device_->CreateVertexBuffer(4 * sizeof(CustomVertex), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
                                                    &vertex_buffer, nullptr);
	  return SUCCEEDED(result);
  }

  bool Direct3DRender::CreateRenderTask() {
    render_task_ = std::async(std::launch::async, [this]() {
      while (!exit_) {
        auto item = video_frames_list_.PopBack();
        Render(item);
      }
    });
	  return true;
  }

  void Direct3DRender::Render(VideoFramePtr video_frame) const {
	  DIRECTXOPERATION(device_->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0));
	  D3DLOCKED_RECT d3d_rect;
    DIRECTXOPERATION(texture_->LockRect(0, &d3d_rect, nullptr, 0));
    auto source = &((*video_frame)[0]);
    auto target = static_cast<BYTE *>(d3d_rect.pBits);
    auto stride = d3d_rect.Pitch;
	  auto frame_height = param_->height;
	  auto frame_width = param_->width;
	  for (auto i = 0U; i < frame_height; i++) {
		  memcpy(target + i * stride, source + i * frame_width, frame_width);
	  }
	  for (auto i = 0U; i < frame_height / 2; i++) {
		  memcpy(target + stride * frame_height + i * stride / 2, source + frame_width * frame_height + frame_width * frame_height / 4 + i * frame_width / 2, frame_width / 2);
	  }
	  for (auto i = 0U; i < frame_height / 2; i++) {
		  memcpy(target + stride * frame_height + stride * frame_height / 4 + i * stride / 2, source + frame_width * frame_height + i * frame_width / 2, frame_width / 2);
	  }
	  DIRECTXOPERATION(texture_->UnlockRect(0));
	  DIRECTXOPERATION(device_->BeginScene());
	  DIRECTXOPERATION(device_->SetTexture(0, texture_));
	  DIRECTXOPERATION(device_->SetStreamSource(0, vertex_buffer, 0, sizeof(CustomVertex)));
	  DIRECTXOPERATION(device_->SetFVF(D3DFVF_CUSTOMVERTEX));
	  DIRECTXOPERATION(device_->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2));
	  DIRECTXOPERATION(device_->EndScene());
	  DIRECTXOPERATION(device_->Present(nullptr, nullptr, nullptr, nullptr));
  }

  bool Direct3DRender::Play() {
    if (!CreateD3dDevice()) {
      return false;
    }
    if (!CreateTexture()) {
      return false;
    }
    if (!CreateVertexBuffer()) {
      return false;
    }
    if (!CreateRenderTask()) {
      return false;
    }
    return true;
  }

  void Direct3DRender::Stop() {
	  exit_ = true;
	  render_task_.wait();
	  if (vertex_buffer != nullptr) {
		  vertex_buffer->Release();
		  vertex_buffer = nullptr;
	  }  
    if (texture_ != nullptr) {
      texture_->Release();
      texture_ = nullptr;
    }
    if (device_ != nullptr) {
      device_->Release();
      device_ = nullptr;
    }
  }

  void Direct3DRender::SetOSD(OSDParamListPtr osd_param_list) {
  }

  void Direct3DRender::SetDisplayRatio(DisplayRatio display_ratio) {
  }

  void Direct3DRender::Rotate(RotateType rotate_type) {
  }

  bool Direct3DRender::InputVideoFrame(VideoFramePtr video_frame) {
	  video_frames_list_.PushFront(video_frame);
	  return true;
  }
}

