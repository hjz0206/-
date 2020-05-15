#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

#define Time_init	LARGE_INTEGER lFrequency; \
					QueryPerformanceFrequency(&lFrequency);  
#define Time_begin	LARGE_INTEGER lBeginCount; \
					QueryPerformanceCounter(&lBeginCount);
#define Time_end	LARGE_INTEGER lEndCount; \
					QueryPerformanceCounter(&lEndCount);
#define Time_out	double time = (double)(lEndCount.QuadPart - lBeginCount.QuadPart) / (double)lFrequency.QuadPart; \
					printf("运行时间%f秒\n",time);

int main() {

	int n, i = 0, j = 0, k;
	int overlap = 0, subnum = 0;
	

	printf("输入你想读入的城市文件名（有7,20,29三种选择）：\n");
	scanf("%d", &n);

	FILE* fp;
	if (n == 7) {
		fp= fopen("att7.txt", "r");
	}
	else if (n == 20) {
		fp = fopen("att20.txt", "r");
	}
	else if (n == 29) {
		fp = fopen("bays29.txt", "r");
	}
	else {
		printf("您输入的数据不正确！\n");
	}
	fp = fopen("att7.txt", "r");
	//fp = fopen("att20.txt", "r");
	//fp = fopen("bays29.txt", "r");
	//这部分代码在其他平台下没有这三行可以编译通过，但VS下需要手动更改注释，要不然会出现fp未初始化的问题！
	//创建一个邻接矩阵记录数据
	int citymap[30][30] = { -1 }; //-1代表此路不通
	while (!feof(fp)) {
		fscanf(fp, "%d", &i);
		fscanf(fp, "%d", &j);
		fscanf(fp, "%d", &citymap[i][j]);
	}

	int** dp = (int**)malloc(sizeof(int) * n);
	for (k = 0; k < n; k++) {
		dp[k] = (int*)malloc(sizeof(int) * (1 << (n - 1))); //1<<(n-1)是2的n-1次方
	}

	int** M = (int**)malloc(sizeof(int) * n);
	for (k = 0; k < n; k++) {
		M[k] = (int*)malloc(sizeof(int) * (1 << (n - 1))); //1<<(n-1)是2的n-1次方
	}

	int s = (1 << (n - 1));
	dp[0][0] = 0;
	for (int i = 1; i < n; i++)
	{
		dp[i][0] = citymap[i][0];
	}

	dp[0][(s - 1)] = 0x7ffff;

	Time_init;
	Time_begin;

	for (int j = 1; j < (s - 1); j++)////最后一列不在循环里计算
	{


		for (int i = 1; i < n; i++) //把1~(n-1)这n-1个点，映射为集合对应的二进制数中的0~(n-2)位
		{

			if ((j & (1 << (i - 1))) == 0)//i不在集合中
			{
				int m = 0x7ffff;
				for (int k = 1; k < n; k++)
				{
					overlap++;
					if ((j & (1 << (k - 1))) > 0)//k在集合中
					{
						int tmp = dp[k][(j - (1 << (k - 1)))] + citymap[i][k];
						if (m > tmp) {
							m = tmp;
							dp[i][j] = m;
							M[i][j] = k;//保存最优决策
							subnum++;
						}
					}
				}

			}
		}
	}
	dp[0][s-1]= 0x7ffff;
	for (int i = 1; i < n; i++)
	{
		int tmp = citymap[0][i] + dp[i][(s - 1) - (1 << (i - 1))];
		if (tmp < dp[0][s-1])
		{
			dp[0][s - 1] = tmp;
			M[0][s - 1] = i;
		}
	}

	Time_end;  //计时结束
	Time_out;  //结果

	printf("最短路径的长度是：%d\n", dp[0][s - 1]);

	printf("具体路径是：0");
	for (i = s - 1, j = 0; i > 0; )
	{
		j = M[j][i];//下一步去往哪个结点
		i = i - (1 <<(j - 1));//从i中去掉j结点
		printf("->");
		printf("%d", j);
	}
	printf("->0\n");
	printf("总子问题数为：%d\n", overlap);
	printf("非重叠子问题数为：%d\n", subnum);


	free(dp);
	fclose(fp);
}