#include "stdafx.h"
#include "Kinect.h"
#include "Head.h"
#include <math.h>
#include <map>
#include <limits>
#include <strsafe.h>

static const DWORD c_AppRunTime = 5 * 60;//程序运行时间(s)，设置5*60表示运行5分钟后程序自动关闭
static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;
CKinect::CKinect()
{
	cout << "gouzaokinect" << endl;
	m_pKinectSensor = NULL;
	m_pCoordinateMapper = NULL;
	m_pMultiSourceFrameReader = NULL;
	m_pDepthCoordinates = NULL;
	m_pOutputRGBX = NULL;
	m_pBackgroundRGBX = NULL;
	m_pColorRGBX = NULL;

	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format

	//初始化OpenCV数组
	m_Depth.create(cDepthHeight, cDepthWidth, CV_16UC1);
	m_Color.create(cColorHeight, cColorWidth, CV_8UC4);
	m_BodyIndex.create(cDepthHeight, cDepthWidth, CV_8UC1);

	fpt = new featurePoint();
	gmt = new Garment();
	//mdl = new Model();
}
CKinect::CKinect(const CKinect& kin){
	/*m_pKinectSensor = NULL;
	m_pCoordinateMapper = NULL;
	m_pMultiSourceFrameReader = NULL;
	m_pDepthCoordinates = NULL;
	m_pOutputRGBX = NULL;
	m_pBackgroundRGBX = NULL;
	m_pColorRGBX = NULL;

	//m_pKinectSensor = new IKinectSensor;
	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format
	*/
	/*if (m_pOutputRGBX != NULL) delete m_pOutputRGBX;
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pOutputRGBX = *kin.m_pOutputRGBX;

	if (m_pBackgroundRGBX != NULL) delete m_pBackgroundRGBX;
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pBackgroundRGBX = *kin.m_pBackgroundRGBX;

	if (m_pColorRGBX != NULL) delete m_pColorRGBX;
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	*m_pColorRGBX = *kin.m_pColorRGBX;

	if (m_pDepthCoordinates != NULL) delete m_pDepthCoordinates;
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];
	*m_pDepthCoordinates = *kin.m_pDepthCoordinates;

	if (m_pDepthRGBX != NULL) delete m_pDepthRGBX;
	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];
	*m_pDepthRGBX = *kin.m_pDepthRGBX;*/
	SAFE_RELEASE_VEC(m_pOutputRGBX);
	SAFE_RELEASE_VEC(m_pBackgroundRGBX);
	SAFE_RELEASE_VEC(m_pColorRGBX);
	SAFE_RELEASE_VEC(m_pDepthCoordinates);
	SAFE_RELEASE_VEC(m_pDepthRGBX);
	if (!m_Depth.empty()) m_Depth.release();
	m_Depth = kin.m_Depth.clone();
	if (!m_Color.empty()) m_Color.release();
	m_Color = kin.m_Color.clone();
	if (!m_BodyIndex.empty()) m_BodyIndex.release();
	m_BodyIndex = kin.m_BodyIndex.clone();
	if (!m_body.empty()) m_body.release();
	m_body = kin.m_body.clone();

	if (fpt != NULL) SAFE_RELEASE( fpt);
	fpt = new featurePoint;
	*fpt = *kin.fpt;
	if (gmt != NULL) delete gmt;
	gmt = new Garment;
	*gmt = *kin.gmt;
	//if (mdl != NULL) delete mdl;
	//mdl = new Model;
	//*mdl = *kin.mdl;

	/*m_Depth.create(cDepthHeight, cDepthWidth, CV_16UC1);
	m_Color.create(cColorHeight, cColorWidth, CV_8UC4);
	m_BodyIndex.create(cDepthHeight, cDepthWidth, CV_8UC1);
	m_body = kin.m_body.clone();
	contourRect = kin.contourRect;
	m_Depth=kin.m_Depth.clone();
	m_Color; kin.m_Color.clone();
	m_BodyIndex=kin.m_BodyIndex.clone();
	fpt=new featurePoint;
	*fpt = *kin.fpt;
	gmt=new Garment;
	*gmt = *kin.gmt;
	mdl=new Model;
	*mdl = *kin.mdl;*/

}

