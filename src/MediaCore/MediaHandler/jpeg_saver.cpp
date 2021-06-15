#include "jpeg_saver.h"
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>
#include "character_set_convertor.h"
namespace handler {
  JPEGSaver::JPEGSaver() {
    Gdiplus::GdiplusStartup(&token_, &startup_input_, nullptr);
    mime_ = std::wstring(L"image/jpeg");
  }

  JPEGSaver::~JPEGSaver() {
    Gdiplus::GdiplusShutdown(token_);
  }

  bool JPEGSaver::Open(const std::string& file_path) {
	  jpeg_file_path_ = file_path;
	  Poco::Path path(file_path);
	  auto id = Poco::UUIDGenerator::defaultGenerator().createOne();
	  bmp_file_path_ = path.parent().toString() + id.toString() + std::string(".bmp");
	  return BitmapSaver::Open(bmp_file_path_);
  }

  void JPEGSaver::Close() {
    BitmapSaver::Close();
    auto temp_bmp_file_path_ = utils::CharacterSetConvertor::Instance()->GBKToUnicode(bmp_file_path_);
	  auto temp_jpeg_file_path_ = utils::CharacterSetConvertor::Instance()->GBKToUnicode(jpeg_file_path_);
	  if (!ConvertBitmapToJPEG(temp_bmp_file_path_, temp_jpeg_file_path_, 100)) {
		  return;
	  }
	  Poco::File file(bmp_file_path_);
	  file.remove();
  }

  int JPEGSaver::GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
	  UINT  num = 0;
	  UINT  size = 0; 	  
	  Gdiplus::GetImageEncodersSize(&num, &size);
	  if (size == 0) {
		  return -1;  	    
	  }
    auto* image_codec_info = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));
	  if (image_codec_info == nullptr) {
		  return -1;	    
	  }
	  Gdiplus::GetImageEncoders(num, size, image_codec_info);
	  for (auto j = 0; j < num; ++j) {
		  if (wcscmp(image_codec_info[j].MimeType, format) == 0) {
			  *pClsid = image_codec_info[j].Clsid;
			  free(image_codec_info);
			  return j;
		  }
	  }
	  free(image_codec_info);
	  return -1; 
  }

  bool JPEGSaver::ConvertBitmapToJPEG(const std::wstring& bmp_file_path, const std::wstring& jpeg_file_path, int qa) const {
    auto image = std::make_shared<Gdiplus::Image>(bmp_file_path.c_str());
    if (image == nullptr) {
      return false;
    }
    CLSID encoder_clsid;
    if (!GetEncoderClsid(mime_.c_str(), &encoder_clsid)) {
      return false;
    }
    Gdiplus::EncoderParameters parameters;
    auto quality = qa;
    parameters.Count = 1;
    parameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
    parameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
    parameters.Parameter[0].NumberOfValues = 1;
    parameters.Parameter[0].Value = &quality;
    return S_OK == image->Save(jpeg_file_path.c_str(), &encoder_clsid, &parameters);
  }
}
