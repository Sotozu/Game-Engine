#pragma once
#include "ContentManagerPaul.h"

namespace FieaGameEngine
{
	inline const std::map<std::wstring, std::shared_ptr<RTTI>>& ContentManagerPaul::LoadedAssets() const
	{
		return mLoadedAssets;
	}

	inline const std::wstring& ContentManagerPaul::RootDirectory() const
	{
		return mRootDirectory;
	}

	inline void ContentManagerPaul::SetRootDirectory(const std::wstring& rootDirectory)
	{
		mRootDirectory = rootDirectory + (StringHelper::EndsWith(rootDirectory, L"\\") ? std::wstring() : L"\\");
	}

	template<typename T>
	inline std::shared_ptr<T> ContentManagerPaul::Load(const std::wstring& assetName, bool reload, std::function<std::shared_ptr<T>(std::wstring&)> customReader)
	{
		if (reload == false)
		{
			auto it = mLoadedAssets.find(assetName);
			if (it != mLoadedAssets.end())
			{
				return std::static_pointer_cast<T>(it->second);
			}
		}

		uint64_t targetTypeId = T::TypeIdClass();
		auto pathName = mRootDirectory + assetName;
		auto asset = (customReader != nullptr ? customReader(pathName) : ReadAsset(targetTypeId, pathName));
		mLoadedAssets[assetName] = asset;

		return std::static_pointer_cast<T>(asset);
	}
}