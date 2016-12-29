/*

*/
#pragma once

#include<string>
#include<d2d1.h>
#include<wincodec.h>


namespace ImageManager {

	
	//��ʼ�����ڳ���ʼ��ʱ��ʹ�þͺ��ˡ�
	static void Initalize() {
		HRESULT result = CoInitialize(nullptr);
	}

	//ͼƬ��
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
		//���캯��
		Image();

		//��������
		~Image();

		//��ȡͼƬ�Ŀ��
		int GetWidth();

		//��ȡͼƬ�ĸ߶�
		int GetHeight();

		//��ȡͼƬ�ĸ߶ȳ��Կ��
		int BuffSize();

		//��ȡ�����ַ
		BYTE* GetPixelData();

		//�޸����������Ϣ��ˢ��
		void Flush();
	};

	class ImageTarget:Image {
	private:
		ID2D1RenderTarget* rendertarget;

		friend class ImagePool;
	public:
		//���캯��
		ImageTarget();
	
		//��������
		~ImageTarget();

		//�ڻ���ǰʹ��
		void BeginDraw();

		//���ƽ�����ʹ��
		void EndDraw();
		
		//�������λͼ����һ����ɫ���
		void Clear(D2D1::ColorF color = D2D1::ColorF::Black);

		//��һ��λͼ������ȥ
		void DrawImage(Image* image, float posx, float posy, float width, float height);


	};


	//��ȡ��Դ������
	class ImagePool {
	private:
		IWICImagingFactory* source;
		ID2D1Factory* factory;
	public:
		//���캯��
		ImagePool();
		//��������
		~ImagePool();

		//����һ��ͼƬ
		void CreateImage(std::wstring filename, Image* image);

		//���ͼƬ
		void WriteImage(std::wstring filename, Image* image);

		//����һ�����λͼ
		void CreateImageTarget(ImageTarget* target, int width, int height);

	};





}