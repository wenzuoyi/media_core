#ifndef FILE_PLAYER_H_
#define FILE_PLAYER_H_
#include "base_player.h"
namespace core {
  class MEDIA_CORE_API_HEADER FilePlayerEvent : public BasePlayerEvent {
  public:
	  FilePlayerEvent();
	  virtual ~FilePlayerEvent();
  };

  class MEDIA_CORE_API_HEADER FilePlayer : public BasePlayer {
  public:
    FilePlayer();
    virtual ~FilePlayer();
  };
}
#endif // FILE_PLAYER_H_
