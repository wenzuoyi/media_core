#ifndef FILE_PLAYER_H_
#define FILE_PLAYER_H_
#include "base_player.h"
namespace core {
  class MEDIA_CORE_API_HEADER FilePlayerEvent : virtual public BasePlayerEvent {
  public:
	  FilePlayerEvent();
	  virtual ~FilePlayerEvent();
	  virtual void OnEOF() = 0;
	  virtual void OnBOF() = 0;
  };

  class MEDIA_CORE_API_HEADER FilePlayer : virtual public BasePlayer {
  public:
    FilePlayer();
    virtual ~FilePlayer();
    virtual void RPlay(bool enable) = 0;
    virtual void PreviousFrame() = 0;
    virtual void EnableLoopPlayback(bool enable) = 0;
  };
}
#endif // FILE_PLAYER_H_