CKinect& CKinect::operator = (const CKinect& kin){

	if (fpt != NULL) delete fpt;
	fpt = new featurePoint;
	*fpt = *kin.fpt;
	if (gmt != NULL) delete gmt;
	gmt = new Garment;
	*gmt = *kin.gmt;
	//if (mdl != NULL) delete mdl;
	//mdl = new Model;
	//*mdl = *kin.mdl;

	m_body = kin.m_body;
	contourRect = kin.contourRect;
	m_Depth = kin.m_Depth;
	m_Color; kin.m_Color;
	m_BodyIndex = kin.m_BodyIndex;
	/*
	//m_pKinectSensor = new IKinectSensor;
	// create heap storage for composite image pixel data in RGBX format
	m_pOutputRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for background image pixel data in RGBX format
	m_pBackgroundRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for color pixel data in RGBX format
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];

	// create heap storage for the coorinate mapping from color to depth
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];

	m_pDepthRGBX = new RGBQUAD[cDepthWidth * cDepthHeight];// create heap storage for color pixel data in RGBX format

	*m_pOutputRGBX = *kin.m_pOutputRGBX;
	*m_pBackgroundRGBX = *kin.m_pBackgroundRGBX;
	*m_pColorRGBX = *kin.m_pColorRGBX;
	*m_pDepthCoordinates = *kin.m_pDepthCoordinates;
	*m_pDepthRGBX = *kin.m_pDepthRGBX;
	*/
	return *this;
}

CKinect::~CKinect()
{
	SAFE_RELEASE_VEC(m_pOutputRGBX);
	SAFE_RELEASE_VEC(m_pBackgroundRGBX);
	SAFE_RELEASE_VEC(m_pColorRGBX);
	SAFE_RELEASE_VEC(m_pDepthCoordinates);
	SAFE_RELEASE_VEC(m_pDepthRGBX);

	// done with frame reader
	SafeRelease(m_pMultiSourceFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
	delete fpt;
	delete gmt;
	//delete mdl;
}


HRESULT	CKinect::InitKinect(Model mod)
{
	//mdl = &mod;
	//if (!mdl) return E_FAIL;
	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the frame reader
		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color | FrameSourceTypes::FrameSourceTypes_Body | FrameSourceTypes::FrameSourceTypes_BodyIndex,
				&m_pMultiSourceFrameReader);

//			hr = m_pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Color ,	&m_pMultiSourceFrameReader);
		}
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}

