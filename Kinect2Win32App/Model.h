#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include "include_opencv/opencv2/opencv.hpp"
#include "featurePoint.h"
using namespace std;
using namespace cv;

class Model{
public:
	Model();
	Model(const Model& m);
	~Model();
	void modelInitial();
	featurePoint* fpt;
	Rect contourRect;
	Mat m_body;
	Model& operator = (const Model&);
private:
	Mat m_model;
};

#endif