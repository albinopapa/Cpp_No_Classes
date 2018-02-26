#pragma once

#include "LeanWindows.h"
#include <d3d11.h>
#include <wrl/client.h>

// vertex format for the framebuffer fullscreen textured quad
struct FSQVertex
{
	float x, y, z;		// position
	float u, v;			// texcoords
};

namespace Color{ struct _Color; }
namespace Window{ struct _Window; }

namespace Direct3D
{
	using Microsoft::WRL::ComPtr;
	struct _Direct3D
	{
		_Direct3D( Window::_Window& window );
		~_Direct3D();

		ComPtr<ID3D11Device>				pDevice;
		ComPtr<ID3D11DeviceContext>			pImmediateContext;
		ComPtr<IDXGISwapChain>				pSwapChain;
		ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
		ComPtr<ID3D11Texture2D>				pSysBufferTexture;
		ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
		ComPtr<ID3D11PixelShader>			pPixelShader;
		ComPtr<ID3D11VertexShader>			pVertexShader;
		ComPtr<ID3D11Buffer>				pVertexBuffer;
		ComPtr<ID3D11InputLayout>			pInputLayout;
		ComPtr<ID3D11SamplerState>			pSamplerState;
	};

	void Present( const _Direct3D& d3d, const Color::_Color* pBuffer );
}