#pragma region Description
/*
An adapter can be thought of as a GPU or other rendering subsystem:
When multiple GPUs are present in the system, you can explicitly pick which GPU you would prefer to run on,
or even create multiple devices on multiple adapters and allow them to co-operate with Direct3D 12's explicit multi-GPU support.
https://www.milty.nl/grad_guide/basic_implementation/d3d12/device.html
*/
#pragma endregion

#include "Vector.h"
#include <d3d11.h>
#include <winrt\Windows.Foundation.h>


namespace FieaGameEngine
{


	/// <summary>
	/// Stores a pointer to an adapter on the system.
	/// </summary>
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* adapter);
		IDXGIAdapter* _adaptar{nullptr};
		DXGI_ADAPTER_DESC _description;
	};

	class AdapterReader
	{
	public:
		static Vector<AdapterData> GetAdapters();
	private:
		static Vector<AdapterData> _adapters;
	};

}