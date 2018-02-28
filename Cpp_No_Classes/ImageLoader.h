#pragma once

#include "string_util.h"
#include <wincodec.h>
#include <wrl/client.h>

// ImageLoader loads .bmp, .jpg and .png files.
class ImageLoader
{
public:
	static Microsoft::WRL::ComPtr<IWICBitmap> Load( const String::string &Filename );
};