void CKinect::Update()
{
	//cout << "kinect update" << endl;
	if (!m_pMultiSourceFrameReader)
	{
		return;
	}

	IMultiSourceFrame* pMultiSourceFrame = NULL;
	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;
	IBodyIndexFrame* pBodyIndexFrame = NULL;
	IBodyFrame *pBodyFrame = NULL;

	HRESULT hr = m_pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

	if (SUCCEEDED(hr))//深度信息
	{
		IDepthFrameReference* pDepthFrameReference = NULL;
		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
		}
		SafeRelease(pDepthFrameReference);
	}

	if (SUCCEEDED(hr))//彩色信息
	{
		IColorFrameReference* pColorFrameReference = NULL;
		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
		}
		SafeRelease(pColorFrameReference);
	}

	if (SUCCEEDED(hr))//骨骼信息
	{
		IBodyFrameReference* pBodyFrameReference = NULL;
		hr = pMultiSourceFrame->get_BodyFrameReference(&pBodyFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);
		}
		SafeRelease(pBodyFrameReference);
	}

	if (SUCCEEDED(hr))//人体掩膜部分
	{
		IBodyIndexFrameReference* pBodyIndexFrameReference = NULL;
		hr = pMultiSourceFrame->get_BodyIndexFrameReference(&pBodyIndexFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameReference->AcquireFrame(&pBodyIndexFrame);
		}
		SafeRelease(pBodyIndexFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		INT64 nDepthTime = 0;
		IFrameDescription* pDepthFrameDescription = NULL;
		int nDepthWidth = 0;
		int nDepthHeight = 0;
		UINT nDepthBufferSize = 0;
		UINT16 *pDepthBuffer = NULL;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxDistance = 0;

		IFrameDescription* pColorFrameDescription = NULL;
		int nColorWidth = 0;
		int nColorHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nColorBufferSize = 0;
		RGBQUAD *pColorBuffer = NULL;

		IBody* ppBodies[BODY_COUNT] = { 0 };

		IFrameDescription* pBodyIndexFrameDescription = NULL;
		int nBodyIndexWidth = 0;
		int nBodyIndexHeight = 0;
		UINT nBodyIndexBufferSize = 0;
		BYTE *pBodyIndexBuffer = NULL;

		// get depth frame data
		hr = pDepthFrame->get_RelativeTime(&nDepthTime);

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Width(&nDepthWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Height(&nDepthHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}

		if (SUCCEEDED(hr))
		{
			nDepthMaxDistance = USHRT_MAX;
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
		}

		m_Depth = Mat(nDepthHeight, nDepthWidth, CV_16UC1, pDepthBuffer).clone();///////////////

		// get color frame data
		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Width(&nColorWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Height(&nColorHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
			}
			else if (m_pColorRGBX)
			{
				pColorBuffer = m_pColorRGBX;
				nColorBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		m_Color = Mat(nColorHeight, nColorWidth, CV_8UC4, pColorBuffer);///////////////

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		// get body index frame data
		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->get_FrameDescription(&pBodyIndexFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Width(&nBodyIndexWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrameDescription->get_Height(&nBodyIndexHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyIndexFrame->AccessUnderlyingBuffer(&nBodyIndexBufferSize, &pBodyIndexBuffer);
		}
		m_BodyIndex = Mat(nBodyIndexHeight, nBodyIndexWidth, CV_8UC1, pBodyIndexBuffer);

		if (SUCCEEDED(hr))
		{
			ProcessFrame(nDepthTime, pDepthBuffer, nDepthWidth, nDepthHeight, nDepthMinReliableDistance, nDepthMaxDistance,
						 pColorBuffer, nColorWidth, nColorHeight,
						 BODY_COUNT, ppBodies,
						 pBodyIndexBuffer, nBodyIndexWidth, nBodyIndexHeight);
		}

		SafeRelease(pDepthFrameDescription);
		SafeRelease(pColorFrameDescription);
		SafeRelease(pBodyIndexFrameDescription);

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pDepthFrame);
	SafeRelease(pColorFrame);
	SafeRelease(pBodyFrame);
	SafeRelease(pBodyIndexFrame);
	SafeRelease(pMultiSourceFrame);
}

void CKinect::ProcessFrame(INT64 nTime,
						   const UINT16* pDepthBuffer, int nDepthWidth, int nDepthHeight, USHORT nMinDepth, USHORT nMaxDepth,
						   const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
						   int nBodyCount, IBody** ppBodies,
						   const BYTE* pBodyIndexBuffer, int nBodyIndexWidth, int nBodyIndexHeight)
{
	LARGE_INTEGER qpcNow = { 0 };
	WCHAR szStatusMessage[64];
	//map<DepthSpacePoint, int> mapDepthPoint;
	//map<DepthSpacePoint, int>::iterator mapit;
	// Make sure we've received valid data
	if (m_pCoordinateMapper && m_pDepthCoordinates && m_pOutputRGBX &&
		pDepthBuffer && (nDepthWidth == cDepthWidth) && (nDepthHeight == cDepthHeight) &&
		pColorBuffer && (nColorWidth == cColorWidth) && (nColorHeight == cColorHeight) &&
		pBodyIndexBuffer && (nBodyIndexWidth == cDepthWidth) && (nBodyIndexHeight == cDepthHeight) &&
		m_pDepthRGBX)
	{
		HRESULT hr = m_pCoordinateMapper->MapColorFrameToDepthSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nColorWidth * nColorHeight, m_pDepthCoordinates);
		if (FAILED(hr))
		{
			return;
		}
		
		// loop over output pixels
		for (int colorIndex = 0; colorIndex < (nColorWidth*nColorHeight); ++colorIndex)
		{
			// default setting source to copy from the background pixel
			const RGBQUAD* pSrc = m_pBackgroundRGBX + colorIndex;

			DepthSpacePoint p = m_pDepthCoordinates[colorIndex];

			// Values that are negative infinity means it is an invalid color to depth mapping so we
			// skip processing for this pixel
			if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
			{
				int depthX = static_cast<int>(p.X + 0.5f);
				int depthY = static_cast<int>(p.Y + 0.5f);

				if ((depthX >= 0 && depthX < nDepthWidth) && (depthY >= 0 && depthY < nDepthHeight))
				{
					BYTE player = pBodyIndexBuffer[depthX + (depthY * cDepthWidth)];
					// if we're tracking a player for the current pixel, draw from the color camera
					if (player != 0xff)
					{
						// set source for copy to the color pixel
						pSrc = m_pColorRGBX + colorIndex;
					}
				}
			}
			// write output
			m_pOutputRGBX[colorIndex] = *pSrc;
		}
	}//确保参数都准确

	

	//
	//imshow("color2", m_Color);

	//resize(m_Color, showImage, Size(cColorWidth / 2, cColorHeight / 2));
	resize(m_Color, showImage, Size(cColorWidth, cColorHeight));
	//imshow("Color", showImage);//
	//imshow("Depth", m_Depth);
	imshow("BodyIndex", m_BodyIndex);
	//Mat Icanny;
	Mat bodyIndex2 = m_BodyIndex.clone();
	Mat thpng;//二值图
	Mat m_canny;
	threshold(bodyIndex2, thpng, 170, 255, CV_THRESH_BINARY);
	//imshow("erzhitu", thpng);//二值图显示

	//提取轮廓
	//vector<vector<Point>>bodyContours;
	//findContours(thpng, bodyContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//drawContours(result, bodyContours, -1, Scalar(255, 0, 0), 5);
	Canny(thpng, m_canny, 150, 220);
	GaussianBlur(m_canny, m_canny, Size(9, 9), 0, 0);
	//imshow("lunkuo", m_canny);//canny图显示
	Mat m_fillHole;
	fillHole(m_canny, m_fillHole);
	//imshow("fillHole", m_fillHole);//空洞填充图
	Mat m_contours(m_canny.size(), CV_8U, Scalar(255));
	vector<vector<cv::Point>>bodyContours;
	findContours(m_fillHole, bodyContours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	getSizeContours(bodyContours);
	vector<Point>contours = getBodyContoursPoint(bodyContours);
	drawContours(m_contours, bodyContours, -1, CV_RGB(0,0,0), 2);
	cout << bodyContours.size() <<": ";
	//cout << showImage.size() << endl;

	for (int i = 0; i < bodyContours.size(); i++){
		cout << bodyContours[i].size()<<" -- ";
	}
	cout << endl;
	//imshow("drawcontours", m_contours);//轮廓图显示
	//for (int i = 0; i < nColorHeight*nColorWidth; i++) cout << m_pDepthCoordinates[i].X << "&&" << m_pDepthCoordinates[i].Y<<"  ";
	//cout << endl;
	//寻找彩色图中人体的点
	/*for (int j = 0; j < contours.size(); ++j){
		DepthSpacePoint tPoint;
		tPoint.X = contours[j].x;
		tPoint.Y = contours[j].y;
		cout << tPoint.X << "%%" << tPoint.Y << endl;
		if (tPoint.X >= 0 && tPoint.X < nDepthWidth && tPoint.Y >= 0 && tPoint.Y < nDepthHeight)  //判断是否合法
		{
			int index = (int)tPoint.Y * nDepthWidth + (int)tPoint.X; //取得彩色图上那点对应在BodyIndex里的值(注意要强转)
			if (pBodyIndexBuffer[index] != 0xff)                   //如果判断出彩色图上某点是人体，就用它来替换背景图上对应的点
			{
				ColorSpacePoint colorPoint;
				HRESULT hr = m_pCoordinateMapper->MapDepthPointToColorSpace(tPoint, (UINT16)100, &colorPoint);
				if (SUCCEEDED(hr)){
					int colorX = static_cast<int>(colorPoint.X + 0.5f);
					int colorY = static_cast<int>(colorPoint.Y + 0.5f);
					cout << "colorclor" << colorX << ":" << colorY << endl;
					circle(showImage, Point(colorX, colorY), 8, CV_RGB(255, 0, 0), -1, 8, 0);
				}
			}
		}
	}*/
	//寻找彩色图中人体的点
	//寻找彩色图中人体的点
	cout << nColorWidth << "||" << nColorHeight << endl;
	Mat m_bg(nColorHeight, nColorWidth, CV_8UC1,Scalar(0));
	vector<Point>::iterator it;
	for (int i = 0; i < nColorHeight; ++i){
		for (int j = 0; j < nColorWidth; ++j){
			DepthSpacePoint tPoint = m_pDepthCoordinates[i * nColorWidth + j];
			//Point temp(tPoint.X, tPoint.Y);
			//it = find(contours.begin(),contours.end(),temp);
			//if (it!=contours.end())
			if (tPoint.X >= 0 && tPoint.X < nDepthWidth && tPoint.Y >= 0 && tPoint.Y < nDepthHeight)  //判断是否合法
			{
				int index = (int)tPoint.Y * nDepthWidth + (int)tPoint.X; //取得彩色图上那点对应在BodyIndex里的值(注意要强转)
				if (pBodyIndexBuffer[index] != 0xff)                   //如果判断出彩色图上某点是人体，就用它来替换背景图上对应的点
				{
					//cout << " bodyindex: " << pBodyIndexBuffer[index] << "\t";
					//circle(showImage, Point(j, i), 5, CV_RGB(255, 255, 0), -1, 8, 0);
					m_bg.at<uchar>(i, j) = 255;
					//circle(m_bg, Point(j, i), 1, CV_RGB(255, 255, 255), -1, 8, 0);
					//m_bg.at<Vec3b>(i, j)[0] = 255;
				}
			}
		}
	}
	cout << m_bg.size() << endl;
	imshow("bg",m_bg);
	/*Mat userThpng;
	threshold(m_bg, userThpng, 200, 255, CV_THRESH_BINARY);
	Mat m_userCanny;
	Canny(thpng, m_userCanny, 150, 220);
	imshow("userCanny",m_userCanny);
	Mat m_userFillHole;
	fillHole(m_userCanny, m_userFillHole);
	imshow("userFillHole", m_userFillHole);*/
	vector<vector<Point>>userContoursPoints;
	userContoursPoints.clear();
	findContours(m_bg, userContoursPoints, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	Mat m_userContours(m_bg.size(), CV_8U,Scalar(255));
	drawContours(m_userContours, userContoursPoints, -1, CV_RGB(0, 0, 0), 2); 
	imshow("userContours",m_userContours);
	vector<Point>userContour;
	userContour = getBodyContoursPoint(userContoursPoints);
	for (int i = 0; i < userContour.size(); i++){
		circle(showImage, Point(userContour[i].x, userContour[i].y), 5, CV_RGB(255, 255, 0), -1, 8, 0);
	}
	

	fpt->featurepointInit();
	IplImage *IPLshowImage;
	IPLshowImage = &IplImage(showImage);
	fpt->getCircle200(IPLshowImage, userContour);
	cout << "contoursPoint2.size()="<<fpt->contoursPoint2.size() << endl;
	vector<Point>tempp = fpt->contoursPoint2;
	cout << "tempp.size()=" << tempp.size() << endl;
	fpt->getSpecialPoint27(IPLshowImage, tempp);
	if (userContour.size()){
		contourRect = boundingRect(userContour);
		rectangle(showImage,contourRect,Scalar(255,128,64));
		cout << "contourRect.size=" << contourRect.size() << endl;
		m_body = showImage(contourRect);
		Mat m_body2;
		//resize(m_body, m_body2, cv::Size(), 0.5, 0.5);
		resize(m_body, m_body2, cv::Size(), 1, 1);
		imshow("bodybody", m_body2);
		/*IplImage *bodyImage, *cpBodyImage;
		bodyImage = &IplImage(showImage);
		cpBodyImage = cvCreateImage(contourRect.size(), 8, 3);
		cvSetImageROI(bodyImage, contourRect);
		cvCopy(bodyImage, cpBodyImage);
		Mat m_body2(cpBodyImage);
		m_body = m_body2.clone();
		imshow("bodybody", m_body);
		cvResetImageROI(bodyImage);
		free(bodyImage);
		bodyImage = NULL;
		free(cpBodyImage);
		cpBodyImage = NULL;*/
	}
	//fpt->getBodyRegion(m_bg, IPLshowImage);
	//Mat m_bg(cColorHeight, cColorWidth, CV_8UC4,Scalar(0,0,0));
	//寻找彩色图中人体的点

	//ColorSpacePoint *colorPoint=NULL;
	//HRESULT hr = m_pCoordinateMapper->MapDepthFrameToColorSpace(nColorWidth*nColorHeight, (UINT16*)pDepthBuffer,nDepthWidth * nDepthHeight , colorPoint);
	//cout << "ddd" << SUCCEEDED(hr) << endl;
	//ColorSpacePoint *colorSpacePoint = NULL;
	//HRESULT hr = m_pCoordinateMapper->MapDepthPointsToColorSpace(nDepthWidth*nDepthHeight, m_pDepthCoordinates, cDepthWidth*cDepthHeight, (UINT16*)pDepthBuffer, nColorWidth*nColorHeight, colorSpacePoint);
	//if (SUCCEEDED(hr)){
	/*for (int i = 0; i < bodyContours.size(); i++){
		for (int j = 0; j < bodyContours[i].size(); j++){
			//circle(showImage,Point(i,j),5,CV_RGB(0,255,0),-1,8,0);
			circle(showImage, Point(bodyContours[i][j].x* showImage.cols*1.0 / cDepthWidth, bodyContours[i][j].y * showImage.rows*1.0 / cDepthHeight), 5, CV_RGB(0, 255, 0), -1, 8, 0);
			//cout << "contoursPoint"<<bodyContours[i][j];
			//ColorSpacePoint *colorSpacePoint = NULL;
			//DepthSpacePoint depthSpacePoint = { bodyContours[i][j].x, bodyContours[i][j].y };
			//cout << "深度图的点：" << bodyContours[i][j].x<< bodyContours[i][j].y<<endl;
			//UINT16 depth = pDepthBuffer[bodyContours[i][j].y * cDepthWidth + bodyContours[i][j].x];*/
			/*HRESULT hr = m_pCoordinateMapper->MapDepthPointsToColorSpace(nColorWidth*nColorHeight, m_pDepthCoordinates, cDepthWidth*cDepthHeight, (UINT16*)pDepthBuffer, nDepthWidth*nDepthHeight, colorSpacePoint);
			if (SUCCEEDED(hr)){
				int pos = bodyContours[i][j].y * nColorWidth + bodyContours[i][j].x;
				int x = colorSpacePoint[pos].X;
				int y = colorSpacePoint[pos].Y;
				//circle(showImage, Point(x, y), 3, CV_RGB(0, 255, 0), -1, 8, 0);
				cout << "lunkuodian:" << x << y << endl;
				circle(showImage, Point(x, y), 3, CV_RGB(0, 255, 0), -1, 8, 0);
			}
			int pos = bodyContours[i][j].y * cColorWidth + bodyContours[i][j].x;//像素点在一维序列中的位置。
			int x = colorPoint[pos].X;
			int y = colorPoint[pos].Y;
			circle(showImage, Point(x,y), 3, CV_RGB(0, 255, 0), -1, 8, 0);*/
			/*DepthSpacePoint depthSpacePoint;
			depthSpacePoint.X = bodyContours[i][j].x;
			depthSpacePoint.Y = bodyContours[i][j].y;
			ColorSpacePoint colorSpacePoint;
			UINT16 depth = findDepthBuffer(depthSpacePoint, nDepthWidth*nDepthHeight, pDepthBuffer);
			cout << "depth=" << depth << endl;
			HRESULT hr = m_pCoordinateMapper->MapDepthPointToColorSpace(depthSpacePoint, depth, &colorSpacePoint);
			if (SUCCEEDED(hr)){
				Point temp;
				temp.x = static_cast<int>(colorSpacePoint.X);
				temp.y = static_cast<int>(colorSpacePoint.X);
				circle(showImage, temp, 5, CV_RGB(255, 0, 0), -1, 8, 0);
			}
		}
	}*/
	//}
	//骨骼信息
	for (int count = 0; count < BODY_COUNT; count++){
		BOOLEAN bTracked = false;
		HRESULT hr = ppBodies[count]->get_IsTracked(&bTracked);
		if (SUCCEEDED(hr) && bTracked){
			Joint joint[JointType_Count];
			hr = ppBodies[count]->GetJoints(JointType_Count, joint);
			if (SUCCEEDED(hr)){
				HandState leftHandState = HandState_Unknown;
				hr = ppBodies[count]->get_HandLeftState(&leftHandState);
				if (SUCCEEDED(hr)){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[JointType_HandLeft].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							if (leftHandState == HandState::HandState_Open){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 128, 0), 5, CV_AA);
							}
							else if (leftHandState == HandState::HandState_Closed){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 0, 128), 5, CV_AA);
							}
							else if (leftHandState == HandState::HandState_Lasso){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(128, 128, 0), 5, CV_AA);
							}
						}
					}
				}
				HandState rightHandState = HandState_Unknown;
				hr = ppBodies[count]->get_HandRightState(&rightHandState);
				if (SUCCEEDED(hr)){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[JointType_HandRight].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast<int>(colorSpacePoint.X);
						int y = static_cast<int>(colorSpacePoint.Y);;
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							if (rightHandState == HandState::HandState_Open){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 128, 0), 5, CV_AA);
							}
							else if (rightHandState == HandState::HandState_Closed){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(0, 0, 128), 5, CV_AA);
							}
							else if (rightHandState == HandState::HandState_Lasso){
								cv::circle(showImage, cv::Point(x, y), 75, cv::Scalar(128, 128, 0), 5, CV_AA);
							}
						}
					}
				}
				for (int type = 0; type < JointType_Count; type++){
					ColorSpacePoint colorSpacePoint = { 0 };
					hr = m_pCoordinateMapper->MapCameraPointToColorSpace(joint[type].Position, &colorSpacePoint);
					if (SUCCEEDED(hr)){
						int x = static_cast< int >(colorSpacePoint.X);
						int y = static_cast< int >(colorSpacePoint.Y);
						if ((x >= 0) && (x < cColorWidth) && (y >= 0) && (y < cColorHeight)){
							cv::circle(showImage,cv::Point(x, y), 5, Scalar(255,255,0), - 1, CV_AA);
						}
					}
				}
			}
		}
		//resize(m_Color, showImage, cv::Size(),0.5,0.5);
	}
	
	cout << endl;
	//imshow("Color", showImage);

	Mat showImageResize;
	resize(showImage, showImageResize, cv::Size(), 0.5, 0.5);
	//imshow("Color", showImageResize);
	RGBQUAD* pRGBX = m_pDepthRGBX;
	// end pixel is start + width*height - 1
	const UINT16* pBufferEnd = pDepthBuffer + (nDepthWidth * nDepthHeight);
	while (pDepthBuffer < pBufferEnd)
	{
		USHORT depth = *pDepthBuffer;
		BYTE intensity = static_cast<BYTE>((depth >= nMinDepth) && (depth <= nMaxDepth) ? (depth % 256) : 0);
		pRGBX->rgbRed = intensity;
		pRGBX->rgbGreen = intensity;
		pRGBX->rgbBlue = intensity;
		++pRGBX;
		++pDepthBuffer;
	}

	// Draw the data nDepthHeight OpenCV
	Mat DepthImage(nDepthHeight, nDepthWidth, CV_8UC4, m_pDepthRGBX);
	Mat show = DepthImage.clone();
	imshow("DepthImage", show);
	/*
	Mat ColorImage2(cColorWidth, cColorHeight, CV_8UC4, m_pOutputRGBX);
	Mat show2 = ColorImage2.clone();
	imshow("colorbodyImage",show2);*/

	waitKey(1);
}

