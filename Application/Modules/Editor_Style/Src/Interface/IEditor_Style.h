#pragma once
#ifdef EDITOR_STYLE_DLL_EXPORTS
#define EDITOR_STYLE_API __declspec(dllexport)
#else
#define EDITOR_STYLE_API __declspec(dllimport)
#endif

