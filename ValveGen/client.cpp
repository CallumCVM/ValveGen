

#include "client.h"

namespace valvegen
{
	inline void**& getvtable(void* inst, size_t offset = 0)
	{
		return *reinterpret_cast<void***>((size_t)inst + offset);
	}
	inline const void** getvtable(const void* inst, size_t offset = 0)
	{
		return *reinterpret_cast<const void***>((size_t)inst + offset);
	}
	template< typename Fn >
	inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
	{
		return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
	}


	Client::Client() :
		client_dll_handle_(nullptr)
	{}



	void Client::WaitForClientToBecomeReady()
	{
		HMODULE client_dll = nullptr;
		
		do
		{
			client_dll = GetModuleHandle("client.dll");
		} while (client_dll == nullptr);

		client_dll_handle_ = client_dll;
	}



	bool Client::InitClientInterface()
	{
		if (!client_dll_handle_)
			return false;

		FARPROC create_interface_fn = GetProcAddress(client_dll_handle_, "CreateInterface");

		if (!create_interface_fn)
			return false;

		auto FindListAddress = [](FARPROC function_address) -> DWORD
		{
			const BYTE OPCODE_JMP = 0xE9;

			DWORD start_address = reinterpret_cast<DWORD>(function_address);

			PBYTE opcode_info = reinterpret_cast<PBYTE>(start_address);

			if (opcode_info[4] != OPCODE_JMP)
				return 0;

			DWORD jmp_address = *reinterpret_cast<DWORD*>(start_address + 5);
			DWORD list_address = start_address + 4 + jmp_address + 5;

			return *reinterpret_cast<DWORD*>(list_address + 6);
		};

		DWORD list_address = FindListAddress(create_interface_fn);

		if (list_address == NULL)
			return false;

		EngineInterface* interface_list = *reinterpret_cast<EngineInterface**>(list_address);

		if (!interface_list)
			return false;

		bool found_interface = false;

		while (interface_list)
		{
			std::string interface_name = interface_list->name;

			for (size_t i = interface_name.length() - 1; i > 0; i--)
			{
				if (isdigit(interface_name[i]))
					interface_name.erase(interface_name.length() - 1, 1);
				else
					break;
			}

			if (interface_name == "VClient")
			{
				found_interface = true;
				break;
			}

			interface_list = interface_list->next;
		}

		if (!found_interface)
			return false;

		class_ptr_ = ((LPVOID(*)(void))interface_list->callback)();

		if (!class_ptr_)
			return false;

		return true;
	}



	ClientClass* Client::GetAllClasses()
	{
		int index = -1;

		for (int i = 0; i < 16; i++)
		{
			auto pdwClient = *(PDWORD_PTR*)class_ptr_;
			std::uint8_t* function = (std::uint8_t*)pdwClient[i];

			if (function[0] == 0xA1 && function[5] == 0xC3)
			{
				index = i;
				break;
			}
		}

		typedef ClientClass* (__thiscall *tGetAllClasses)(void*);
		tGetAllClasses pGetAllClasses = getvfunc<tGetAllClasses>(class_ptr_, index);

		if (!pGetAllClasses)
			return nullptr;

		return pGetAllClasses(class_ptr_);
	}
}