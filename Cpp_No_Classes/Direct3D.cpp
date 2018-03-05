#include "Direct3D.h"
#include "Color.h"
#include "Window.h"
#include <cassert>

#pragma comment(lib, "d3d11.lib")

namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

namespace Framework::Direct3D
{
	_Direct3D::_Direct3D( Window::_Window& window )
	{
		//////////////////////////////////////////////////////
		// create device and swap chain/get render target view
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 1;
		sd.BufferDesc.Width = Graphics::ScreenWidth;
		sd.BufferDesc.Height = Graphics::ScreenHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window.hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		HRESULT				hr;
		UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

		// create device and front/back buffers
		if( FAILED( hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			nullptr,
			&pImmediateContext ) ) )
		{
			if( FAILED( hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_WARP,
				nullptr,
				createFlags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&sd,
				&pSwapChain,
				&pDevice,
				nullptr,
				&pImmediateContext ) ) )
			{
				//throw std::runtime_error("Creating device and swap chain" );
				throw std::runtime_error( "Creating device and swap chain" );
			}
		}

		// get handle to backbuffer
		ComPtr<ID3D11Resource> pBackBuffer;
		if( FAILED( hr = pSwapChain->GetBuffer(
			0,
			__uuidof( ID3D11Texture2D ),
			( LPVOID* )&pBackBuffer ) ) )
		{
			throw std::runtime_error( "Getting back buffer" );
		}

		// create a view on backbuffer that we can render to
		if( FAILED( hr = pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pRenderTargetView ) ) )
		{
			throw std::runtime_error( "Creating render target view on backbuffer" );
		}


		// set backbuffer as the render target using created view
		pImmediateContext->OMSetRenderTargets( 1, pRenderTargetView.GetAddressOf(), nullptr );


		// set viewport dimensions
		D3D11_VIEWPORT vp;
		vp.Width = float( Graphics::ScreenWidth );
		vp.Height = float( Graphics::ScreenHeight );
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pImmediateContext->RSSetViewports( 1, &vp );


		///////////////////////////////////////
		// create texture for cpu render target
		D3D11_TEXTURE2D_DESC sysTexDesc;
		sysTexDesc.Width = Graphics::ScreenWidth;
		sysTexDesc.Height = Graphics::ScreenHeight;
		sysTexDesc.MipLevels = 1;
		sysTexDesc.ArraySize = 1;
		sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sysTexDesc.SampleDesc.Count = 1;
		sysTexDesc.SampleDesc.Quality = 0;
		sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
		sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sysTexDesc.MiscFlags = 0;
		// create the texture
		if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc, nullptr, &pSysBufferTexture ) ) )
		{
			throw std::runtime_error( "Creating sysbuffer texture" );
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = sysTexDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		// create the resource view on the texture
		if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
			&srvDesc, &pSysBufferTextureView ) ) )
		{
			throw std::runtime_error( "Creating view on sysBuffer texture" );
		}


		////////////////////////////////////////////////
		// create pixel shader for framebuffer
		// Ignore the intellisense error "namespace has no member"
		if( FAILED( hr = pDevice->CreatePixelShader(
			FramebufferShaders::FramebufferPSBytecode,
			sizeof( FramebufferShaders::FramebufferPSBytecode ),
			nullptr,
			&pPixelShader ) ) )
		{
			throw std::runtime_error( "Creating pixel shader" );
		}


		/////////////////////////////////////////////////
		// create vertex shader for framebuffer
		// Ignore the intellisense error "namespace has no member"
		if( FAILED( hr = pDevice->CreateVertexShader(
			FramebufferShaders::FramebufferVSBytecode,
			sizeof( FramebufferShaders::FramebufferVSBytecode ),
			nullptr,
			&pVertexShader ) ) )
		{
			throw std::runtime_error( "Creating vertex shader" );
		}


		//////////////////////////////////////////////////////////////
		// create and fill vertex buffer with quad for rendering frame
		const FSQVertex vertices[] =
		{
			{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.5f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 0.5f, 0.0f, 1.0f },
		};

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( FSQVertex ) * 6;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;
		if( FAILED( hr = pDevice->CreateBuffer( &bd, &initData, &pVertexBuffer ) ) )
		{
			throw std::runtime_error( "Creating vertex buffer" );
		}


		//////////////////////////////////////////
		// create input layout for fullscreen quad
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// Ignore the intellisense error "namespace has no member"
		if( FAILED( hr = pDevice->CreateInputLayout( ied, 2,
			FramebufferShaders::FramebufferVSBytecode,
			sizeof( FramebufferShaders::FramebufferVSBytecode ),
			&pInputLayout ) ) )
		{
			throw std::runtime_error( "Creating input layout" );
		}


		////////////////////////////////////////////////////
		// Create sampler state for fullscreen textured quad
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc, &pSamplerState ) ) )
		{
			throw std::runtime_error( "Creating sampler state" );
		}


	}

	_Direct3D::~_Direct3D()
	{
		// clear the state of the device context before destruction
		if( pImmediateContext ) pImmediateContext->ClearState();
	}
}