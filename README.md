# LFMmap

-	MMAP的概念我在这里不多介绍了，这是基于MMAP的一套代码，可以借此实现高速KV,或者本地文件快速存储等功能，后续我会以此为基础，写一份OC基础上的本地文件日志写入助手用例。
- 	此代码非线程安全，需要在上层注意线程安全问题。
- 	基于c编写的mmap使用方法 接口如下：
	-	打开本地一个文件 如果没有会自动创建 
	``` 
		MMAP_FILE * open_mmapfile(const char * __restrict __filepath , int *error);
	```	 
	-	写入一个内容到文件中 内有扩容策略
	```
		int write_mmapfile(MMAP_FILE *model, const void * __restrict __ptr ,long start, size_t len , bool permit_dilatation);
	```
	- 	关闭
	```
		int close_mmapfile(MMAP_FILE *model);
	```  
	-  文件重置到初始化状态 既内容全为0的状态
	```
		int restroe_mmapfile(MMAP_FILE *model);
	```
	