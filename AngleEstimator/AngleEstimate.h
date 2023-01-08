#pragma once
#ifndef ANGLEESTIMATE_H
#define ANGLEESTIMATE_H
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
//#include<map>
#include<opencv2/objdetect.hpp>
#include<Windows.h>

class AngleEstimate{
private:
	std::vector<std::vector<int>> colors = {//{hmin,smin,vmin, hmax, smax, vmax}
	{ 0,172,87, 62,255,180}, //orange
	{ 72,87,38, 129,255,105 }//mint
	};
	std::vector<cv::Mat> mask;
	std::vector < std::vector < cv::Point>> contours;
	//std::vector < std::vector < cv::Point>> contours_poly;
	
	void clearData();
public:
	AngleEstimate() {
		//colors.push_back( { 0,172,87, 62,255,180}); //orange
		//colors.push_back({ 72,87,38, 129,255,105 }); // mint
		for (int i = 0; i < colors.size(); i++)mask.push_back(cv::Mat());
	}
	void detectObject();
	void runProgram();
	void drawBoundingBoxesAndShowAngle(cv::Mat& img);
	void setUpMasks(cv::Mat &img);
	void findCont();
	


};

#endif
