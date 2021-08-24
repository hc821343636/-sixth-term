#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdlib.h>
#pragma pack(1) /* 必须在结构体定义之前使用,这是为了让结构体中各成员按1字节对齐*/
typedef struct tagBITMAPFILEHEADER
{
 unsigned short bfType;		 //保存图片类型。 'BM'(1-2字节) 
 unsigned long bfSize; 	 //位图文件的大小，以字节为单位（3-6字节，低位在前）
 unsigned short bfReserved1;//位图文件保留字，必须为0(7-8字节）
 unsigned short bfReserved2;//位图文件保留字，必须为0(9-10字节）
 unsigned long bfOffBits;  //RGB数据偏移地址,位图数据的起始位置，以相对于位图（11-14字节，低位在前）
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
 unsigned long 	biSize; 		 //本结构所占用字节数（15-18字节）
 unsigned long  biWidth; 	 //位图的宽度，以像素为单位（19-22字节）
 unsigned long  biHeight; 	 //位图的高度，以像素为单位（23-26字节）
 unsigned short biPlanes; 	 //目标设备的级别，必须为1(27-28字节）
 unsigned short biBitCount;  //每个像素所需的位数，必须是1（双色）（29-30字节）,4(16色），8(256色）16(高彩色)或24（真彩色）之一

 unsigned long  biCompression;//位图压缩类型，必须是0（不压缩），（31-34字节）
 //1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一

 unsigned long  biSizeImage;  //位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位（35-38字节）

 unsigned long  biXPelsPerMeter;//位图水平分辨率，每米像素数（39-42字节）
 unsigned long  biYPelsPerMeter;//位图垂直分辨率，每米像素数（43-46字节)
 unsigned long  biClrUsed; 		 //位图实际使用的颜色表中的颜色数（47-50字节）
 unsigned long  biClrImportant; //位图显示过程中重要的颜色数（51-54字节）
}BITMAPINFOHEADER;

void Bmp_Bigger_And_Smaller(BITMAPFILEHEADER head,BITMAPINFOHEADER info,double bili,char *a,char *b)
{
 	FILE *fpr1=fopen(a,"rb");
	FILE *fpw2=fopen(b,"wb");
	if(fpr1==NULL||fpw2==NULL)
	{
	printf("图片打开失败!\n");
	return ;
	}
	//读取原照片的头信息
	fread(&head,sizeof(BITMAPFILEHEADER),1,fpr1);
	fread(&info,sizeof(BITMAPINFOHEADER),1,fpr1);

	unsigned int old_width=info.biWidth;//获取原图片的宽
	unsigned int old_height=info.biHeight;//获取原图片的高

	//获取原图片的位图数据
	unsigned char *src_data=(unsigned char *)malloc(old_width*old_height*3);
	fseek(fpr1,54,SEEK_SET);			//随机定位，以字节数为1开始定位，后54位 
	fread(src_data,old_width*old_height*3,1,fpr1);

	printf("原图片的宽:%d\n",old_width);
	printf("原图片的高:%d\n",old_height);

	//修改原照片的宽高
	unsigned int new_width,new_height;
	printf("新图片的宽:%d \n",(int)bili*old_width);
	printf("新图片的高:%d \n",(int)bili*old_height);
	new_width=(int)bili*old_width;
	new_height=(int)bili*old_height;
	head.bfSize=new_width*new_height*3+54;
	info.biWidth=new_width;
	info.biHeight=new_height;

	//将修改过的头信息前54个字节，写进新照片
	fwrite(&head,sizeof(BITMAPFILEHEADER),1,fpw2);
	fwrite(&info,sizeof(BITMAPINFOHEADER),1,fpw2);
	//现在是把内容字节缩放，并且拷贝到put_data 
	int i=0,j=0;
	unsigned long dwsrcX,dwsrcY;
	unsigned char *pucDest;
	unsigned char *pucSrc;
	unsigned char *dest_data=(unsigned char *)malloc(new_width*new_height*3);
	//这个拷贝字节数的作用，先按照宽度，在按照高度。双重循环 
	for(i=0;i<new_height;i++)
	{
		dwsrcY=i/bili;
		pucDest=dest_data+i*new_width*3;		//下面完整的for循环，所存入的字节量 
		pucSrc=src_data+dwsrcY*old_width*3;		//缩小/放大了 
		for(j=0;j<new_width;j++)
		{
			dwsrcX=j/bili;
			memcpy(pucDest+j*3,pucSrc+dwsrcX*3,3);//数据拷贝
			//从源sourec中复制m个字节到目标destination中 ,(void* destination, void* sourec, unsigned m);
		}
	}
	fseek(fpw2,54,SEEK_SET);			//随机定位，以字节数为1开始定位，后54位 
	fwrite(dest_data,new_width*new_height*3,1,fpw2);
	printf("成功!\n");
	
	//释放堆空间,关闭文件
	free(dest_data);
	free(src_data);
	fclose(fpr1);
	fclose(fpw2);
}
int main(int argc, char* argv[])
{
	//定义原照片信息结构体
	BITMAPFILEHEADER old_head;
	BITMAPINFOHEADER old_info;
	//将结构体清空
	memset(&old_head,0,sizeof(BITMAPFILEHEADER));
	memset(&old_info,0,sizeof(BITMAPINFOHEADER));

  	double bili = atof(argv[2])/100.0;//把字符串转化为double型
  	printf("%f",bili);
  
	Bmp_Bigger_And_Smaller(old_head,old_info, bili,argv[1],argv[3]);
	return 0;
}


