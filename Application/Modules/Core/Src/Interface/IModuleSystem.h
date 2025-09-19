#pragma once
#include "Core.h"

enum ModuleEntry_
{
	/* Initialize */
	ModuleEntry_Initialize,
	ModuleEntry_InitializeWindow,
	ModuleEntry_InitializeInput,
	ModuleEntry_InitializeAudio,
	ModuleEntry_InitializeMotion,
	ModuleEntry_InitializeRenderer,
	ModuleEntry_InitializeEditor,
	ModuleEntry_InitializeScene,

	/*   Update   */
	ModuleEntry_Update,
	ModuleEntry_UpdateWindow,
	ModuleEntry_UpdateInput,
	ModuleEntry_UpdateRenderer,
	ModuleEntry_UpdateMotion,
	ModuleEntry_UpdateScene,
	ModuleEntry_UpdateAudio,
	ModuleEntry_UpdateEditor,

	/*   Render   */
	ModuleEntry_Render,
	ModuleEntry_BeginDraw,
	ModuleEntry_DrawProcess,
	ModuleEntry_EditorProcess,
	ModuleEntry_EndDraw,

	/*  Finalize  */
	ModuleEntry_Finalize,
	ModuleEntry_FinalizeScene,
	ModuleEntry_FinalizeRenderer,
	ModuleEntry_FinalizeEditor,
	ModuleEntry_FinalizeMotion,
	ModuleEntry_FinalizeAudio,
	ModuleEntry_FinalizeInput,
	ModuleEntry_FinalizeWindow,

	/*    Max    */
	ModuleEntry_Max
};


namespace JTN
{
	namespace Module
	{
		class IModule;

		class CORE_API IModuleSystem
		{
		public:
			virtual bool Load(const std::string& _moduleName) = 0;
			virtual bool Remove(IModule* _pModule) = 0;

		protected:
			friend IModule; struct ModuleSet;
			virtual IModule* GetModule(const std::type_info& _info) = 0;
			virtual void ModuleEntry(std::pair<ModuleEntry_, std::function<void()>> _pushData) = 0;
			virtual IModule* GetNowEntryModule() = 0;

		};
	};
};

