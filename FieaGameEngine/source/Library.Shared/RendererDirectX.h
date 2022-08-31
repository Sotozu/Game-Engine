#pragma once

#include <winrt\Windows.Foundation.h>
#include <functional>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "RenderTarget.h"
#include "AdapterReader.h"

using namespace gsl;


//I need to initialize directX with this class.
namespace FieaGameEngine
{
	//Need to ask Paul about this not sure what it does other than a being a sentinel.
	class IDeviceNotify
	{
	public:
		virtual ~IDeviceNotify() { };

		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;

	protected:
		IDeviceNotify() { };
	};


	class RendererDirectX final : RenderTarget
	{
		class Game;
		friend Game;

	public:
		RendererDirectX(std::function<void* ()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback);

		RendererDirectX(const RendererDirectX&) = delete;
		RendererDirectX& operator=(const RendererDirectX&) = delete;
		RendererDirectX(RendererDirectX&&) = delete;
		RendererDirectX& operator=(RendererDirectX&&) = delete;
		~RendererDirectX() = default;

		gsl::not_null<ID3D11Device5*> Direct3DDevice() const;
		gsl::not_null<ID3D11DeviceContext4*> Direct3DDeviceContext() const;
		gsl::not_null<IDXGISwapChain1*> SwapChain() const;
		gsl::not_null<ID3D11RenderTargetView*> RenderTargetView() const;
		gsl::not_null<ID3D11DepthStencilView*> DepthStencilView() const;

		SIZE RenderTargetSize() const;
		float AspectRatio() const;
		bool IsFullScreen() const;
		const D3D11_TEXTURE2D_DESC& BackBufferDesc() const;
		const D3D11_VIEWPORT& Viewport() const;
		std::uint32_t MultiSamplingCount() const;
		std::uint32_t MultiSamplingQualityLevels() const;
		
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);

		void Shutdown();

		void UpdateRenderTargetSize();
		std::function<void* ()> GetWindowCallback() const;

		void ClearScreen();
		HRESULT Present();

		void CreateDeviceResources();

		void CreateWindowSizeDependentResources();

		void CreateDeviceIndependentResources();

		void HandleDeviceLost();
		void Begin() override;
		void End() override;

		inline static const DirectX::XMVECTORF32 BackgroundColor{ DirectX::Colors::DarkSlateGray }; //TO DO: Fix this dependency.
		//inline static const DirectX::XMVECTORF32 BackgroundColor{ 230.0f,230.0f,230.0f }; //TO DO: Fix this dependency.

		//What is a "D3D_FEATURE_LEVEL" aka "Feature Level"?
#pragma region
/*
Describes the set of features targeted by a Direct3D device.
Set's up functionality like do we use DirectX9? DirectX10? DirectX11?
The defaulted level here is DirectX9.1 but...
There is a neat function that will determine if it is valid for our system.
That function will iterator through the levels to find one that it can support.
*/
#pragma endregion
		inline static const D3D_FEATURE_LEVEL DefaultFeatureLevel{ D3D_FEATURE_LEVEL_9_1 };


		inline static const std::uint32_t DefaultFrameRate{ 60 };
		inline static const std::uint32_t DefaultMultiSamplingCount{ 4 };
		inline static const std::uint32_t DefaultBufferCount{ 2 };
	private:
		//What is the "Direct3D 11 object model"
#pragma region
/*
The Direct3D 11 object model separates resource creation and rendering functionality into a device and one or more contexts;
this separation is designed to facilitate multi threading.
Check out the following website for more information:
https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro
*/
#pragma endregion

//What is a ID3D11Device5 aka "Direct3D Device"?
#pragma region
/*
A device is used to create resources and to enumerate the capabilities of a display adapter.In Direct3D 11, a device is represented with an ID3D11Device interface.
Each application must have at least one device, most applications only create one device.
Create a device for one of the hardware drivers installed on your machine by calling D3D11CreateDevice or D3D11CreateDeviceAndSwapChain and specify the driver type with the D3D_DRIVER_TYPE flag.
Each device can use one or more device contexts, depending on the functionality desired.
*/
#pragma endregion
		winrt::com_ptr<ID3D11Device5> mDirect3DDevice; //Paul - The device is an abstraction to the hardware.

//What is a ID3D11DeviceContext4 aka "Direct3D Device Context"?
#pragma region
/*
A device context contains the circumstance or setting in which a device is used.
More specifically, a device context is used to set pipeline state and generate rendering commands using the resources owned by a device.
Direct3D 11 implements two types of device contexts, one for immediate renderingand the other for deferred rendering;
both contexts are represented with an ID3D11DeviceContext interface.
*/
#pragma endregion
		winrt::com_ptr<ID3D11DeviceContext4> mDirect3DDeviceContext; //Paul - Device context is the environment working against the device.

// - What is a IDXGISwapChain1 aka "Swap Chain"?
#pragma region
/*
The idea of a swap chain is that what is currently being displayed onto the screen and that's stored in what's called the front buffer.
And then I have a back buffer and I'm rendering to the back buffer while the monitor is actively displaying the front buffer.
And when I'm done rendering to the back buffer, I swap the two and very commonly, I'll synchronize the swap of the front buffer in the back buffer to coincide with the vertical, refresh rate, the vsync of your display.
So if this is updating at 60 hertz, Then I'm going to time the swap in my swap chain to switch between the front buffer and the back buffer to coincide with that.
And the whole idea of the swap chain is to reduce tearing tearing is an artifact and Graphics that says, I don't want to display two different frames at the same time.
So I only want to display complete content. You can see this in lots of games. We're going to see it in our application because I'm not necessarily synchronizing with the vertical refresh rate.
So, that's the swap chain.
We're gonna have to build a swap chain in order to render to the screen.
*/
#pragma endregion


		//ADDED 6/22/2022
		bool InitializeShaders();

		winrt::com_ptr<IDXGISwapChain1> mSwapChain;
		D3D_FEATURE_LEVEL mFeatureLevel = DefaultFeatureLevel;

		D3D11_TEXTURE2D_DESC mBackBufferDesc;
		winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> mDepthStencilView;
		D3D11_VIEWPORT mViewport;

		std::uint32_t mFrameRate{ DefaultFrameRate };
		bool mIsFullScreen{ false };
		std::uint32_t mMultiSamplingCount{ DefaultMultiSamplingCount };
		std::uint32_t mMultiSamplingQualityLevels{ 0 };

		std::function<void* ()> mGetWindow;
		std::function<void(SIZE&)> mGetRenderTargetSize;
		SIZE mRenderTargetSize;
		IDeviceNotify* mDeviceNotify;

		winrt::com_ptr<ID3D11InputLayout> _inputLayout;

	};
}

#include "RendererDirectX.inl"