#ifndef _DIJKSTRA_HPP_
#define _DIJKSTRA_HPP_

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stack>

#include <iostream>  
#include <cstdio>  
#include <queue>  
#include <vector>  

using namespace std;  

typedef int* Matrix_i2v2;
typedef float* Matrix_f2v2;
extern const float INF;

/*
void print_follow(int *CrossOrder, int s,int n, float *distance, int *record, int finalpoint)
{
	std::stack<int> q;
	int i = finalpoint;
	int jumpoff = 0;
	int p=i;
	while(record[p]!=-1)
	{
		//std::cout<< "Here" <<std::endl;
		q.push(p);
		p=record[p];
	}
	q.push(p);
	//std::cout<<s+1<<"-->"<<i+1<<" ";
	//std::cout<<"distance: "<<distance[i]<<" ";
	//std::cout<<s+1;
	CrossOrder[0] = s+1;
	if(jumpoff == 0)
	{
		q.top();
		q.pop();
		jumpoff = 1;
	}
	
	int out = 1;
	while(!q.empty())
	{
		CrossOrder[out] = q.top()+1;
		//std::cout<<"-->"<<q.top()+1;
		q.pop();
		out++;
	}
	//std::cout<<std::endl;
}
*/
/*
int Dijkstra(int *CrossOrder, int startpoint, int finalpoint, int crossnum, Matrix_f2v2 map, float *distance, bool *flag)
{
	int record[crossnum] = {0};
	for(int i = 0; i < crossnum; i++)
	{
		record[i] = -1;
	}
	distance[startpoint] = 0;
	for(int i = 0; i < crossnum; i++)
	{
		//找到和起点距离最短的点
		float minx = INF;
		int minmark = 0;
		for(int j = 0; j < crossnum; j++)
		{
			if(flag[j]==false && distance[j]<=minx)
			{
				minx = distance[j];
				minmark = j;
			}
		}
		//并标记
		flag[minmark] = true;
		//更新所有和它连接的点的距离
		for(int j = 0; j < crossnum; j++)
		{
			if(flag[j] == false && distance[j] > distance[minmark] + *(map + minmark*crossnum + j))
			{
				distance[j] = distance[minmark] + *(map + minmark*crossnum + j);
				record[j] = minmark;
			}
		}
	}
	print_follow(CrossOrder, startpoint, crossnum, distance, record, finalpoint);

	return 1;
}
*/
/*
struct node
{  
    int x;
    float d;  
    node(){}  
    node(int a,float b){x=a;d=b;}  
    bool operator < (const node & a) const  
    {  
        if(d==a.d) return x<a.x;  
        else return d > a.d;  
    }  
};  

int Dijkstra_new(int *CrossOrder, int startpoint, int finalpoint, int crossnum, Matrix_f2v2 map)  
{
	vector<node> eg[crossnum];
	float distance[crossnum] = {0};
    for(int i = 0; i < crossnum; i++)
	{
		for(int j = 0; j < crossnum; j++)
		{
			eg[i].emplace_back(node(j,*(map + i*crossnum + j)));
		}
	}
    for(int i=0;i < crossnum;i++)
    {
		distance[i]=INF;  
    }
    int record[crossnum] = {0};
	for(int i = 0; i < crossnum; i++)
	{
		record[i] = -1;
	}
    distance[startpoint] = 0;  
    
    //用优先队列优化  
    priority_queue<node> q;  
    q.push(node(startpoint,distance[startpoint]));  
    while(!q.empty())  
    {  
        node x=q.top();
        q.pop();  
        for(unsigned int i=0;i<eg[x.x].size();i++)  
        {  
            node y = eg[x.x][i];  
            if(distance[y.x]>x.d+y.d)  
            {  
                distance[y.x] = x.d+y.d;  
                q.push(node(y.x,distance[y.x]));  
                record[i] = x.x;
            }  
        }  
    } 
    print_follow(CrossOrder, startpoint, crossnum, distance, record, finalpoint);
    
    return 0;  
}  */

typedef int Patharc[200][200];
typedef int ShortPathTable[200][200];

typedef struct {
    int vex[200];
    int arc[200][200];
    int numVertexes;
} MGraph;

// 构建图
void CreateMGraph(MGraph *G, Matrix_f2v2 map, int crossnum){
    int i, j;

    // 初始化图
    G->numVertexes = 9;
    for(i = 0; i < G->numVertexes; ++i){
        G->vex[i] = i;
    }
    for(i = 0; i < G->numVertexes; ++i){
        for(j = 0; j < G->numVertexes; ++j){
            if(i == j)
                G->arc[i][j] = 0;
            else
                G->arc[i][j] = G->arc[j][i] = INF;
        }
    }

	for(int i = 0; i < crossnum; i++)
	{
		for(int j = 0; j < crossnum; j++)
		{
			G->arc[i][j] = *(map + i*crossnum + j);
		}
	}
	
    // 设置对称位置元素值
    for(i = 0; i < G->numVertexes; ++i){
        for(j = i; j < G->numVertexes; ++j){
            G->arc[j][i] = G->arc[i][j];
        }
    }
}

// Floyd algorithm
void ShortPath_Floyd(MGraph G, Patharc P, ShortPathTable D){
    int i, j, k;
    // 二重循环，初始化P, D
    for(i = 0; i < G.numVertexes; ++i){
        for(j = 0; j < G.numVertexes; ++j){
            D[i][j] = G.arc[i][j];
            P[i][j] = j;
        }
    }
    // 三重循环, Floyd algorithm
    for(k = 0; k < G.numVertexes; ++k){
        for(i = 0; i < G.numVertexes; ++i){
            for(j = 0; j < G.numVertexes; ++j){
                if(D[i][j] > D[i][k]+D[k][j]){
                    D[i][j] = D[i][k]+D[k][j];
                    P[i][j] = P[i][k];
                }
            }
        }
    }
}

// 打印最短路径
void PrintShortPath(MGraph G, Patharc P, ShortPathTable D, Matrix_i2v2 CrossOrder, int crossnum)
{
    int i, j, k;
    for(i = 0; i < G.numVertexes; ++i)
    {
        for(j = i+1; j < G.numVertexes; ++j)
        {
            //cout<<"v"<<i<<"--"<<"v"<<j<<" "<<"weight: "<<D[i][j]<<"  Path: "<<i<<" -> ";
            k = P[i][j];
            while(k != j){
                //cout<<k<<" -> ";
                k = P[k][j];
                *(CrossOrder + i*crossnum + j) = P[k][j];
                break;
            }
            //cout<<j<<endl;
        }
        //cout<<endl;
    }
}

int Floyd(Matrix_i2v2 CrossOrder, int crossnum, Matrix_f2v2 map) 
{
    MGraph G;
    Patharc P;
    ShortPathTable D;
    CreateMGraph(&G, map, crossnum);
    ShortPath_Floyd(G, P, D);
    PrintShortPath(G, P, D, CrossOrder, crossnum);
    return 0;
}



#endif
