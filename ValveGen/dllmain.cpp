

#include "required.h"
#include "client.h"
#include "classbuilder.h"

DWORD WINAPI Begin(LPVOID param)
{
	valvegen::Client* client = valvegen::Client::Instance();

	client->WaitForClientToBecomeReady();

	if (!client->InitClientInterface())
		return EXIT_FAILURE;

	HINSTANCE module_instance = param ? reinterpret_cast<HINSTANCE>(param) : nullptr;

	valvegen::ClassBuilder::Instance()->CreateClasses(module_instance);

	return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
	)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, hinstDLL, 0, nullptr);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		valvegen::ClassBuilder::Instance()->Cleanup();
	}
	return TRUE;
}