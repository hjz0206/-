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
					printf("����ʱ��%f��\n",time);

int main() {

	int n, i = 0, j = 0, k;
	int overlap = 0, subnum = 0;
	

	printf("�����������ĳ����ļ�������7,20,29����ѡ�񣩣�\n");
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
		printf("����������ݲ���ȷ��\n");
	}
	fp = fopen("att7.txt", "r");
	//fp = fopen("att20.txt", "r");
	//fp = fopen("bays29.txt", "r");
	//�ⲿ�ִ���������ƽ̨��û�������п��Ա���ͨ������VS����Ҫ�ֶ�����ע�ͣ�Ҫ��Ȼ�����fpδ��ʼ�������⣡
	//����һ���ڽӾ����¼����
	int citymap[30][30] = { -1 }; //-1�����·��ͨ
	while (!feof(fp)) {
		fscanf(fp, "%d", &i);
		fscanf(fp, "%d", &j);
		fscanf(fp, "%d", &citymap[i][j]);
	}

	int** dp = (int**)malloc(sizeof(int) * n);
	for (k = 0; k < n; k++) {
		dp[k] = (int*)malloc(sizeof(int) * (1 << (n - 1))); //1<<(n-1)��2��n-1�η�
	}

	int** M = (int**)malloc(sizeof(int) * n);
	for (k = 0; k < n; k++) {
		M[k] = (int*)malloc(sizeof(int) * (1 << (n - 1))); //1<<(n-1)��2��n-1�η�
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

	for (int j = 1; j < (s - 1); j++)////���һ�в���ѭ�������
	{


		for (int i = 1; i < n; i++) //��1~(n-1)��n-1���㣬ӳ��Ϊ���϶�Ӧ�Ķ��������е�0~(n-2)λ
		{

			if ((j & (1 << (i - 1))) == 0)//i���ڼ�����
			{
				int m = 0x7ffff;
				for (int k = 1; k < n; k++)
				{
					overlap++;
					if ((j & (1 << (k - 1))) > 0)//k�ڼ�����
					{
						int tmp = dp[k][(j - (1 << (k - 1)))] + citymap[i][k];
						if (m > tmp) {
							m = tmp;
							dp[i][j] = m;
							M[i][j] = k;//�������ž���
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

	Time_end;  //��ʱ����
	Time_out;  //���

	printf("���·���ĳ����ǣ�%d\n", dp[0][s - 1]);

	printf("����·���ǣ�0");
	for (i = s - 1, j = 0; i > 0; )
	{
		j = M[j][i];//��һ��ȥ���ĸ����
		i = i - (1 <<(j - 1));//��i��ȥ��j���
		printf("->");
		printf("%d", j);
	}
	printf("->0\n");
	printf("����������Ϊ��%d\n", overlap);
	printf("���ص���������Ϊ��%d\n", subnum);


	free(dp);
	fclose(fp);
}