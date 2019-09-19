#include "include/character_set_convertor.h"
#include "character_set_convertor_impl.h"
namespace utils {
  CharacterSetConvertor* CharacterSetConvertor::Instance() {
    static CharacterSetConvertorImpl instance;
    return reinterpret_cast<CharacterSetConvertor*>(&instance);
  }
}
