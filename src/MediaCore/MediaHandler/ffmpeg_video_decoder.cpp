#include "ffmpeg_video_decoder.h"

namespace handler {
	FFmpegVideoDecoder::FFmpegVideoDecoder() = default;

	FFmpegVideoDecoder::~FFmpegVideoDecoder() = default;

  bool FFmpegVideoDecoder::Init(VideoPackageType video_package_type) {
    if (video_package_type != VideoPackageType::kH264 && video_package_type != VideoPackageType::kH265) {
      return false;
    }
    const auto av_codec_id = ConvertToAVCodecID(video_package_type);
    if (av_codec_id == AV_CODEC_ID_NONE) {
      return false;
    }
    codec_ = ::avcodec_find_decoder(av_codec_id);
    if (codec_ == nullptr) {
      return false;
    }
    parser_ = ::av_parser_init(codec_->id);
    if (parser_ == nullptr) {
      return false;
    }
    context_ = ::avcodec_alloc_context3(codec_);
    if (context_ == nullptr) {
      return false;
    }
    if (::avcodec_open2(context_, codec_, nullptr) < 0) {
      return false;
    }
    return true;
  }

  void FFmpegVideoDecoder::Fini() {
	  ::av_parser_close(parser_);
	  ::avcodec_close(context_);
	  ::avcodec_free_context(&context_);
  }

  VideoFrameListPtr FFmpegVideoDecoder::Decode(VideoPackagePtr package) {
    if (package == nullptr || package->data == nullptr) {
      return nullptr;
    }
    auto offset = 0U;
    auto result = std::make_shared<VideoFrameList>();
    while (offset < package->data->size()) {
      auto* packet = ::av_packet_alloc();
      if (packet == nullptr) {
        break;
      }
      auto* buffer = &(*(package->data))[0] + offset;
      auto buffer_size = package->data->size() - offset;
      auto parse_bytes = ::av_parser_parse2(parser_, context_, &packet->data, &packet->size, buffer, buffer_size,
                                            AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
      if (parse_bytes <= 0) {
        ::av_packet_free(&packet);
        break;
      }
      auto decoding_error_code = Decode(packet, [this, result](AVFrame* frame) {
        if (frame != nullptr) {
          auto item = ConvertAVFrameToVideoFrame(frame);
          result->push_back(item);
        }
      });
      if (0 != decoding_error_code) {
        ::av_packet_free(&packet);
        break;
      }
      offset += parse_bytes;
      ::av_packet_free(&packet);
    }
	  return result;
  }

  int FFmpegVideoDecoder::Decode(AVPacket* packet, std::function<void(AVFrame*)>&& callback) const {
    auto result = ::avcodec_send_packet(context_, packet);
    if (result < 0) {
      return result;
    }
    while (result >= 0) {
      auto* frame = ::av_frame_alloc();
      if (frame == nullptr) {
        return 0;
      }
      result = ::avcodec_receive_frame(context_, frame);
      if (result == 0) {
        callback(frame);
        ::av_frame_free(&frame);
      } else if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) {
        result = 0;
		    ::av_frame_free(&frame);
        break;
      } else if (result < 0) {
        fprintf(stderr, "ffmpeg - Error during decoding\n");
		    ::av_frame_free(&frame);
        break;
      }
    }
	  return result;
  }

  inline AVCodecID FFmpegVideoDecoder::ConvertToAVCodecID(VideoPackageType video_package_type) {
    auto result = AV_CODEC_ID_NONE;
    if (video_package_type == VideoPackageType::kH264) {
      result = AV_CODEC_ID_H264;
    } else if (video_package_type == VideoPackageType::kH265) {
      result = AV_CODEC_ID_HEVC;
    }
    return result;
  }

  VideoFramePtr FFmpegVideoDecoder::ConvertAVFrameToVideoFrame(AVFrame* frame) {
    auto result = std::make_shared<VideoFrame>();
    if (frame != nullptr) {
      result->data[0] = std::make_shared<Buffer>(frame->data[0], frame->data[0] + frame->linesize[0]);
      result->line_size[0] = frame->linesize[0];
      result->data[2] = std::make_shared<Buffer>(frame->data[1], frame->data[1] + frame->linesize[1]);
      result->line_size[2] = frame->linesize[1];
      result->data[1] = std::make_shared<Buffer>(frame->data[2], frame->data[2] + frame->linesize[2]);
      result->line_size[1] = frame->linesize[2];
      result->width = frame->width;
      result->height = frame->height;
    }
    return result;
  }
}
