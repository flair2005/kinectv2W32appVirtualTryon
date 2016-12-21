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
	void getModel(Model model);
	void getGarment(Garment garment);
	void getKinect(CKinect &kinect);
	void getmUser();
	void getmModel();
	void getRectUser();
	void getrectModel();
	void getmGarment();
	Model mod;
	Garment gar;
	CKinect kin;
private:
	Mat m_user;
	Mat m_model;
	Rect rect_user;
	Rect rect_model;
	Mat m_garment;
};

#endif