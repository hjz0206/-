#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <time.h>


#define Time_init	LARGE_INTEGER lFrequency; \
					QueryPerformanceFrequency(&lFrequency);  
#define Time_begin	LARGE_INTEGER lBeginCount; \
					QueryPerformanceCounter(&lBeginCount);
#define Time_end	LARGE_INTEGER lEndCount; \
					QueryPerformanceCounter(&lEndCount);
#define Time_out	double time = (double)(lEndCount.QuadPart - lBeginCount.QuadPart) / (double)lFrequency.QuadPart; \
					printf("运行时间%f秒\n",time);

#define T0 200000.0  // 初始温度
#define T_end (1e-10)
#define q  0.99   // 退火系数
#define L 1000  // 每个温度时的迭代次数，即链长

int citymap[49][49];
int tmp[144] = { 0 };
int city_list[48]; // 用于存放一个解

void distance()//初始化距离矩阵
{
	int i, j,t=0, m, n;
	int x1, y1, x2, y2;
	FILE* fp;
	fp = fopen("lab3.dat", "r");
	
	while (!feof(fp)) {
		fscanf(fp, "%d", &tmp[t]);
		t++;
	}
	for (m = 0; m < 48; m++) {
		i = tmp[m * 3];
		x1 = tmp[m * 3 + 1];
		y1 = tmp[m * 3 + 2];
		for (n = 0; n < 48; n++) {
			j = tmp[n * 3];
			x2 = tmp[n * 3 + 1];
			y2 = tmp[n * 3 + 2];
			int xd = x1 - x2;
			int yd = y1 - y2;
			double rj = sqrt(((xd * xd) + (yd * yd)) / 10.0);
			int  tj = (int)(rj + 0.5);
			if (tj < rj)
				citymap[i][j] = tj + 1;
			else
				citymap[i][j] = tj;
		}
	}
	for (int k = 0; k < 49; k++)  citymap[k][k] = 0;

	fclose(fp);
}

void init()
{
	for (int i = 0; i < 48; i++)
      city_list[i] = i + 1;  // 初始化一个解
 }

// 产生一个新解
// 此处采用随机交叉两个位置的方式产生新的解
void create_new()
{
	double r1 = ((double)rand()) /(RAND_MAX + 1.0);
	double r2 = ((double)rand()) / (RAND_MAX + 1.0);
	int pos1 = (int)(48 * r1); 
	int pos2 = (int)(48 * r2); //随机产生两个位置
	int temp = city_list[pos1];
	city_list[pos1] = city_list[pos2];
	city_list[pos2] = temp;   // 交换两个点
	 }

int path_len(int *citylist) {
	int i;
	int sum=0;
	for (i = 0; i < 47; i++) {
		sum = sum + citymap[citylist[i]][citylist[i + 1]];
	}
	sum = sum + citymap[citylist[47]][citylist[0]];//加上最后一个城市回到最初城市的距离
	return sum;
}
int main(void)
{
	
	srand((unsigned)time(NULL)); //初始化随机数种子
	
	double T;
	int count = 0; // 记录降温次数
	T = T0; //初始温度
	int city_list_copy[48]; // 用于保存原始解
	double f1, f2, df; //f1为初始解目标函数值，f2为新解目标函数值，df为二者差值
	double r; // 0-1之间的随机数，用来决定是否接受新解
	init(); //初始化一个解
	distance();
	Time_init;
	Time_begin;

	while (T > T_end) // 当温度低于结束温度时，退火结束
		{
		 for (int i = 0; i < L; i++)
			 {
			  memcpy(city_list_copy, city_list, 48 * sizeof(int)); // 复制数组
			  create_new(); // 产生新解
			  f1 = path_len(city_list_copy);
			  f2 = path_len(city_list);
			  df = f2 - f1;
			 // Metropolis准则
				if (df >= 0)
				 {
				 r = ((double)rand()) / (RAND_MAX);
				 //产生0到1的随机数
				 if (exp(-df / T) <= r) // 保留原来的解
					{
					memcpy(city_list, city_list_copy, 48 * sizeof(int));
					           }
				            }
			         }
		        T *= q; // 降温
	       count++;
		     }
	Time_end;  //计时结束
	Time_out;  //结果
  printf("采用模拟退火算法，初始温度T0=%.2f,降温系数q=%.2f,每个温度迭代%d次,共降温%d次，得到的TSP最优路径为:\n", T0, q, L, count);
  for (int i = 0; i < 47; i++)  // 输出最优路径
	   {
		       printf("%d->", city_list[i]);
	   }
  printf("%d\n", city_list[47]);
double len = path_len(city_list); // 最优路径长度
printf("最优路径长度为:%lf\n", len);
return 0;
}





