#ifndef BASE_PLAYER_H_
#define BASE_PLAYER_H_
#include <string>
#include "global_media_core.h"
#include "base_player_datatype.h"
namespace core {
  class MEDIA_CORE_API_HEADER BasePlayerEvent {
  public:
	  BasePlayerEvent();
	  virtual ~BasePlayerEvent();
  };

	class MEDIA_CORE_API_HEADER BasePlayer {
	public:
		BasePlayer();
		virtual ~BasePlayer();
		virtual bool Open(const std::string& uri) = 0;
		virtual void Close() = 0;
		virtual bool Play() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual bool Seek(int64_t time_stamp) = 0;
		virtual bool NextFrame() = 0;
		virtual void Speed(double speed) = 0;
		virtual void Mute(bool enable) = 0;
		virtual void SetVolume(int volume) = 0;
		virtual int GetVolume() = 0;
		virtual int Snapshot(const std::string& url) = 0;
		virtual int Zoom(RegionPtr region) = 0;
	};

}
#endif // BASE_PLAYER_H_
