/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/

#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include "SSE.h"

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) DXException( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;
//Sizei Graphics::ScreenSize = { ScreenWidth, ScreenHeight };
//Sizef Graphics::fScreenSize = static_cast< Sizef >( Graphics::ScreenSize );

int   Graphics::ScreenWidth = 1920;
int   Graphics::ScreenHeight = 1080;
float Graphics::fScreenWidth = static_cast< float >( Graphics::ScreenWidth );
float Graphics::fScreenHeight = static_cast< float >( Graphics::ScreenHeight );
Sizei Graphics::ScreenSize = { ScreenWidth, ScreenHeight };
Sizef Graphics::fScreenSize = static_cast< Sizef >( Graphics::ScreenSize );
Recti Graphics::ScreenRect = { { 0, 0 }, ScreenSize };
Rectf Graphics::fScreenRect = static_cast< Rectf >( ScreenRect );

Graphics::Graphics( HWNDKey& key )
	:
	m_direct3d( key.hWnd, *this ),
	pBuffer( ScreenWidth, ScreenHeight )
{
}

void Graphics::EndFrame()
{
	m_direct3d.Present( pBuffer );
}

void Graphics::BeginFrame( Color C )
{
	// clear the sysbuffer
	//pSysBuffer.Fill( C );
	pBuffer.Clear();
}

void Graphics::PutPixel( int x, int y, int r, int g, int b )
{
	PutPixel( x, y, { unsigned char( r ), unsigned char( g ), unsigned char( b ) } );
}

void Graphics::PutPixel( int x, int y, Color c )
{
	PutPixel( buffer::index_t( x, y, pBuffer.extent() ), c );
}

void Graphics::PutPixel( const Index<2>& idx, Color c )
{
	assert( idx.Column() >= 0 );
	assert( idx.Column() < int( Graphics::ScreenWidth ) );
	assert( idx.Row() >= 0 );
	assert( idx.Row() < int( Graphics::ScreenHeight ) );

	pBuffer[ idx ] = c;
}

void Graphics::PutPixelAlpha( int X, int Y, Color C )
{	
	const auto index = Index<2>( X, Y, pBuffer.extent() );
	PutPixel( index, C.AlphaBlend( pBuffer[ index ] ) );
}

void Graphics::DrawCircle( const Vec2i & Center, int Radius, Color C )
{
	const auto sqRadius = sq( Radius );

	const Vec2i vRadius = { Radius, Radius };
	const auto bounds =
		Rectify( Recti( Center - vRadius, Center + vRadius ), ScreenRect );

	for2d( bounds.left, bounds.top, bounds.right, bounds.bottom, 
		[ &Center, sqRadius, this, C ]( Index<2>& idx )
	{
		const auto sqDist = sq( idx.Column() ) + sq( idx.Row() );
		if( sqDist < sqRadius )
		{
			PutPixel(
				static_cast< int32_t >( idx.Column() ) + Center.x,
				static_cast< int32_t >( idx.Row() ) + Center.y, C );
		}
	} );
}

void Graphics::DrawCircleAlpha( const Recti &Rect, Color C )
{
	const auto radius = Rect.GetWidth() / 2;
	const auto sqRadius = sq( radius );
	const auto sqInner = sq( std::max( radius >> 1, 1 ) );

	const auto center = Rect.GetCenter();
	const Vec2i vRadius = { radius, radius };
	const auto bounds =
		Rectify( Recti( center - vRadius, center + vRadius ), ScreenRect );

	const Color centerColor = Colors::White * ( C.Brightness() );

	for( int y = bounds.top - vRadius.y; y < bounds.bottom - vRadius.y; ++y )
	{
		for( int x = bounds.left - vRadius.x; x < bounds.right - vRadius.x; ++x )
		{
			const auto sqDist = sq( x ) + sq( y );
			if( sqDist < sqRadius )
			{
				const auto dist = sqrtf( static_cast< float >( sqDist ) );
				const auto step = dist / static_cast< float >( radius );

				const auto alpha =
					static_cast< unsigned char >( ( 1.f - step ) * 255.f );

				const Color color = ( sqDist < sqInner ) ?
					Color( centerColor, alpha ) :
					Color( C, alpha );
				
				PutPixelAlpha( x + center.x, y + center.y, color );
			}
		}
	}
}

void Graphics::DrawRect( const Recti& Rect, Color C )
{
	const auto bounds = Rectify( Rect, ScreenRect, true );

	for( int y = bounds.top; y < bounds.bottom; ++y )
	{
		for( int x = bounds.left; x < bounds.right; ++x )
		{
			PutPixel( x, y, C );
		}
	}
}
void Graphics::DrawRectAlpha( const Recti & Rect, Color C )
{
	const auto bounds = Rectify( Rect, ScreenRect, true );
	
	for( int y = bounds.top; y < bounds.bottom; ++y )
	{
		for( int x = bounds.left; x < bounds.right; ++x )
		{
			PutPixelAlpha( x, y, C );
		}
	}
}

void Graphics::SetResolution( const RECT & WinRect )
{
	const Recti &rect = *( reinterpret_cast< const Recti* >( &WinRect ) );
	ScreenWidth = rect.GetWidth();
	ScreenHeight = rect.GetHeight();
	fScreenWidth = static_cast< float >( ScreenWidth );
	fScreenHeight = static_cast< float >( ScreenHeight );
	ScreenSize = rect.GetSize();
	fScreenSize = static_cast< Sizef >( Graphics::ScreenSize );
	ScreenRect = rect;
	fScreenRect = static_cast< Rectf >( ScreenRect );
}


Graphics::Direct3D::Direct3D( HWND WinHandle, Graphics & Parent )
	:
	m_parent( Parent )
{
	assert( WinHandle != nullptr );

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
	sd.OutputWindow = WinHandle;
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
			throw CHILI_GFX_EXCEPTION( hr, L"Creating device and swap chain" );
		}
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		( LPVOID* )&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating render target view on backbuffer" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
														&srvDesc, &pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating view on sysBuffer texture" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating pixel shader" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex shader" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex buffer" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating input layout" );
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
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sampler state" );
	}

}

Graphics::Direct3D::~Direct3D()
{
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::Direct3D::Present( const buffer& pSysBuffer )
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(), 0u,
											 D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>( mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );

	auto idx = buffer::index_t( 0, 0, pSysBuffer.extent() );

	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++,idx += srcPitch )
	{
		memcpy( &pDst[ y * dstPitch ], &pSysBuffer[ idx ], rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(), 0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
	pImmediateContext->PSSetShaderResources( 0u, 1u, pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u, 1u, pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u, 0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr, L"Presenting back buffer" );
		}
	}
}
