#include "stdafx.h"
#include "Garment.h"
#include <vector>

Garment::Garment(){

}
Garment::~Garment(){

}
void Garment::garmentInitial(){
	IplImage *srcIplImage = cvLoadImage("garment\\allgarment.png");
	if (!srcIplImage->imageData)
	{
		cout << "Fail to load garment pic" << endl;
		return ;
	}
	Mat srcGarment(srcIplImage);
	m_garment = srcGarment.clone();
	imshow("srcGarment",m_garment);
}