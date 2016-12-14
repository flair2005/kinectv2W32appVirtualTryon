//#ifndef DEFOR_H
//#define DEFOR_H

/*#include <iostream>
#include <vector>
#include "include_opencv/opencv2/opencv.hpp"*/
#include "featurePoint.h"
#include "Model.h"
#include "Garment.h"
#include "Kinect.h"
using namespace std;
using namespace cv;

class Deformation:public Model,public CKinect,public Garment{
public:

	Deformation();
	~Deformation();
	void deformationInit(Model mod,CKinect kin,Garment gar);
	void computing();
	Model mod;
	Garment gar;
	CKinect kin;
private:
	Mat m_user;
	Mat m_model;
	Rect rect_user;
	Rect rect_model;
};

//#endif
