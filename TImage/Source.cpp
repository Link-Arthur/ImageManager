#include<ImageManager.hpp>

using namespace ImageManager;

//这个必须用指针，你没法在使用Initalize定义他
ImagePool* pool;

int main() {
	//初始化
	Initalize();

	//创建资源库
	pool = new ImagePool();

	Image IT;

	ImageTarget T;

	//创建输出位图
	pool->CreateImageTarget(&T, 4000, 4000);

	//创建位图
	pool->CreateImage(L"Link.png", &IT);

	//在将位图绘制的时候使用
	T.BeginDraw();

	//绘制位图
	T.DrawImage(&IT, 0, 0, 800, 600);
	T.DrawImage(&IT, 1000, 1000, 2000, 2000);

	T.EndDraw();

	//将位图输出文件
	pool->WriteImage(L"LinkT.png", (Image*)&T);

	//清理资源
	delete pool;

}