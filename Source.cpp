#include<Windows.h>
#include<iostream>
#include<string>
#include <fstream>
#include <io.h>
#include<opencv2/opencv.hpp>
#include "putText.h"

using namespace std;
using namespace cv;

string getFileInfo(string filename)
{
	struct _stat stat_buffer;
	int result = _stat(filename.c_str(), &stat_buffer);

	struct tm tmStruct;
	char timeChar[26];

	localtime_s(&tmStruct, &stat_buffer.st_ctime);
	strftime(timeChar, sizeof(timeChar), "%Y %m %d", &tmStruct);
	return timeChar;
}

void writeLog(String filePath)
{
	string outPath = filePath + "out/";
	string pattern_jpg = filePath + "*.jpg";
	vector<String> imageFiles;
	glob(pattern_jpg, imageFiles);
	if(imageFiles.empty())
	{
		return;
	}
	for (int i = 0; i < imageFiles.size(); i++) {
		Mat src = imread(imageFiles[i]);
		int fontSize = src.cols / 50;
		
		string log = "09 03 24";
		log = getFileInfo(imageFiles[i]);
		string fn = "LED Digital 7";
		float rate = 0.95;
		putTextZH(src, log.c_str(), Point(src.cols * rate - fontSize * log.size() * 0.5, src.rows * rate), Scalar(255 - 251, 255-108 ,255-50), fontSize, fn.c_str());

		string imgName = outPath + imageFiles[i].substr(filePath.length());
		imwrite(imgName, src);
	}
}
int main()
{
	writeLog("D:/CPPWorkSpace/AddWatermark/");
}