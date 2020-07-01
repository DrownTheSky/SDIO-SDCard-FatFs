#include "stm32f4xx.h"
#include "./usart/bsp_usart.h"
#include "./sdio/bsp_sdio_sd.h"
#include "ff.h"
#include <string.h>

void Fatfs_Mkfs(void);
void FatFs_Read(void);
void FatFs_Write(void);
void FatFs_Read_Write(void);


FATFS fs;
FRESULT fr;
FIL fil;

int main(void)
{
	USART_Config();
	fr = f_mount(&fs, "", 1);
	if (fr == 0)
		printf("\nFatFs挂载成功\n");
	else
		printf("\nFatFs挂载失败,错误代码(%d)\n", fr);

	Fatfs_Mkfs();
	FatFs_Read_Write();
	FatFs_Write();
	FatFs_Read();

	while(1)
	{
		
	}
}

void Fatfs_Mkfs(void)
{
	fr = f_mkfs("0:", 0, 0);

	if (fr == 0)
		printf("\n格式化成功\n");
	else
		printf("\n格式化失败,错误代码(%d)\n", fr);
}

void FatFs_Read_Write(void)
{
	uint8_t Temp = 0;
	UINT bw;
	uint8_t str[100] = "FATFS读写文件测试";

	fr = f_open(&fil, "0:Test.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (fr == 0)
		printf("\n文件打开成功\n");
	else
		printf("\n文件打开失败,错误代码(%d)\n", fr);

	fr = f_write(&fil, &str, strlen((char *)str), &bw);
	if (fr == 0)
		printf("\n文件写入成功,写入字节数 %d \n", bw);
	else
		printf("\n文件写入失败,错误代码(%d)\n", fr);

	fr = f_lseek(&fil, f_size(&fil));
	if (fr == 0)
		printf("\n偏移光标成功,偏移数 %d \n", (int)f_size(&fil));
	else
		printf("\n偏移光标失败\n");

	Temp += f_printf(&fil, "\n添加新内容\n");
	Temp += f_printf(&fil, "当前字符数 ：%d\n", f_size(&fil));
	if (Temp > 0)
		printf("\n添加新内容成功,字节数为 ：%d\n", Temp);
	else
		printf("\n添加新内容失败\n");

	fr = f_lseek(&fil, 0);
	if (fr == 0)
		printf("\n偏移光标成功,偏移数 %d \n", 0);
	else
		printf("\n偏移光标失败\n");

	fr = f_read(&fil, &str, f_size(&fil), &bw);
	if (fr == 0)
		printf("\n文件读取成功,读取字节数 %d \n读取内容 ：\n%s\n", bw, str);
	else
		printf("\n文件写入失败,错误代码(%d)\n", fr);

	fr = f_close(&fil);
	if (fr == 0)
		printf("\n文件关闭成功\n");
	else
		printf("\n文件关闭失败,错误代码(%d)\n", fr);
}

void FatFs_Read(void)
{
	UINT bw;
	uint8_t str[100];
	
	fr = f_open(&fil, "0:Test.txt", FA_READ);
	if (fr == 0)
		printf("\n文件打开成功\n");
	else
		printf("\n文件打开失败,错误代码(%d)\n", fr);
	
	fr = f_lseek(&fil, 0);
	if (fr == 0)
		printf("\n偏移光标成功,偏移数 %d \n", 0);
	else
		printf("\n偏移光标失败\n");
	
	fr = f_read(&fil, &str, sizeof(str), &bw);
	if (fr == 0)
		printf("\n文件读取成功,读取字节数 %d \n读取内容 ：\n%s\n", bw, str);
	else
		printf("\n文件写入失败,错误代码(%d)\n", fr);

	fr = f_close(&fil);
	if (fr == 0)
		printf("\n文件关闭成功\n");
	else
		printf("\n文件关闭失败,错误代码(%d)\n", fr);
}

void FatFs_Write(void)
{
	UINT bw;
	uint8_t str[] = "123一二三abc";
	
	fr = f_open(&fil, "0:Test.txt", FA_WRITE);
	if (fr == 0)
		printf("\n文件打开成功\n");
	else
		printf("\n文件打开失败,错误代码(%d)\n", fr);
	
	fr = f_lseek(&fil, f_size(&fil));
	if (fr == 0)
		printf("\n偏移光标成功,偏移数 %d \n", (int)f_size(&fil));
	else
		printf("\n偏移光标失败\n");
	
	fr = f_write(&fil, &str, strlen((char *)str), &bw);
	if (fr == 0)
		printf("\n文件写入成功,写入字节数 %d \n", bw);
	else
		printf("\n文件写入失败,错误代码(%d)\n", fr);

	fr = f_close(&fil);
	if (fr == 0)
		printf("\n文件关闭成功\n");
	else
		printf("\n文件关闭失败,错误代码(%d)\n", fr);
}
