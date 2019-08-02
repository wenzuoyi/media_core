#ifndef GLOBAL_OUTPUT_MEDIA_SOURCE_H_
#define GLOBAL_OUTPUT_MEDIA_SOURCE_H_
#ifdef API_EXPORT_FLAG
#define API_HEADER __declspec(dllexport)
#else
#define API_HEADER __declspec(dllimport)
#endif
#endif // GLOBAL_OUTPUT_MEDIA_SOURCE_H_
