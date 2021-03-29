#include "direct3d_render.h"
#include <cassert>
#include "character_set_convertor.h"
namespace output {
  #define  DXRETURNVOID(opt) \
    if (FAILED(opt)) { \
	    return; \
	  }
  #define DXRETURNVALUE(opt, var) \
  if (FAILED(opt)) { \
    return var; \
  } \

  Direct3DRender::Direct3DRender() : video_frames_list_(3), roi_() {
  }
  
  void Direct3DRender::Init() {
	  d3d_ = :: Direct3DCreate9(D3D_SDK_VERSION);
	  assert(d3d_ != nullptr);
	  window_ruler_ = utils::WindowRuler::CreateInstance();
  }

  void Direct3DRender::Fini() {
	  if (d3d_ != nullptr) {
		  d3d_->Release();
	  }
	  window_ruler_ = nullptr;
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
	  ResizeWindow();
  }

  bool Direct3DRender::CreateD3dDevice() {
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    const auto result = d3d_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, param_->render_wnd,
                                           D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &d3dpp,
                                           &device_);
    return SUCCEEDED(result);
  }

  bool Direct3DRender::CreateD3dSurface() {
    if (param_ == nullptr) {
      return false;
    }
    auto var = device_->CreateOffscreenPlainSurface(param_->width, param_->height,
                                               static_cast<D3DFORMAT>(MAKEFOURCC('Y', 'V', '1', '2')), D3DPOOL_DEFAULT,
                                               &source_surface_, nullptr);
    DXRETURNVALUE(var, false)
    var = device_->CreateOffscreenPlainSurface(param_->width, param_->height, D3DFMT_X8R8G8B8,
                                                           D3DPOOL_DEFAULT, &customize_surface_, nullptr);  	
		return SUCCEEDED(var);
  }

  bool Direct3DRender::CreateRenderTask() {
	  exit_ = false;
    render_task_ = std::async(std::launch::async, [this]() {
      while (!exit_) {
        auto item = video_frames_list_.PopBack();
        if (item != nullptr) {
          Render(item);
        }
      }
	    video_frames_list_.Clear();
    });
	  return true;
  }

  bool Direct3DRender::CreateOSDFont() {
    return SUCCEEDED(
      D3DXCreateFont(device_, 30, 0, 0, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
        FF_DONTCARE, TEXT("Î¢ÈíÑÅºÚ"), &font_));
  }

  void Direct3DRender::Render(VideoFramePtr video_frame) const {
	  DXRETURNVOID(device_->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0))
		D3DLOCKED_RECT surface;
	  DXRETURNVOID(source_surface_->LockRect(&surface, nullptr, D3DLOCK_DONOTWAIT))
		CopyBufferToSurface(video_frame, &surface);
	  video_frame = nullptr;
	  DXRETURNVOID(source_surface_->UnlockRect())
	  DXRETURNVOID(device_->StretchRect(source_surface_, nullptr, customize_surface_, nullptr, D3DTEXF_NONE));
	  HDC hdc;
	  DXRETURNVOID(customize_surface_->GetDC(&hdc));
	  if (sink_ != nullptr) {
		  sink_->OnCustomPainting(hdc);
	  }
	  DXRETURNVOID(customize_surface_->ReleaseDC(hdc));
	  DXRETURNVOID(device_->BeginScene());
	  LPDIRECT3DSURFACE9 target_surface{ nullptr };
	  DXRETURNVOID(device_->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &target_surface))
    RECT* source_rect = nullptr;
    if (enable_roi_ && update_roi_) {
      source_rect = &roi_;
    }
    if (window_ruler_->IsAutoAdaptFrom()) {
		  DXRETURNVOID(device_->StretchRect(customize_surface_, source_rect, target_surface, nullptr, D3DTEXF_LINEAR))
    } else {
      const auto rect = window_ruler_->GetRendingArea();
		  DXRETURNVOID(device_->StretchRect(customize_surface_, source_rect, target_surface, &rect, D3DTEXF_LINEAR))
    }
    SetOSDContent();
	  DXRETURNVOID(device_->EndScene());
	  DXRETURNVOID(device_->Present(nullptr, nullptr, nullptr, nullptr));
	  DXRETURNVOID(target_surface->Release());	  
  }

  void Direct3DRender::CopyBufferToSurface(VideoFramePtr video_frame, D3DLOCKED_RECT* surface) {
    auto target = static_cast<BYTE *>(surface->pBits);
    auto stride = surface->Pitch;
    for (auto i = 0; i < video_frame->height; i++) {
      CopyYData(video_frame, target, stride, i);
      if (i < video_frame->height / 2) {
        CopyUVData(video_frame, target, stride, i);
      }
    }
  }

  void Direct3DRender::CopyYData(VideoFramePtr video_frame, unsigned char* target_data, int target_lines_size, int i) {
    const auto source_y_object = video_frame->data[0];
    const auto source_y_data = &((*source_y_object)[0]);
    const auto source_y_linesize = (video_frame->line_size)[0];
    memcpy(target_data + i * target_lines_size, source_y_data + i * source_y_linesize, source_y_linesize);
  }

  void Direct3DRender::CopyUVData(VideoFramePtr video_frame, unsigned char* target_data, int target_lines_size, int i) {
    const auto target_u = (target_lines_size * video_frame->height + target_data);
    const auto target_v = (target_lines_size * video_frame->height * 5 / 4 + target_data);
	  const auto source_u_object = video_frame->data[1];
	  const auto source_u_data = &((*source_u_object)[0]);
	  const auto source_u_linesize = video_frame->line_size[1];
    const auto source_v_object = video_frame->data[2];
	  const auto source_v_data = &((*source_v_object)[0]);
	  const auto source_v_linesize = video_frame->line_size[2];
	  memcpy(target_v + i * target_lines_size / 2, source_u_data + i * source_u_linesize, source_u_linesize);
	  memcpy(target_u + i * target_lines_size / 2, source_v_data + i * source_v_linesize, source_v_linesize);
  }

  void Direct3DRender::SetOSDContent() const {
    if (osd_param_list_ == nullptr || osd_param_list_->empty()) {
      return;
    }
    for (const auto& item : *osd_param_list_) {
      if (item.enable) {
        RECT rect;
        rect.left = item.x_pos;
        rect.top = item.y_pos;
        rect.right = item.x_pos + item.width;
        rect.bottom = item.y_pos + item.height;
        const auto content = utils::CharacterSetConvertor::Instance()->UTF8ToUnicode(item.content);
        font_->DrawText(nullptr, &content[0], static_cast<INT>(content.length()), &rect,
                        DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
      }
    }
  }

  bool Direct3DRender::Play() {
    if (!CreateD3dDevice()) {
      return false;
    }
    if (!CreateD3dSurface()) {
      return false;
    }
    if (!CreateOSDFont()) {
      return false;
    }
    if (!CreateRenderTask()) {
      return false;
    }
    is_playing_ = true;
    return true;
  }

  void Direct3DRender::Stop() {
	  exit_ = true;
	  InputVideoFrame(VideoFramePtr());
	  render_task_.wait();
	  video_frames_list_.Clear();
    if (customize_surface_ != nullptr) {
      customize_surface_->Release();
      customize_surface_ = nullptr;
    }
    if (source_surface_ != nullptr) {
      source_surface_->Release();
      source_surface_ = nullptr;
    }
    if (device_ != nullptr) {
      device_->Release();
      device_ = nullptr;
    }
    if (font_ != nullptr) {
      font_->Release();
      font_ = nullptr;
    }
    is_playing_ = false;
  }

  void Direct3DRender::SetOSD(OSDParamListPtr osd_param_list) {
    if (osd_param_list != nullptr && osd_param_list != osd_param_list_) {
      osd_param_list_ = osd_param_list;
    }
  }

  void Direct3DRender::SetDisplayRatio(DisplayRatio display_ratio) {
    if (display_ratio == DisplayRatio::kAdapter) {
      window_ruler_->EnableAutoAdaptFrom(true);
    } else if (display_ratio == DisplayRatio::kRatio43) {
      window_ruler_->SetAspectRatio(std::make_pair(4, 3));
    } else if (display_ratio == DisplayRatio::kRatio169) {
      window_ruler_->SetAspectRatio(std::make_pair(16, 9));
    }
    TransformCoordinateSystem();
  }

  void Direct3DRender::InputVideoFrame(VideoFramePtr video_frame) {
    video_frames_list_.PushFront(video_frame);
    if (sink_ != nullptr) {
      sink_->OnTransmitDataEvent(video_frame);
    }
  }

  bool Direct3DRender::IsROIEnable() const {
	  return enable_roi_;
  }

  void Direct3DRender::EnableROI(bool enable) {
    if (enable == enable_roi_) {
      return;
    }
    enable_roi_ = enable;
    update_roi_ = false;
    roi_ = {0, 0, 0, 0};
  }

  void Direct3DRender::UpdateROI(const RECT& roi) {
    if (std::tie(roi_.left, roi_.top, roi_.right, roi_.bottom) == std::tie(roi.left, roi.top, roi.right, roi.bottom)) {
      return;
    }
    roi_ = roi;
    if (!update_roi_) {
      update_roi_ = true;
    }
  }

  void Direct3DRender::TransformCoordinateSystem() {
    const auto rending_area = window_ruler_->GetRendingArea();
    std::tie(left_top_corner_.x, left_top_corner_.y) = std::make_tuple(rending_area.left, rending_area.top);
    std::tie(right_bottom_corner_.x, right_bottom_corner_.y) = std::make_tuple(rending_area.right, rending_area.bottom);
    ::ClientToScreen(param_->render_wnd, &left_top_corner_);
    ::ClientToScreen(param_->render_wnd, &right_bottom_corner_);
    const auto parent = ::GetParent(param_->render_wnd);
    ::ScreenToClient(parent, &left_top_corner_);
    ::ScreenToClient(parent, &right_bottom_corner_);
  }

  void Direct3DRender::ResizeWindow() {
    if (is_playing_) {
      Stop();
      Play();
    }
	  RECT rect;
	  GetClientRect(param_->render_wnd, &rect);
	  window_width_ = rect.right - rect.left;
	  window_height_ = rect.bottom - rect.top;
	  window_ruler_->SetWindowSize(window_width_, window_height_);
	  TransformCoordinateSystem();
  }

  bool Direct3DRender::IsValidRendingArea(const POINT& point) const {
    if (window_ruler_ == nullptr) {
      return false;
    }
    if (point.x < left_top_corner_.x || point.x > right_bottom_corner_.x) {
      return false;
    }
    if (point.y < left_top_corner_.y || point.y > right_bottom_corner_.y) {
      return false;
    }
    return true;
  }
}

