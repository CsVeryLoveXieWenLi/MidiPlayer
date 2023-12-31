
#include <Windows.h>

#include <llapi/LoggerAPI.h>

#include "static.h"
#include "init.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ll::registerPlugin(
            PLUGIN_NAME,
            PLUGIN_INTRODUCTION,
            ll::Version(PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_REVISION, ll::Version::Release),
            std::map<std::string, std::string>{
                {"Author", PLUGIN_AUTHOR},
                {"Web", PLUGIN_WEB},
        });
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C"
{
    // 必须导出该函数 以供加载器初始化
    _declspec(dllexport) void onPostInit()
    {
        // 取消缓冲区同步
        std::ios::sync_with_stdio(false);

		// 初始化
		Event::ServerStartedEvent::subscribe([](const Event::ServerStartedEvent& event) {

			InitUtils::RegisterCommand();
			InitUtils::RegisterEvent();

			Logger logger(PLUGIN_NAME);
			logger.info(format("Plugin {} Load Success!", PLUGIN_NAME));

			return true;
		});
    }
}
