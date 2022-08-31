#include "pch.h"
#include "RendererDirectX.h"
#include "GameException.h"
#include "DrawableGameComponent.h"
#include "DirectXHelper.h"
#include "ContentTypeReaderManager.h"

using namespace std;
using namespace gsl;
using namespace FieaGameEngine;
using namespace winrt;
using namespace DirectX;

namespace FieaGameEngine
{
	RendererDirectX::RendererDirectX(std::function<void* ()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) : 
		mGetWindow{ getWindowCallback }, 
		mGetRenderTargetSize{ getRenderTargetSizeCallback }
	{
	
		assert(getWindowCallback != nullptr);
		assert(mGetRenderTargetSize != nullptr);

		CreateDeviceIndependentResources();
		CreateDeviceResources();

	}

	void RendererDirectX::CreateWindowSizeDependentResources()
	{
#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		// Windows Phone does not support resizing the swap chain, so clear it instead of resizing.
		mSwapChain = nullptr;
#endif

		ID3D11RenderTargetView* nullViews[] = { nullptr };
		mDirect3DDeviceContext->OMSetRenderTargets(narrow_cast<uint32_t>(std::size(nullViews)), nullViews, nullptr);
		mRenderTargetView = nullptr;
		mDepthStencilView = nullptr;
		mDirect3DDeviceContext->Flush();

		mGetRenderTargetSize(mRenderTargetSize);
		if (mSwapChain == nullptr)
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };

			swapChainDesc.Width = mRenderTargetSize.cx;
			swapChainDesc.Height = mRenderTargetSize.cy;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			swapChainDesc.SampleDesc.Count = mMultiSamplingCount;
			swapChainDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
#else
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
#endif

			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = DefaultBufferCount;
#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#else
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
#endif

			com_ptr<IDXGIDevice4> dxgiDevice = mDirect3DDevice.as<IDXGIDevice4>();
			assert(dxgiDevice != nullptr);

			com_ptr<IDXGIAdapter> dxgiAdapter;
			ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.put()));

			com_ptr<IDXGIFactory5> dxgiFactory;
			ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.put())));

			void* window = mGetWindow();

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{ 0 };
			fullScreenDesc.RefreshRate.Numerator = mFrameRate;
			fullScreenDesc.RefreshRate.Denominator = 1;
			fullScreenDesc.Windowed = !mIsFullScreen;

			ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(mDirect3DDevice.get(), reinterpret_cast<HWND>(window), &swapChainDesc, &fullScreenDesc, nullptr, mSwapChain.put()), "IDXGIDevice::CreateSwapChainForHwnd() failed.");
#else
			ThrowIfFailed(dxgiFactory->CreateSwapChainForCoreWindow(mDirect3DDevice.get(), reinterpret_cast<IUnknown*>(window), &swapChainDesc, nullptr, mSwapChain.put()), "IDXGIFactory2::CreateSwapChainForCoreWindow() failed.");
#endif

			ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
		}
		else
		{
			// Resize existing swap chain
			HRESULT hr = mSwapChain->ResizeBuffers(DefaultBufferCount, mRenderTargetSize.cx, mRenderTargetSize.cy, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				// If the device was removed for any reason, a new device and swap chain will need to be created.
				HandleDeviceLost();

				// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
				// and correctly set up the new device.
				return;
			}
			else
			{
				ThrowIfFailed(hr, "IDXGISwapChain1::ResizeBuffers() failed.");
			}
		}

		// Create a render target view
		com_ptr<ID3D11Texture2D> backBuffer;
		ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.put())), "IDXGISwapChain1::GetBuffer() failed.");
		backBuffer->GetDesc(&mBackBufferDesc);
		ThrowIfFailed(mDirect3DDevice->CreateRenderTargetView(backBuffer.get(), nullptr, mRenderTargetView.put()), "IDXGIDevice::CreateRenderTargetView() failed.");

		// Create a depth-stencil view
		D3D11_TEXTURE2D_DESC depthStencilDesc{ 0 };
		depthStencilDesc.Width = mRenderTargetSize.cx;
		depthStencilDesc.Height = mRenderTargetSize.cy;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
		depthStencilDesc.SampleDesc.Count = mMultiSamplingCount;
		depthStencilDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
