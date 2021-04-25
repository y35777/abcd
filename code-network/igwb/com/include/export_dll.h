#undef IGWB_Svc_Export

#ifdef _PLATFORM_WIN32
#define IGWB_Svc_Export __declspec (dllexport)
#else
#define IGWB_Svc_Export
#endif
