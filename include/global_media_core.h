#ifndef GLOBAL_MEDIA_CORE_H_
#define GLOBAL_MEDIA_CORE_H_
#ifdef MEDIA_CORE_API_EXPORT_FLAG
#define MEDIA_CORE_API_HEADER __declspec(dllexport)
#else
#define MEDIA_CORE_API_HEADER __declspec(dllimport)
#endif
#endif // GLOBAL_MEDIA_CORE_H_
