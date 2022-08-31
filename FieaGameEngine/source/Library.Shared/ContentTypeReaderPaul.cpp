#include "pch.h"
#include "ContentTypeReaderPaul.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(AbstractContentTypeReaderPaul)

	uint64_t AbstractContentTypeReaderPaul::TargetTypeId() const
	{
		return mTargetTypeId;
	}

	AbstractContentTypeReaderPaul::AbstractContentTypeReaderPaul(GamePaul& game, const uint64_t targetTypeId) :
		mGame(&game), mTargetTypeId(targetTypeId)
	{
	}
}