#else
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
#endif

		com_ptr<ID3D11Texture2D> depthStencilBuffer;
		ThrowIfFailed(mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.put()), "IDXGIDevice::CreateTexture2D() failed.");

#if (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2DMS);
#else
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
#endif
		ThrowIfFailed(mDirect3DDevice->CreateDepthStencilView(depthStencilBuffer.get(), &depthStencilViewDesc, mDepthStencilView.put()), "IDXGIDevice::CreateDepthStencilView() failed.");

		// Set the viewport to the entire window
		mViewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(mRenderTargetSize.cx), static_cast<float>(mRenderTargetSize.cy));

		// Set render targets and viewport through render target stack	
		Begin();
	}

	void RendererDirectX::HandleDeviceLost()
	{
		mSwapChain = nullptr;

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceLost();
		}

		CreateDeviceResources();
		CreateWindowSizeDependentResources();

		if (mDeviceNotify != nullptr)
		{
			mDeviceNotify->OnDeviceRestored();
		}
	}

	void RendererDirectX::CreateDeviceResources()
	{
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		uint32_t createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		if (SdkLayersAvailable())
		{
			// If the project is in a debug build, enable debugging via SDK Layers with this flag.
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		// Create the Direct3D device object and a corresponding context.
		com_ptr<ID3D11Device> direct3DDevice; //We are using the winrt::com_ptr wrapper here as a smartpointer so that we don't have to explicity call ReleaseObject on it in code.
		com_ptr<ID3D11DeviceContext> direct3DDeviceContext;

		ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, narrow_cast<uint32_t>(size(featureLevels)), D3D11_SDK_VERSION, direct3DDevice.put(), &mFeatureLevel, direct3DDeviceContext.put()), "D3D11CreateDevice() failed");
		mDirect3DDevice = direct3DDevice.as<ID3D11Device5>();
		assert(mDirect3DDevice != nullptr);

		mDirect3DDeviceContext = direct3DDeviceContext.as<ID3D11DeviceContext4>();
		assert(mDirect3DDeviceContext != nullptr);

		ThrowIfFailed(mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingQualityLevels), "CheckMultisampleQualityLevels() failed.");
		if (mMultiSamplingQualityLevels == 0)
		{
			throw GameException("Unsupported multi-sampling quality");
		}

#ifndef NDEBUG
		com_ptr<ID3D11Debug> d3dDebug = mDirect3DDevice.as<ID3D11Debug>();
		if (d3dDebug)
		{
			com_ptr<ID3D11InfoQueue> d3dInfoQueue = d3dDebug.as<ID3D11InfoQueue>();
			if (d3dInfoQueue)
			{
#ifdef _DEBUG
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
				d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
				D3D11_MESSAGE_ID hide[] =
				{
					D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
				};
				D3D11_INFO_QUEUE_FILTER filter{ 0 };
				filter.DenyList.NumIDs = narrow_cast<uint32_t>(size(hide));
				filter.DenyList.pIDList = hide;
				d3dInfoQueue->AddStorageFilterEntries(&filter);
			}
		}
#endif
	}

	void RendererDirectX::Begin()
	{
		ID3D11RenderTargetView* views[] = { mRenderTargetView.get() };
		const span<ID3D11RenderTargetView*> renderTargetViews{ views };
		RenderTarget::Begin(Direct3DDeviceContext(), renderTargetViews, not_null<ID3D11DepthStencilView*>(mDepthStencilView.get()), mViewport);
	}

	void RendererDirectX::End()
	{
		RenderTarget::End(Direct3DDeviceContext());
	}

	bool RendererDirectX::InitializeShaders()
	{
		//Delete this method once complete
		//Done in video 10 of the series on youtube.
		return false;
	}

	void RendererDirectX::Shutdown()
	{
		mDirect3DDeviceContext->ClearState();
		mDirect3DDeviceContext->Flush();

		mDepthStencilView = nullptr;
		mRenderTargetView = nullptr;
		mSwapChain = nullptr;
		mDirect3DDeviceContext = nullptr;
		mDirect3DDevice = nullptr;
#if defined(DEBUG) || defined(_DEBUG)
		DumpD3DDebug();
#endif
	}

	void RendererDirectX::CreateDeviceIndependentResources() {}

	void RendererDirectX::UpdateRenderTargetSize()
	{
		CreateWindowSizeDependentResources();
	}

	void RendererDirectX::ClearScreen()
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}
}