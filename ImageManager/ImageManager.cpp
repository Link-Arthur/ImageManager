#include "ImageManager.hpp"

#ifndef _DEBUG
#define nullptr NULL
#endif // !_DEBUG

#ifdef _DEBUG
#define DEBUG_LOG(result,expression) if (FAILED(result)){ MessageBox(nullptr, expression, L"Error", 0); return ;}
#define DEBUG_LOG_RETURN(result,expression,value) if (FAILED(result)){MessageBox(nullptr, expression, L"Error", 0); return value;}
#else 
#define DEBUG_LOG(result,expression) 
#define DEBUG_LOG_RETURN(result,expression,value)
#endif // _DEBUG


#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"windowscodecs.lib")



template<typename T>
void release(T& Interface) {
	if (Interface) Interface->Release();
	Interface = nullptr;
}

void Log(HRESULT result, LPCWSTR expression) {
	if (FAILED(result)) MessageBox(nullptr, expression, L"Error", 0);
}

ImageManager::ImagePool::ImagePool()
{
	HRESULT result = CoCreateInstance(CLSID_WICImagingFactory,
		nullptr, CLSCTX_INPROC, IID_IWICImagingFactory, (void **)(&source));

	D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
}

ImageManager::ImagePool::~ImagePool()
{
	release(source);
}

void ImageManager::ImagePool::CreateImage(std::wstring filename, Image* image)
{
	if (image->source)
		release(image->source);

	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICFormatConverter *pConverter = nullptr;

	HRESULT result;

	result = source->CreateDecoderFromFilename(&filename[0],
		nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	DEBUG_LOG(result, L"Load Image Failed.");

	result = pDecoder->GetFrame(0, &pSource);

	DEBUG_LOG(result, L"Get Image First Frame Failed");

	result = source->CreateFormatConverter(&pConverter);

	DEBUG_LOG(result, L"Create Format Converter Failed");

	result = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);

	DEBUG_LOG(result, L"Initalize Format Converter Failed");

	result = source->CreateBitmapFromSource(pConverter,
		WICBitmapCacheOnDemand, &image->source);

	DEBUG_LOG(result, L"Create Bitmap Source Failed");

	release(pDecoder);
	release(pSource);
	release(pConverter);
}

void ImageManager::ImagePool::WriteImage(std::wstring filename, Image* image)
{
	HRESULT result;

	IWICStream* stream;
	IWICBitmapEncoder* encoder;
	IWICBitmapFrameEncode* frame;

	result = source->CreateStream(&stream);

	DEBUG_LOG(result, L"Create Stream Failed For Write Image");

	result = stream->InitializeFromFilename(&filename[0], GENERIC_WRITE);

	DEBUG_LOG(result, L"Initalize Stream Failed For Write Image");

	result = source->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);

	DEBUG_LOG(result, L"Create Encoder Failed For Write Image");

	result = encoder->Initialize(stream, WICBitmapEncoderNoCache);

	DEBUG_LOG(result, L"Initalize Encoder Failed For Write Image");

	IPropertyBag2* pro;

	result = encoder->CreateNewFrame(&frame, &pro);

	DEBUG_LOG(result, L"Create Frame Failed For Write Image");

	result = frame->Initialize(pro);

	DEBUG_LOG(result, L"Initalize Frame Failed For Write Image");

	frame->SetSize(image->GetWidth(), image->GetHeight());

	WICPixelFormatGUID format;

	image->source->GetPixelFormat(&format);

	frame->SetPixelFormat(&format);

	WICRect rect = { 0,0,image->GetWidth(),image->GetHeight() };

	result = frame->WriteSource(image->source, &rect);

	DEBUG_LOG(result, L"Write Image Failed");
	
	result = frame->Commit();

	DEBUG_LOG(result, L"Commit Failed");
	
	result = encoder->Commit();

	DEBUG_LOG(result, L"Commit Failed");

	release(stream);
	release(encoder);
	release(frame);
	release(pro);
}

void ImageManager::ImagePool::CreateImageTarget(ImageTarget* target, int width, int height)
{
	HRESULT result;
	
	result = source->CreateBitmap((UINT)width, (UINT)height, GUID_WICPixelFormat32bppPRGBA,
		WICBitmapCreateCacheOption::WICBitmapCacheOnDemand, &target->source);

	DEBUG_LOG(result, L"Create BitMap Source Failed");

	result = factory->CreateWicBitmapRenderTarget(target->source,
		D2D1::RenderTargetProperties(), &target->rendertarget);

	DEBUG_LOG(result, L"Create BitMap Target Failed");
}

ImageManager::Image::Image()
{
	source = nullptr;
	lock = nullptr;
	
	stride = 0;
	buffsize = 0;

	pixel_source = nullptr;
}

ImageManager::Image::~Image()
{
	release(source);
}

int ImageManager::Image::GetWidth()
{
	UINT width;
	UINT height;
	source->GetSize(&width, &height);
	return width;
}

int ImageManager::Image::GetHeight()
{
	UINT width;
	UINT height;
	source->GetSize(&width, &height);
	return height;
}

int ImageManager::Image::BuffSize()
{
	return buffsize;
}

BYTE * ImageManager::Image::GetPixelData()
{
	HRESULT result;

	WICRect rect = { 0,0,GetWidth(),GetHeight() };

	result = source->Lock(&rect, WICBitmapLockWrite, &lock);

	DEBUG_LOG_RETURN(result, L"Lock PixelData Failed", nullptr);

	lock->GetStride(&stride);

	result = lock->GetDataPointer(&buffsize, &pixel_source);

	DEBUG_LOG_RETURN(result, L"Get PixelData Failed", nullptr);

	return pixel_source;
}

void ImageManager::Image::Flush()
{
	release(lock);
}

ImageManager::ImageTarget::ImageTarget()
{
	rendertarget = nullptr;
}

ImageManager::ImageTarget::~ImageTarget()
{
	release(rendertarget);
}

void ImageManager::ImageTarget::BeginDraw()
{
	rendertarget->BeginDraw();
}

void ImageManager::ImageTarget::EndDraw()
{
	rendertarget->EndDraw();
}

void ImageManager::ImageTarget::Clear(D2D1::ColorF color)
{
	rendertarget->Clear(color);
}

void ImageManager::ImageTarget::DrawImage(Image * image, float posx, float posy, float width, float height)
{
	HRESULT result;
	
	ID2D1Bitmap* bitmap = nullptr;

	result = rendertarget->CreateBitmapFromWicBitmap(image->source, &bitmap);

	DEBUG_LOG(result, L"Create Draw Bitmap Failed");

	rendertarget->DrawBitmap(bitmap, D2D1::RectF(posx, posy, posx + width, posy + height));

	release(bitmap);
}

