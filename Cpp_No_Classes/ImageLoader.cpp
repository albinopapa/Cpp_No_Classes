#include "ImageLoader.h"
#include "Logger.h"
#include "ComponentInitter.h"
#include "DXException.h"

using Microsoft::WRL::ComPtr;
using namespace String;

ComPtr<IWICBitmap> ImageLoader::Load( const string & Filename )
{
	AutoLogger logger( GetFunctionName() );

	HRESULT hr = S_OK;
	ComPtr<IWICBitmapDecoder> pDecoder;
	if( FAILED( hr = WicInitter::Instance().GetFactory()->CreateDecoderFromFilename(
		Filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to load image file: " + Filename );
	}

	ComPtr<IWICBitmapFrameDecode> pFrame;
	if( FAILED( hr = pDecoder->GetFrame( 0, &pFrame ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to get the frame from image file." );
	}

	ComPtr<IWICFormatConverter> pConverter;
	if( FAILED( hr = WicInitter::Instance()->CreateFormatConverter( &pConverter ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to create format converter." );
	}
	if(FAILED(hr = pConverter->Initialize(
		pFrame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.f,
		WICBitmapPaletteTypeCustom ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to initizlie format converter." );
	}

	ComPtr<IWICBitmap> pBitmap;
	if( FAILED( hr = WicInitter::Instance()->CreateBitmapFromSource(
		pConverter.Get(),
		WICBitmapCacheOnDemand,
		&pBitmap ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to create bitmap." );
	}

	return pBitmap;
}
