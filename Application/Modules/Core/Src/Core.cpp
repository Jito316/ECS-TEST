#include "Core.h"
#include "Engine/Engine.h"
namespace JTN
{
	Engine* instance = nullptr;
	CORE_API IEngine* GetEngine()
	{
		return instance;
	}
}

CORE_API void Execute()
{
	JTN::instance = new JTN::Engine;
	auto& engine = *JTN::instance;
	engine.Init();
	engine.Update();
	engine.Release();
	delete JTN::instance;
}