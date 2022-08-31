#include "pch.h"
#include "FpsComponent.h"
#include "Game.h"

using namespace std;
using namespace std::literals;
using namespace DirectX;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(FpsComponent)

	FpsComponent::FpsComponent(Game& game) :
		DrawableGameComponent(game)		
	{
	}

	XMFLOAT2& FpsComponent::TextPosition()
	{
		return mTextPosition;
	}

	int FpsComponent::FrameCount() const
	{
		return mFrameCount;
	}

	int FpsComponent::FrameRate() const
	{
		return mFrameRate;
	}

	void FpsComponent::Initialize()
	{
		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");
	}

	void FpsComponent::Update(const GameState& gameState)
	{
		if ((gameState._gTime.TotalGameTime() - mLastTotalGameTime) >= 1s)
		{
			mLastTotalGameTime = gameState._gTime.TotalGameTime();
			mFrameRate = mFrameCount;
			mFrameCount = 0;
		}

		++mFrameCount;
	}

	void FpsComponent::Draw(const GameState& gameState)
	{
		mSpriteBatch->Begin();

		wostringstream fpsLabel;
		fpsLabel << setprecision(4) << L"Frame Rate: " << mFrameRate << L"    Total Elapsed Time: " << gameState._gTime.TotalGameTimeSeconds().count();
		mSpriteFont->DrawString(mSpriteBatch.get(), fpsLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
	}
}