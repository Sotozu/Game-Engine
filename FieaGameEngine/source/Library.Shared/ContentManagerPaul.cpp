#include "pch.h"
#include "ContentManagerPaul.h"
#include "ContentTypeReaderManagerPaul.h"
#include "GameException.h"

using namespace std;

namespace FieaGameEngine
{
	const wstring ContentManagerPaul::DefaultRootDirectory{ L"Content\\" };

	ContentManagerPaul::ContentManagerPaul(GamePaul& game, const wstring& rootDirectory) :
		mGame(game), mRootDirectory(rootDirectory)
	{
	}

	void ContentManagerPaul::AddAsset(const wstring& assetName, const shared_ptr<RTTI>& asset)
	{
		mLoadedAssets[assetName] = asset;
	}

	void ContentManagerPaul::RemoveAsset(const wstring& assetName)
	{
		mLoadedAssets.erase(assetName);
	}

	void ContentManagerPaul::Clear()
	{
		mLoadedAssets.clear();
	}

	shared_ptr<RTTI> ContentManagerPaul::ReadAsset(const int64_t targetTypeId, const wstring& assetName)
	{
		const auto& contentTypeReaders = ContentTypeReaderManagerPaul::ContentTypeReaders();
		auto it = contentTypeReaders.find(targetTypeId);
		if (it == contentTypeReaders.end())
		{
			throw GameException("Content type reader not registered.");
		}

		auto& reader = it->second;
		return reader->Read(assetName);
	}
}