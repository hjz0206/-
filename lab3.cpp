#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include<stack>
#include<queue>
#include<algorithm>


#define Time_init	LARGE_INTEGER lFrequency; \
					QueryPerformanceFrequency(&lFrequency);  
#define Time_begin	LARGE_INTEGER lBeginCount; \
					QueryPerformanceCounter(&lBeginCount);
#define Time_end	LARGE_INTEGER lEndCount; \
					QueryPerformanceCounter(&lEndCount);
#define Time_out	double time = (double)(lEndCount.QuadPart - lBeginCount.QuadPart) / (double)lFrequency.QuadPart; \
					printf("����ʱ��%f��\n",time);

int citymap[49][49];
int dis[48];     //������������̾��룻
int vis[48];		//������飬��Ǹý���Ƿ����뼯�ϣ����ý���Ƿ���ʹ���
int sum = 0;
int times = 0;
int p[48];		//���游�׽�㣻
int judge[48]; // �жϽڵ��Ƿ���������
int finalcity[49];
int tmp[144] = { 0 };
typedef struct TNode
{
	int data;
	int parent;
}TNode;

typedef struct Tree
{
	TNode parent[100];
	int  NodeNum;
}Tree;

void inin()//��ʼ����mp�����ʼ����vis��������ʼ����
{
	int i, t = 0, j, m = 1, n;
	int x1, y1, x2, y2;
	FILE* fp;
	fp = fopen("lab3.dat", "r");
	memset(vis, 0, sizeof(vis));		//ȫ��δ���ʹ�
	while (!feof(fp)) {
		fscanf(fp, "%d", &tmp[t]);
		//printf("%d\t", tmp[t]);
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
			//printf("%d\t", i);
			//printf("%d\t", j);
			//printf("%d\n", citymap[i][j]);
		}
	}
	for (int k = 0; k < 49; k++)  citymap[k][k] = 0;

	fclose(fp);

}
void prime(int st, int n)	//st������һ����ʼ�Ľ�㣬�˽ڵ����⣻
{
	int i, j = st, k;

	for (i = 1; i <= n; i++) {
		if (citymap[st][i] != 0x7ffff) {
			dis[i] = citymap[st][i];
			p[i] = 1;
			judge[i] = 0;
		}
	}
	judge[st] = 0;//�ڵ�1�Ǹ��ڵ� 

	for (i = 2; i <= n; i++) {
		int min = 0x7ffff;
		for (k = 2; k <= n; k++) {
			if ((!judge[k]) && (dis[k] < min)) {//�жϽڵ�k������С�������� 
				min = dis[k];
				j = k;
			}
		}
		judge[j] = 1;

		for (k = 2; k <= n; k++) {
			if (!judge[k] && (citymap[j][k] < dis[k])) {//������С���� 
				dis[k] = citymap[j][k];
				p[k] = j;
			}
		}
		printf("%d�ĸ��ڵ�Ϊ %d\n", j, p[j]);

	}
	//printf("�ܴ���Ϊ��%d", sum);
}

void InitTree(Tree& T)

{
	for (int i = 0; i < 49; i++) {
		T.parent[i].data = -1;
		T.parent[i].parent = -1;

	}
	T.NodeNum = 0;
}

bool InsertNode(Tree& T, int node) {
	if (node != -1) {
		T.parent[T.NodeNum].data = node;
		T.NodeNum++;
		return true;
	}
	return false;
}

bool InsertParent(Tree& T, int node1, int node2) {
	int place1, place2;
	place1 = -1; place2 = -1;
	for (int i = 0; i < T.NodeNum; i++) {
		if (node1 == T.parent[i].data)
			place1 = i;
		if (node2 == T.parent[i].data)
			place2 = i;
	}
	if (place1 != -1 && place2 != -1) {
		T.parent[place2].parent = place1;
		return true;
	}
	return false;
}
void CreateTree(Tree& T) {
	int nodenum = 48;
	int parent = 47;
	int i = 1, j = 2;
	int node, node1, node2;
	while (nodenum > 0) {
		InsertNode(T, i);
		nodenum--;
		i++;
	}
	while (parent > 0) {
		InsertParent(T, p[j], j);
		//printf("%d %d\n", T.parent[j-1].data, T.parent[j-1].parent);
		parent--;
		j++;
	}
}

void PreOrder(Tree T, int i) {
	if (T.NodeNum != 0) {
		printf("%d->", T.parent[i].data);
		finalcity[times++] = T.parent[i].data;
		for (int j = 0; j < T.NodeNum; j++) {
			if (T.parent[j].parent == i) {
				PreOrder(T, j);
				//printf("%d\t", j);
			}
		}

	}
}
int length() {
	int i;
	for (i = 0; i < 47; i++) {
		sum = sum + citymap[finalcity[i]][finalcity[i + 1]];
	}
	sum = sum + citymap[1][finalcity[i]];//�������һ�����лص�1���еľ���
	return sum;
}

int main() {

	Tree T;
	printf("���������ӡ48���ݵ�TSP��������㷨�ó���һ��·��\n");
	printf("�ý��ѡ����һ��������Ϊ��ʼ�ڵ�\n");
	Time_init;
	Time_begin;

	inin();
	prime(1, 48);
	InitTree(T);
	CreateTree(T);
	printf("·��Ϊ��");
	printf("\n");
	PreOrder(T, 0);
	printf("1");//�ص�һ����
	Time_end;  //��ʱ����
	printf("\n");
	Time_out;  //���
	printf("\n");
	printf("�ܵ�·������Ϊ%d", length());

	return 0;

}

