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
					printf("����ʱ��%f��\n",time);

#define T0 100000.0  // ��ʼ�¶�
#define T_end (1e-10)
#define q  0.99   // �˻�ϵ��
#define L 1000  // ÿ���¶�ʱ�ĵ���������������

int citymap[359][359];
int city_list[358]; // ���ڴ��һ����

void distance()//��ʼ���������
{
	int i, j;
	FILE* fp;
	fp = fopen("lab4.dat", "r");
	for (i = 1; i < 359; i++) {
		for (j = 1; j < 359; j++) {
			fscanf(fp, "%d", &citymap[i][j]);
		}
	}
	fclose(fp);
}

void init()
{
	for (int i = 0; i < 358; i++)
		city_list[i] = i + 1;  // ��ʼ��һ����
}

// ����һ���½�
// �˴����������������λ�õķ�ʽ�����µĽ�
void create_new()
{
	double r1 = ((double)rand()) / (RAND_MAX + 1.0);
	double r2 = ((double)rand()) / (RAND_MAX + 1.0);
	int pos1 = (int)(358 * r1);
	int pos2 = (int)(358 * r2); //�����������λ��
	int temp = city_list[pos1];
	city_list[pos1] = city_list[pos2];
	city_list[pos2] = temp;   // ����������
}

int path_len(int* citylist) {
	int i;
	int sum = 0;
	for (i = 0; i < 357; i++) {
		sum = sum + citymap[citylist[i]][citylist[i + 1]];
	}
	sum = sum + citymap[citylist[357]][citylist[0]];//�������һ�����лص�������еľ���
	return sum;
}
int main(void)
{

	srand((unsigned)time(NULL)); //��ʼ�����������

	double T;
	int count = 0; // ��¼���´���
	T = T0; //��ʼ�¶�
	int city_list_copy[358]; // ���ڱ���ԭʼ��
	double f1, f2, df; //f1Ϊ��ʼ��Ŀ�꺯��ֵ��f2Ϊ�½�Ŀ�꺯��ֵ��dfΪ���߲�ֵ
	double r; // 0-1֮�������������������Ƿ�����½�
	init(); //��ʼ��һ����
	distance();
	Time_init;
	Time_begin;

	while (T > T_end) // ���¶ȵ��ڽ����¶�ʱ���˻����
	{
		for (int i = 0; i < L; i++)
		{
			memcpy(city_list_copy, city_list, 358 * sizeof(int)); // ��������
			create_new(); // �����½�
			f1 = path_len(city_list_copy);
			f2 = path_len(city_list);
			df = f2 - f1;
			// Metropolis׼��
			if (df >= 0)
			{
				r = ((double)rand()) / (RAND_MAX);
				//����0��1�������
				if (exp(-df / T) <= r) // ����ԭ���Ľ�
				{
					memcpy(city_list, city_list_copy, 358 * sizeof(int));
				}
			}
		}
		T *= q; // ����
		count++;
	}
	Time_end;  //��ʱ����
	Time_out;  //���
	printf("����ģ���˻��㷨����ʼ�¶�T0=%.2f,����ϵ��q=%.2f,ÿ���¶ȵ���%d��,������%d�Σ��õ���TSP����·��Ϊ:\n", T0, q, L, count);
	for (int i = 0; i < 357; i++)  // �������·��
	{
		printf("%d->", city_list[i]);
	}
	printf("%d\n", city_list[357]); 
	double len = path_len(city_list); // ����·������
	printf("����·������Ϊ:%lf\n", len);
	return 0;
}
