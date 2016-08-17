

#include "required.h"
#include "client.h"
#include "classbuilder.h"

DWORD WINAPI Begin(LPVOID)
{
	valvegen::Client* client = valvegen::Client::Instance();

	client->WaitForClientToBecomeReady();

	if (!client->InitClientInterface())
		return EXIT_FAILURE;

	valvegen::ClassBuilder::Instance()->CreateClasses();

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
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Begin, nullptr, 0, nullptr);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
		valvegen::ClassBuilder::Instance()->Cleanup();
	}
	return TRUE;
}