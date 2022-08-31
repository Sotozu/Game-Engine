#pragma once
#include "Vector.h"


namespace FieaGameEngine
{
	class Light;

	class LightRegistry
	{
	public:

		static void AddLight(Light& light);
		static void RemoveLight(Light& light);
		static Light* GetLight();
	private:
		
		Light* _light;

		static LightRegistry& GetInstance();


		LightRegistry() = default;
		LightRegistry(const LightRegistry&) = delete; //Don't want the TypeRegistry to be copyable
		LightRegistry& operator=(const LightRegistry&) = delete; //Don't want the TypeRegistry to be assigned

		static void IAddLight(Light& light);
		static void IRemoveLight(Light& light);
		static Light* IGetLight();

	};
}


