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
	void connectGarUser(Mat m_gar,Mat m_user);
	Point getPointUser_x();
	Point getPointModel_y();
	Point getNeckPFromMat(Mat garment);
	Mat  combineMat(Mat x, Mat y, Mat mask);
	Mat  positionCorrect(Point user_x,Point model_y,Mat garment);
	Model mod;
	Garment gar;
	CKinect kin;

	Rect getGarmentRect(Mat garment);
	int getUserWidth();
	int getUserHeight();
	int getGarmentWidth(Mat mask);
	int getGarmentHeight(Mat mask);
	void fillHole(const Mat srcBw, Mat &dstBw);

private:
	Mat m_user;
	Mat m_model;
	Rect rect_user;
	Rect rect_model;
	Mat m_garment;
};

#endif