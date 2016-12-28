#include<ImageManager.hpp>

using namespace ImageManager;

//���������ָ�룬��û����ʹ��Initalize������
ImagePool* pool;

int main() {
	//��ʼ��
	Initalize();

	//������Դ��
	pool = new ImagePool();

	Image IT;

	ImageTarget T;

	//�������λͼ
	pool->CreateImageTarget(&T, 4000, 4000);

	//����λͼ
	pool->CreateImage(L"Link.png", &IT);

	//�ڽ�λͼ���Ƶ�ʱ��ʹ��
	T.BeginDraw();

	//����λͼ
	T.DrawImage(&IT, 0, 0, 800, 600);
	T.DrawImage(&IT, 1000, 1000, 2000, 2000);

	T.EndDraw();

	//��λͼ����ļ�
	pool->WriteImage(L"LinkT.png", (Image*)&T);

	//������Դ
	delete pool;

}