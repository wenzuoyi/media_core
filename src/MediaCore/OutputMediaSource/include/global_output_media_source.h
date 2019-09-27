#ifndef GLOBAL_OUTPUT_MEDIA_SOURCE_H_
#define GLOBAL_OUTPUT_MEDIA_SOURCE_H_
#ifdef OUTPUT_MEDIA_API_EXPORT_FLAG
#define OUTPUT_MEDIA_API_HEADER __declspec(dllexport)
#else
#define OUTPUT_MEDIA_API_HEADER __declspec(dllimport)
#endif
#endif // GLOBAL_OUTPUT_MEDIA_SOURCE_H_
