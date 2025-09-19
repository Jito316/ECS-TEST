#pragma once
#ifndef CORE_H
#define CORE_H

#ifdef CORE_DLL_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif // MODULEBASE_DLL_EXPORTS

namespace JTN
{
	class IEngine;
	extern "C" CORE_API IEngine* GetEngine();
};
#endif // !CORE_H
