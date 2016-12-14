#ifndef GARMENT_H
#define GARMENT_H

#include <iostream>
#include <vector>
#include "include_opencv/opencv2/opencv.hpp"
using namespace std;
using namespace cv;

class Garment{
public:
	Garment();
	~Garment();
	Mat m_garment;
	Point featureVector;
	void garmentInitial();
private:

};

#endif