#include "stdafx.h"
#include "Model.h"

Model::Model(){
	fpt = new featurePoint();
}

Model::~Model(){
	delete fpt;
}

void Model::modelInitial(){
	fpt->featurepointInit();
	IplImage *plmgsrc = cvLoadImage("cccc.png");

	if (!plmgsrc->imageData)
	{
		cout << "Fail to load image" << endl;
		return ;
	}
	IplImage *plmg8u = NULL;
	IplImage *plmgCanny = NULL;
	plmg8u = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);
	plmgCanny = cvCreateImage(cvGetSize(plmgsrc), IPL_DEPTH_8U, 1);

	cvCvtColor(plmgsrc, plmg8u, CV_BGR2GRAY);
	cvCanny(plmg8u, plmgCanny, 20, 200, 3);
	Mat cannyImage(plmgCanny);
	Mat resulttemp;
	fpt->fillHole(cannyImage, resulttemp);

	vector<vector<Point>> contours;
	//CV_CHAIN_APPROX_NONE  获取每个轮廓每个像素点  
	findContours(resulttemp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//getSizeContours(contours);
	//contours2=getMaxSizeContours(contours);
	cout << "contours_size=" << contours.size() << endl;
	fpt->getBodyContoursPoint(contours);
	//cout << contours[contours.size() - 1].size() << endl;
	Mat result(cannyImage.size(), CV_8U, Scalar(255));
	drawContours(result, contours, -1, Scalar(0), 2);   // -1 表示所有轮廓  
	//namedWindow("result", 1);
	//cvSetMouseCallback("result", on_mouse, (IplImage *) result);
	//imshow("result", result);/**/
	Mat src(plmgsrc);
	fpt->getCircle200(plmgsrc, fpt->contoursPoint1);
	cout << "dd"<<fpt->contoursPoint2.size()<<endl;
	fpt->getSpecialPoint27(plmgsrc, fpt->contoursPoint2);
	//fpt->getBodyRegion(resulttemp, plmgsrc);
	drawContours(src, contours, -1, Scalar(0, 0, 255, 0), 1);   // -1 表示所有轮廓
	contourRect = boundingRect(fpt->contoursPoint1);
	rectangle(src,contourRect,Scalar(255,128,64),3);
	imshow("src", src);/**/

	m_body = src(contourRect);
	imshow("body", m_body);

}

