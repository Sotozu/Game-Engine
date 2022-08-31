#pragma once

namespace FieaGameEngine
{
	template<typename T>
	inline ContentTypeReaderPaul<T>::ContentTypeReaderPaul(GamePaul& game, const std::uint64_t targetTypeId) :
		AbstractContentTypeReaderPaul(game, targetTypeId)
	{
	}

	template<typename T>
	inline std::shared_ptr<RTTI> ContentTypeReaderPaul<T>::Read(const std::wstring& assetName)
	{
		return _Read(assetName);
	}
}