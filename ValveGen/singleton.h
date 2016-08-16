
#pragma once

#include "required.h"

namespace valvegen
{
	template <typename T>
	class Singleton
	{
	public:
		static T* Instance()
		{
			static T instance;
			return &instance;
		}

	protected:
		inline explicit Singleton() {}
		virtual ~Singleton() {}
	};
}
