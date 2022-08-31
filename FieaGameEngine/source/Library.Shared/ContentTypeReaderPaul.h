#pragma once

#include <memory>
#include <gsl\gsl>
#include "RTTI.h"

namespace FieaGameEngine
{
	class GamePaul;

	class AbstractContentTypeReaderPaul : public RTTI
	{
		RTTI_DECLARATIONS(AbstractContentTypeReaderPaul, RTTI)

	public:		
		AbstractContentTypeReaderPaul(const AbstractContentTypeReaderPaul&) = default;
		AbstractContentTypeReaderPaul& operator=(const AbstractContentTypeReaderPaul&) = default;
		AbstractContentTypeReaderPaul(AbstractContentTypeReaderPaul&&) = default;
		AbstractContentTypeReaderPaul& operator=(AbstractContentTypeReaderPaul&&) = default;
		virtual ~AbstractContentTypeReaderPaul() = default;

		std::uint64_t TargetTypeId() const;
		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) = 0;

	protected:
		AbstractContentTypeReaderPaul(GamePaul& game, const std::uint64_t targetTypeId);

		gsl::not_null<GamePaul*> mGame;
		const std::uint64_t mTargetTypeId;
	};

	template <typename T>
	class ContentTypeReaderPaul : public AbstractContentTypeReaderPaul
	{
	public:		
		ContentTypeReaderPaul(const ContentTypeReaderPaul&) = default;
		ContentTypeReaderPaul& operator=(const ContentTypeReaderPaul&) = default;
		ContentTypeReaderPaul(ContentTypeReaderPaul&&) = default;
		ContentTypeReaderPaul& operator=(ContentTypeReaderPaul&&) = default;
		virtual ~ContentTypeReaderPaul() = default;

		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) override;

	protected:
		ContentTypeReaderPaul(GamePaul& game, const std::uint64_t targetTypeId);

		virtual std::shared_ptr<T> _Read(const std::wstring& assetName) = 0;
	};
}

#include "ContentTypeReaderPaul.inl"