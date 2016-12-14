#ifndef TRANSFER_H
#define TRANSFER_H

#include "Kinect.h"
#include "featurePoint.h"
#include "Garment.h"
#include "Model.h"
class Transfer
{
public:
	Transfer();
	~Transfer();
	void transferInit(Model mod,Garment gar,CKinect kin);
	void transComputing();
	Model mod;
	Garment gar;
	CKinect kin;
private:
	Mat m_user;
	Mat m_model;
	Rect rect_user;
	Rect rect_model;
};

#endif