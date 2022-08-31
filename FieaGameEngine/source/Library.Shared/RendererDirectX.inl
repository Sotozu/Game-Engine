namespace FieaGameEngine
{
	inline gsl::not_null<ID3D11Device5*> RendererDirectX::Direct3DDevice() const
	{
		return gsl::not_null<ID3D11Device5*>(mDirect3DDevice.get());
	}

	inline gsl::not_null<ID3D11DeviceContext4*> RendererDirectX::Direct3DDeviceContext() const
	{
		return gsl::not_null<ID3D11DeviceContext4*>(mDirect3DDeviceContext.get());
	}

	inline gsl::not_null<IDXGISwapChain1*> RendererDirectX::SwapChain() const
	{
		return gsl::not_null<IDXGISwapChain1*>(mSwapChain.get());
	}

	inline gsl::not_null<ID3D11RenderTargetView*> RendererDirectX::RenderTargetView() const
	{
		return gsl::not_null<ID3D11RenderTargetView*>(mRenderTargetView.get());
	}

	inline gsl::not_null<ID3D11DepthStencilView*> RendererDirectX::DepthStencilView() const
	{
		return gsl::not_null<ID3D11DepthStencilView*>(mDepthStencilView.get());
	}

	inline SIZE RendererDirectX::RenderTargetSize() const
	{
		return mRenderTargetSize;
	}

	inline float RendererDirectX::AspectRatio() const
	{
		return static_cast<float>(mRenderTargetSize.cx) / mRenderTargetSize.cy;
	}

	inline bool RendererDirectX::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	inline const D3D11_TEXTURE2D_DESC& RendererDirectX::BackBufferDesc() const
	{
		return mBackBufferDesc;
	}

	inline const D3D11_VIEWPORT& RendererDirectX::Viewport() const
	{
		return mViewport;
	}

	inline std::uint32_t RendererDirectX::MultiSamplingCount() const
	{
		return mMultiSamplingCount;
	}

	inline std::uint32_t RendererDirectX::MultiSamplingQualityLevels() const
	{
		return mMultiSamplingQualityLevels;
	}


	inline void RendererDirectX::RegisterDeviceNotify(IDeviceNotify* deviceNotify)
	{
		mDeviceNotify = deviceNotify;
	}

	inline std::function<void* ()> RendererDirectX::GetWindowCallback() const
	{
		return mGetWindow;
	}

	inline HRESULT RendererDirectX::Present()
	{
		return mSwapChain->Present(1, 0);
	}

}