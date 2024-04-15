#include "DBSCAN.h"

int clusterID = 0;

void point::init()
{
	cluster = 0;
	pointType = pointType_UNDO;//pointType_NOISE pointType_UNDO
	pts = 0;
	visited = 0;
	corePointID = -1;
}


float squareDistance(point a, point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x));
}

void DBSCAN(std::vector<point>& dataset, float Eps, int MinPts)
{
	int len = dataset.size();//数据长度
	for (int i = 0; i < len; i++)//参数初始化
	{
		dataset[i].init();
	}

	std::vector<std::vector <float>> distP2P(len);

	//calculate pts

	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {//i+1
			float distance = squareDistance(dataset[i], dataset[j]);
			distP2P[i].push_back(distance);//disp for debug
			if (distance <= Eps) {
				dataset[i].pts++;
			}
		}
	}
	//core point 

	std::vector<point> corePoint;
	for (int i = 0; i < len; i++) {
		int tempPts = dataset[i].pts;
		if (tempPts >= MinPts) {
			dataset[i].pointType = pointType_CORE;
			dataset[i].corePointID = i;
			corePoint.push_back(dataset[i]);
		}
	}

	//joint core point
	int numCorePoint = corePoint.size(); //core point number

	for (int i = 0; i < numCorePoint; i++) {
		for (int j = 0; j < numCorePoint; j++) {
			float distTemp = distP2P[corePoint[i].corePointID][corePoint[j].corePointID];//display for debug
			if (distTemp <= Eps) {//squareDistance(corePoint[i],corePoint[j])
				corePoint[i].corepts.push_back(j);//other point orderID link to core point
			}
		}
	}
	for (int i = 0; i < numCorePoint; i++) {
		std::stack<point*> ps;
		if (corePoint[i].visited == 1) continue;
		clusterID++;
		corePoint[i].cluster = clusterID; //create a new cluster
		ps.push(&corePoint[i]);
		point* v;
		while (!ps.empty()) {
			v = ps.top();
			v->visited = 1;
			ps.pop();
			for (int j = 0; j < v->corepts.size(); j++) {
				if (corePoint[v->corepts[j]].visited == 1) continue;
				corePoint[v->corepts[j]].cluster = corePoint[i].cluster;
				corePoint[v->corepts[j]].visited = 1;
				ps.push(&corePoint[v->corepts[j]]);
			}
		}

	}

	for (int i = 0; i < len; i++) {
		for (int j = 0; j < numCorePoint; j++) {
			float distTemp = distP2P[i][corePoint[j].corePointID];
			if (distTemp <= Eps) {
				dataset[i].pointType = pointType_BORDER;
				dataset[i].cluster = corePoint[j].cluster;
				break;
			}
		}
	}
}
