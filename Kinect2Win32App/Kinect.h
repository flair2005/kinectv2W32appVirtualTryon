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

	Rect					contourRect;
	Mat						m_body;
	Mat						m_Depth;
	Mat						m_Color;
	Mat						m_BodyIndex;
	Mat						showImage;
	map<int, Point>          mapJoints;
	int						neck_x;
	int						neck_y;
	Point                   shoulderLeft;
	Point                   shoulderRight;
	Point                   footRight;
	Point                   footLeft;

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
	//Model*					mdl;

};

