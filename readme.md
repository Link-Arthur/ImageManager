# ImageManager

### 概述

简单的一个读取图片和对图片进行操作的库。

### ImagePool

读写图片的工厂，在进行读写图片的时候你需要创建这个。

最好使用new来创建他。

#### Read

```cpp

ImagePool pool = new ImagePool();

pool->CreateImage(FileName,&Image)

```

#### Write

```cpp
ImagePool pool = new ImagePool();

pool->WriteImage(FileName,&Image);
```

### ImageTarget

一块内存区域用来写入其他的位图，允许你将不同的位图写入到一块，按照时间顺序将会覆盖以前的位图。

看源码就知道有多偷懒了。

