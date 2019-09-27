#ifndef GLOBAL_INPUT_MEDIA_SOURCE_H_
#define GLOBAL_INPUT_MEDIA_SOURCE_H_
#ifdef INPUT_MEDIA_API_EXPORT_FLAG
#define INPUT_MEDIA_API_HEADER __declspec(dllexport)
#else
#define INPUT_MEDIA_API_HEADER __declspec(dllimport)
#endif
#endif // GLOBAL_INPUT_MEDIA_SOURCE_H_
