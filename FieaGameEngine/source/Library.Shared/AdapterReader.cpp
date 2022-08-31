#include "AdapterReader.h"


namespace FieaGameEngine
{
	//Declaration of  static member
	Vector<AdapterData> AdapterReader::_adapters;

	Vector<AdapterData> AdapterReader::GetAdapters()
	{
		if (_adapters.Size() > 0) { return _adapters; }

		winrt::com_ptr<IDXGIFactory> _factory;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(_factory->GetAddressOf())
	}

}