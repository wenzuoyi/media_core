#ifndef GLOBAL_MEDIA_HANDLER_H_
#define GLOBAL_MEDIA_HANDLER_H_
#ifdef MEDIA_HANDLER_API_EXPORT_FLAG
#define MEDIA_HANDLER_API_HEADER __declspec(dllexport)
#else
#define MEDIA_HANDLER_API_HEADER __declspec(dllimport)
#endif
#endif // GLOBAL_MEDIA_HANDLER_H_
