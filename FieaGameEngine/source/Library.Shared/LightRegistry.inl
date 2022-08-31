#include "pch.h"
#include "LightRegistry.h"

namespace FieaGameEngine
{
	inline LightRegistry& LightRegistry::GetInstance()
	{
		static LightRegistry _instance;
		return _instance;
	}

#pragma region Publicly exposed
	void LightRegistry::AddLight(Light& light)
	{
		GetInstance().IAddLight(light);
	}
	void LightRegistry::RemoveLight(Light& light)
	{
		GetInstance().IRemoveLight(light);
	}

	Light* LightRegistry::GetLight()
	{
		return GetInstance().IGetLight();
	}
#pragma endregion


#pragma region Private implementation
	void LightRegistry::IAddLight(Light& light)
	{
		GetInstance()._light = &light;
	}

	void LightRegistry::IRemoveLight(Light& light)
	{
		if (GetInstance()._light == &light)
		{
			GetInstance()._light = nullptr;
		}
	}

	Light* LightRegistry::IGetLight()
	{
		return GetInstance()._light;
	}
#pragma endregion

}
