#pragma once
#if !defined(_DBSCAN_H_INCLUDED_)
#define _DBSCAN_H_INCLUDED_

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <stack>
#include <map>
enum
{
	pointType_UNDO,
	pointType_NOISE,
	pointType_BORDER,
	pointType_CORE

};

class point {
public:
	float x;
	int cluster;
	int pointType;  //1 noise 2 border 3 core
	int pts;        //points in MinPts 
	int corePointID;
	std::vector<int> corepts;
	int  visited;
	void init();
	point();
	point(float a)
	{
		x = a;
	};
	//point(float a, float b, int c) {
	//	x = a;
	//	y = b;
	//	cluster = c;
	//};
};

float squareDistance(point a, point b);
void DBSCAN(std::vector<point>& dataset, float Eps, int MinPts);
#endif 
