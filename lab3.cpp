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
					printf("运行时间%f秒\n",time);

int citymap[49][49];
int dis[48];     //到生成树的最短距离；
int vis[48];		//标记数组，标记该结点是否纳入集合，即该结点是否访问过；
int sum = 0;
int times = 0;
int p[48];		//保存父亲结点；
int judge[48]; // 判断节点是否在生成树
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

void inin()//初始化：mp数组初始化，vis标记数组初始化；
{
	int i, t = 0, j, m = 1, n;
	int x1, y1, x2, y2;
	FILE* fp;
	fp = fopen("lab3.dat", "r");
	memset(vis, 0, sizeof(vis));		//全部未访问过
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
void prime(int st, int n)	//st是任意一个开始的结点，此节点随意；
{
	int i, j = st, k;

	for (i = 1; i <= n; i++) {
		if (citymap[st][i] != 0x7ffff) {
			dis[i] = citymap[st][i];
			p[i] = 1;
			judge[i] = 0;
		}
	}
	judge[st] = 0;//节点1是根节点 

	for (i = 2; i <= n; i++) {
		int min = 0x7ffff;
		for (k = 2; k <= n; k++) {
			if ((!judge[k]) && (dis[k] < min)) {//判断节点k不在最小生成树中 
				min = dis[k];
				j = k;
			}
		}
		judge[j] = 1;

		for (k = 2; k <= n; k++) {
			if (!judge[k] && (citymap[j][k] < dis[k])) {//更新最小距离 
				dis[k] = citymap[j][k];
				p[k] = j;
			}
		}
		printf("%d的父节点为 %d\n", j, p[j]);

	}
	//printf("总代价为：%d", sum);
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
	sum = sum + citymap[1][finalcity[i]];//加上最后一个城市回到1城市的距离
	return sum;
}

int main() {

	Tree T;
	printf("接下来会打印48个州的TSP问题近似算法得出的一条路线\n");
	printf("该结果选定第一个城市作为开始节点\n");
	Time_init;
	Time_begin;

	inin();
	prime(1, 48);
	InitTree(T);
	CreateTree(T);
	printf("路径为：");
	printf("\n");
	PreOrder(T, 0);
	printf("1");//回到一城市
	Time_end;  //计时结束
	printf("\n");
	Time_out;  //结果
	printf("\n");
	printf("总的路径长度为%d", length());

	return 0;

}

