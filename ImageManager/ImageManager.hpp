#pragma once

#include<string>
#include<d2d1.h>
#include<wincodec.h>


namespace ImageManager {


	//初始化，在程序开始的时候使用就好了。
	static void Initialize() {
		HRESULT result = CoInitialize(nullptr);
	}

	//图片类
	class Image {
	protected:
		IWICBitmap* source;

		IWICBitmapLock* lock;

		UINT stride;
		UINT buffsize;

		BYTE* pixel_source;

		friend class ImagePool;
		friend class ImageTarget;
	public:
		//构造函数
		Image();

		//析构函数
		~Image();

		//获取图片的宽度
		int GetWidth();

		//获取图片的高度
		int GetHeight();

		//获取图片的高度乘以宽度
		int BuffSize();

		//获取数组地址
		BYTE* GetPixelData();

		//修改完毕数组信息后，刷新
		void Flush();
	};

	class ImageTarget :Image {
	private:
		ID2D1RenderTarget* rendertarget;

		friend class ImagePool;
	public:
		//构造函数
		ImageTarget();

		//析构函数
		~ImageTarget();

		//在绘制前使用
		void BeginDraw();

		//绘制结束后使用
		void EndDraw();

		//清理这个位图，用一个颜色填充
		void Clear(D2D1::ColorF color = D2D1::ColorF::Black);

		//将一个位图绘制上去
		void DrawImage(Image* image, float posx, float posy, float width, float height);


	};


	//读取资源的类型
	class ImagePool {
	private:
		IWICImagingFactory* source;
		ID2D1Factory* factory;
	public:
		//构造函数
		ImagePool();
		//析构函数
		~ImagePool();

		//创建一个图片
		void CreateImage(std::wstring filename, Image* image);

		//输出图片
		void WriteImage(std::wstring filename, Image* image);

		//创建一个输出位图
		void CreateImageTarget(ImageTarget* target, int width, int height);

	};





}