#pragma once
#include <Kinect.h>// Kinect Header files
#include "include_opencv/opencv2/opencv.hpp"
#include "featurePoint.h"
#include "Garment.h"
#include "Model.h"
using namespace std;
using namespace cv;
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}
//定义Kinect方法类
class CKinect
{
public:
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	static const int        cColorWidth = 1920;
	static const int        cColorHeight = 1080;
	CKinect();
	CKinect(const CKinect& kin);
	CKinect& operator = (const CKinect &);
	~CKinect();
	HRESULT					InitKinect(Model mod);//初始化Kinect
	void					Update();//更新数据
	void                    ProcessFrame(INT64 nTime,
										 const UINT16* pDepthBuffer, int nDepthHeight, int nDepthWidth, USHORT nMinDepth, USHORT nMaxDepth,
										 const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
										 int nBodyCount, IBody** ppBodies,
										 const BYTE* pBodyIndexBuffer, int nBodyIndexWidth, int nBodyIndexHeight);
	void					fillHole(const Mat srcBw, Mat &dstBw);//孔洞填充
	void					getSizeContours(vector<vector<Point>> &contours);//去除小轮廓
	UINT16					findDepthBuffer(DepthSpacePoint DSP, int DSP_SIZE, const UINT16 *pDepthBuffer);
	vector<Point>			getBodyContoursPoint(vector<vector<Point>> &contours);
	
	void					deformation();


	void					transferInit(Model mod, Garment gar);
	void					transComputing(Mat& m_user);
	void					getModel(Model model);
	void					getGarment(Garment garment);
	void					getmModel();
	void					getrectModel();
	void					getmGarment();
	void					connectGarUser(Mat m_gar, Mat m_user);
	Point					getPointUser_x();
	Point					getPointModel_y();
	Point					getNeckPFromMat(Mat garment);
	Mat						combineMat(Mat x, Mat y, Mat mask);
	Mat						positionCorrect(Point user_x, Point model_y, Mat src);

	Rect					getGarmentRect(Mat garment);
	int						getUserWidth();
	int						getUserHeight();
	int						getGarmentWidth(Mat mask);
	int						getGarmentHeight(Mat mask);
	//void fillHole(const Mat srcBw, Mat &dstBw);
	void					combineGarmentAndBody(Mat& user, Mat& garment, Point bodyNeck, Point garmentNeck, Mat mask_garment);


	Rect					contourRect;
	Mat						m_body;
	Mat						m_Depth;
	Mat						m_Color;
	Mat						m_BodyIndex;
	Mat						showImage;
	//map<int, Point>          mapJoints;
	int						neck_x;
	int						neck_y;
	Point					neck;
	Point                   shoulderLeft;
	Point                   shoulderRight;
	Point                   footRight;
	Point                   footLeft;
	Point                   ankleLeft;
	Point                   ankleRight;
	int						ankleLeft_x;
	int						ankleLeft_y;
	int						ankleRight_x;
	int						ankleRight_y;
	int						shoulderLeft_x;
	int						shoulderLeft_y;
	int						shoulderRight_x;
	int						shoulderRight_y;


	Garment                 gar;
	Model                   mod;

private:
	IKinectSensor*          m_pKinectSensor;// Current Kinect
	// Frame reader
	IMultiSourceFrameReader*m_pMultiSourceFrameReader;

	ICoordinateMapper*      m_pCoordinateMapper;
	DepthSpacePoint*        m_pDepthCoordinates;
	RGBQUAD*                m_pOutputRGBX;
	RGBQUAD*                m_pColorRGBX;
	RGBQUAD*                m_pBackgroundRGBX;
	RGBQUAD*                m_pDepthRGBX;

	//IBody**					myBodyArr;//


	featurePoint*           fpt;
	Garment*                gmt;

	//Mat m_user;
	Mat m_model;
	Rect rect_user;
	Rect rect_model;
	Mat m_garment;
	//Model*					mdl;

};