void CKinect::fillHole(const Mat srcBw, Mat &dstBw)//孔洞填充
{
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//延展图像
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//裁剪延展的图像
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);

}

void CKinect::getSizeContours(vector<vector<Point>> &contours)//去除小轮廓和极大轮廓   <500  >5000
{
	int cmin = 500;   // 最小轮廓长度  
	int cmax = 5000;   // 最大轮廓长度  
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) < cmin || (itc->size()) > cmax)
		{
			itc = contours.erase(itc);
		}
		else ++itc;
	}
}

UINT16 CKinect::findDepthBuffer(DepthSpacePoint DSP, int DSP_SIZE, const UINT16 *pDepthBuffer){
	for (int index = 0; index < DSP_SIZE; ++index)
	{
		DepthSpacePoint p = m_pDepthCoordinates[index];
		if (DSP.X == p.X && DSP.Y == p.Y){
			return pDepthBuffer[index];
		}
	}
	return UINT16(0);
}

vector<Point>  CKinect::getBodyContoursPoint(vector<vector<Point>> &contours)
{
	int cmax = 0;   // 最大轮廓长度  
	vector<Point>temp;
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if ((itc->size()) > cmax)
		{
			cmax = itc->size();
			temp.clear();
			temp = *itc;
		}
		itc++;
	}
	vector<Point>contoursPoint = temp;
	//cout << "contoursPoint1_size()=" << contoursPoint1.size() << endl;
	return contoursPoint;
}

void CKinect::deformation(){
	//cout << mdl->fpt->featurePt.size() << "|model|" << mdl->fpt->auxiliaryPoints.size() << endl;
	cout << fpt->featurePt.size() << "|kinect|"<<fpt->auxiliaryPoints.size()<<endl;
	//imshow("defor_color", m_Color);
}