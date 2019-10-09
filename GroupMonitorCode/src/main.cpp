#define WIN32_LEAN_AND_MEAN // 从 Windows 头中排除极少使用的资料
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // 进程被调用
	case DLL_THREAD_ATTACH: // 线程被调用
	case DLL_THREAD_DETACH: // 线程被停止
	case DLL_PROCESS_DETACH: // 进程被停止
		break;
	}
	return TRUE;
}