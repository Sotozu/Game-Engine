namespace FieaGameEngine
{

#pragma region This is being slapped here for now because everything in this framework uses it.
	inline gsl::not_null<ID3D11Device5*> Game::Direct3DDevice() const
	{
		return mRenderer.Direct3DDevice();
	}

	inline gsl::not_null<ID3D11DeviceContext4*> Game::Direct3DDeviceContext() const
	{
		return mRenderer.Direct3DDeviceContext();
	}

	inline gsl::not_null<IDXGISwapChain1*> Game::SwapChain() const
	{
		return  mRenderer.SwapChain();
	}

	inline gsl::not_null<ID3D11RenderTargetView*> Game::RenderTargetView() const
	{
		return mRenderer.RenderTargetView();
	}

	inline gsl::not_null<ID3D11DepthStencilView*> Game::DepthStencilView() const
	{
		return mRenderer.DepthStencilView();
	}
#pragma endregion

	inline bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	inline const ServiceContainer& Game::Services() const
	{
		return mServices;
	}

	inline std::function<void* ()> Game::GetWindowCallback() const
	{
		return mGetWindow;
	}

	inline ContentManager& Game::Content()
	{
		return mContentManager;
	}



}