#include"stdafx.h"
#include"Transfer.h"

Transfer::Transfer()
{
}

Transfer::~Transfer()
{
}
void Transfer::transferInit(Model mod, Garment gar, CKinect kin){
	this->mod = mod;
	this->gar = gar;
	this->kin = kin;
	//this->rect_model = mod.contourRect;
	this->rect_model = mod.modelSrcRect;
	this->rect_user = kin.contourRect;
	//this->m_model = mod.m_body;
	this->m_model = mod.modelSrc;
	this->m_user = kin.m_body;
	this->m_garment = gar.m_garment;
}
void Transfer::getModel(Model model){
	mod = model;
	//rect_model = model.contourRect;
	//m_model = model.m_body;
	//imshow("trans_m_body",m_model); 
}
void Transfer::getGarment(Garment garment){
	gar = garment;
}
void Transfer::getKinect(CKinect &kinect){
	kin = kinect;
	//rect_user = kinect.contourRect;
	//m_user = kinect.m_body;
}
void Transfer::getmUser(){
	m_user = kin.m_body;
	cout << "|kin_m_body_size|=>" << m_user.size() <<"|kin_joints_size|=>"<<kin.mapJoints.size()<< endl;
}
void Transfer::getmModel(){
	//m_model = mod.m_body;
	mod.modelInitial();
	m_model = mod.modelSrc;
	imshow("modsrc", mod.modelSrc);
	cout << "|model_m_body_size|=>" << m_model.size() << endl;
}

void Transfer::getrectModel(){
	//rect_model = mod.contourRect;
	rect_model = mod.modelSrcRect;
	cout << "|rect_model_size|=>" << rect_model.size() << endl;
}
void Transfer::getRectUser(){
	rect_user = kin.contourRect;
	cout << "|rect_user_size|=>" << rect_user.size() << endl;
}

void Transfer::getmGarment(){
	m_garment = gar.m_garment;
	cout << "|m_garment_size|=>" << m_garment.size() << endl;
}

int Transfer::getUserWidth(){
	if (kin.shoulderRight.x != 1 && kin.shoulderRight.x != 1)
		return abs(kin.shoulderRight.x - kin.shoulderLeft.x);
	else
		return kin.contourRect.width;
}

int Transfer::getUserHeight(){
	if (kin.footLeft.y != 1 && kin.footLeft.y != 0 && kin.neck_y != 0)
		return abs(kin.neck_y - kin.footLeft.y);
	else
		return kin.contourRect.height;
}
int Transfer::getGarmentHeight(Mat mask){
	return 0;
}
int Transfer::getGarmentWidth(Mat mask){
	int rownum = mask.rows;
	int colnum = mask.cols;
	int flag = false;
	int left = 0;
	int right = colnum;
	for (int j = 0; j < colnum; j++){
		if (!(mask.at<Vec3b>(rownum / 2, j)[0] && mask.at<Vec3b>(rownum / 2, j)[1] && mask.at<Vec3b>(rownum / 2, j)[2])){
			left = j;
			break;
		}
	}
	for (int j = colnum-1; j >=0; j--){
		if (!(mask.at<Vec3b>(rownum / 2, j)[0] && mask.at<Vec3b>(rownum / 2, j)[1] && mask.at<Vec3b>(rownum / 2, j)[2])){
			right = j;
			break;
		}
	}
	return right-left;
}
Mat Transfer::combineMat(Mat x, Mat y, Mat mask){
	Mat dst = x.clone();
	int rownum = x.rows<y.rows?x.rows:y.rows;
	int colnum = x.cols<y.cols?x.cols:y.cols;
	for (int i = 0; i < rownum; i++){
		for (int j = 0; j < colnum; j++){
			//cout << "mask" << mask.at<Vec3b>(i, j)[0] << mask.at<Vec3b>(i, j)[1] << mask.at<Vec3b>(i, j)[2] << endl;
			/*if (mask.at<Vec3b>(i, j)[0] == 255 && mask.at<Vec3b>(i, j)[1] == 255 && mask.at<Vec3b>(i, j)[2] == 255){
				dst.at<Vec3b>(i, j)[0] = x.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = x.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = x.at<Vec3b>(i, j)[2];
			}
			else{
				dst.at<Vec3b>(i, j)[0] = y.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = y.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = y.at<Vec3b>(i, j)[2];
			}*/
			if (!(mask.at<Vec3b>(i, j)[0] && mask.at<Vec3b>(i, j)[1] && mask.at<Vec3b>(i, j)[2])){
				dst.at<Vec3b>(i, j)[0] = y.at<Vec3b>(i, j)[0];
				dst.at<Vec3b>(i, j)[1] = y.at<Vec3b>(i, j)[1];
				dst.at<Vec3b>(i, j)[2] = y.at<Vec3b>(i, j)[2];
			}
		}
	}
	return dst;
}

Point Transfer::getPointUser_x(){
	Point user_x(0,0);
	if (!kin.mapJoints.empty()){
		cout << "jingdian 2:" << kin.mapJoints[JointType_Neck].x << "," << kin.mapJoints[JointType_Neck].y << endl;
		cout << "jingdian 3:" << kin.neck_x << "," << kin.neck_y << endl;
		//user_x.x = kin.mapJoints[JointType_Neck].x - kin.contourRect.x;
		//user_x.y = kin.mapJoints[JointType_Neck].y - kin.contourRect.y;
		user_x.x = kin.neck_x - kin.contourRect.x;
		user_x.y = kin.neck_y - kin.contourRect.y;
		circle(kin.m_body, user_x, 8, CV_RGB(255, 0, 0) ,- 1, 8, 0);
		//circle(kin.m_Color, kin.mapJoints[JointType_Neck], 8, CV_RGB(255, 0, 0), -1, 8, 0);
		//user_x.x = kin.mapJoints[JointType_Neck].x;
		//user_x.y = kin.mapJoints[JointType_Neck].y;
	}
	return user_x;
}
Point Transfer::getPointModel_y(){
	Point model_y(0, 0);
	if (!mod.fpt->featurePt.empty()){
		model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2;
			model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2;
		//model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2;
		//model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2;
		circle(mod.modelSrc, model_y, 8, CV_RGB(0, 255, 255), -1, 8, 0);
		imshow("modelSrc2", mod.modelSrc);
	}
	return model_y;
}

Point Transfer::getNeckPFromMat(Mat mask){
	int rownum = mask.rows;
	int colnum = mask.cols;
	Point first(0,0);
	Point second(0,0);
	bool flagfirst = false;
	bool flagsec = false;
	for (int i = 0; i < rownum; i++){
		for (int j = 0; j < colnum; j++){
			if (!(mask.at<Vec3b>(i, j)[0] && mask.at<Vec3b>(i, j)[1] && mask.at<Vec3b>(i, j)[2])){
				if (!flagfirst){
					flagfirst = true;
					first.x = j;
					first.y = i;
				}
				if (!flagsec && flagfirst){
					if (abs(first.x - j)>5 && abs(first.y - i) < 5){
						flagsec = true;
						second.x = j;
						second.y = i;
					}
				}
			}
			if (flagfirst && flagsec) return Point((first.x + second.x) / 2, (first.y + second.y) / 2);
		}
	}
	return Point(0,0);
}

Mat Transfer::positionCorrect(Point user_x, Point model_y, Mat src){
	Mat dst;
	const int rows = src.rows;
	const int cols = src.cols;
	int dx = user_x.x - model_y.x;
	int dy = user_x.y - model_y.y;
	int dstrows = src.rows + abs(dx);
	int dstcols = src.cols + abs(dy);
	dst.create(dstrows, dstcols, src.type());

	Vec3b *p;
	for (int i = 0; i < rows; i++)
	{
		p = dst.ptr<Vec3b>(i);
		for (int j = 0; j < cols; j++)
		{
			//ƽ�ƺ�����ӳ�䵽ԭͼ��
			int x = j - dx;
			int y = i - dy;

			//��֤ӳ����������ԭͼ��Χ��
			if (x >= 0 && y >= 0 && x < cols && y < rows)
				p[j] = src.ptr<Vec3b>(y)[x];
		}
	}
	return dst;
}
void Transfer::fillHole(const Mat srcBw, Mat &dstBw){
	Size m_Size = srcBw.size();
	Mat Temp = Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcBw.type());//��չͼ��
	srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

	cv::floodFill(Temp, Point(0, 0), Scalar(255));

	Mat cutImg;//�ü���չ��ͼ��
	Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

	dstBw = srcBw | (~cutImg);
}
Rect Transfer::getGarmentRect(Mat garment){
	Mat mgray;
	Mat mcanny;
	Mat mfillhole;
	cvtColor(garment, mgray, CV_BGR2GRAY);
	Canny(mgray, mcanny, 20, 200, 3);
	fillHole(mcanny, mfillhole);

	vector<vector<Point>> contours;
	//if (!contours.empty()) contours.clear();
	findContours(mfillhole, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	Rect res;
	res = boundingRect(contours[0]);
	return res;
	/*vector<vector<Point> > contours;
	threshold(garment, temp, 200, 255, CV_THRESH_BINARY);
	bitwise_xor(temp, Scalar(255,255,255), temp);
	imshow("hanshutemp",temp);
	findContours(temp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);*/
	
}
void Transfer::transComputing(){
	//cout << "|kin_m_body_size|=>" << kin.m_Color.size() << "  |mod_m_body_size|=>" << mod.m_body.size() << endl;
	//imshow("trans_m_body",mod.m_body);
	getmUser();
	getmModel();
	imshow("m_model",mod.modelSrc);
	getRectUser();
	getrectModel();
	getmGarment();
	Mat m_model2(m_model.size(), m_model.type());
	if (!m_user.empty()){
		resize(m_model, m_model2, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
	}
	cout << "|m_model2_size|=>" << m_model2.size() << endl;
	imshow("m_model2", m_model2);
	mod.getFpt();
	//Mat m_garment2(m_garment.size(),m_garment.type(),Scalar(255));
	Mat m_garment2;
	m_garment2 = m_garment(rect_model);
	cout << "|m_garment2_size|=>" << m_garment2.size() << endl;
	Mat m_garment3(m_garment.size(), m_garment.type(), Scalar(255));
	if (!m_garment2.empty() && !m_user.empty()) {
		//resize(m_garment2, m_garment3, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
		m_garment3 = m_garment2.clone();
		//imshow("m_garment3", m_garment3);
		int userWidth = getUserHeight();
		int userHeight = getUserWidth();
		cout << "width height" << userWidth << "," << userHeight << endl;
		Rect garmentRect = getGarmentRect(m_garment3);
		int garmentHight = garmentRect.height;
		/*
		20170323    step2
		*/
		Mat newm_garment = m_garment3(garmentRect);
		imshow("newm_garment",newm_garment);
		Mat newGarmentMask;
		threshold(newm_garment, newGarmentMask, 200, 255, CV_THRESH_BINARY);

		//int garmentWidth = getGarmentWidth(newGarmentMask);
		int garmentWidth = garmentRect.width;
		cout << "garmentWidth height" << garmentWidth << "," << garmentHight << endl;
		Mat newm_garment2;
		resize(newm_garment, newm_garment2, cv::Size(), userWidth*1.0 / garmentWidth, userHeight*1.0 / garmentHight);
		imshow("newm_garment2", newm_garment2);
		cout << "|m_garment3_size|=>" << m_garment3.size() << endl;
		Point user_x = getPointUser_x();
		cout << "|user_x-x,y|=>" << user_x.x<<","<<user_x.y << endl;
		Point model_y = getPointModel_y();
		cout << "|model_y-x,y|=>" << model_y.x << "," << model_y.y << endl;
		//model_y.x *= m_user.cols*1.0 / m_model.cols;
		//model_y.y *= m_user.rows*1.0 / m_model.rows;
		Mat m_garment4(m_garment3.size(), m_garment3.type());
		m_garment4 = positionCorrect(user_x,model_y,m_garment3);
		circle(m_garment4, model_y,8, CV_RGB(0, 255, 255), -1, 8, 0);
		imshow("m_garment4", m_garment4);
		cout << "|m_garment4_size|=>" << m_garment4.size() << endl;
		Mat temp;
		//threshold(m_garment3, temp, 200, 255, CV_THRESH_BINARY);
		threshold(m_garment4, temp, 200, 255, CV_THRESH_BINARY);
		imshow("temp", temp);
		//Mat mbodymask2;
		Mat mbodymask(m_garment3.size(), CV_8UC1);
		//bitwise_xor(temp, Scalar(255,255,255), mbodymask2);
		//imshow("mbodymask2", mbodymask2);
		cvtColor(temp, mbodymask, CV_BGR2GRAY);
		imshow("mbodymask", mbodymask);
		//Mat m_test(m_garment3.size(), m_garment3.type());
		//m_user.copyTo(m_test, mbodymask);
		//imshow("m_test", m_test);
		imshow("m_user2", m_user);
		imshow("m_garment3", m_garment3);
		Mat m_test2(m_garment3.size(), m_garment3.type());
		m_test2 = combineMat(m_user, m_garment4, temp);
		imshow("m_test2", m_test2);
		/*Mat mbodymask2(m_garment3.size(), CV_8UC1);
		bitwise_xor(mbodymask, Scalar(255), mbodymask2);
		imshow("mbodymask2", mbodymask2);
		m_test.copyTo(m_test2, mbodymask2);
		imshow("m_test2", m_test2);
		//m_test.copyTo(m_garment3, mbodymask);
		//imshow("m_user2", m_garment3);
		//Mat mtest2 = m_garment3(mbodymask);
		//add(m_user,m_garment3, m_test, mbodymask);
		//bitwise_xor(m_garment3,m_test,m_test2);
		//imshow("m_test",m_test2);
		//cout << "|m_color_size|=>" << kin.m_Color.size() << endl;
		//imshow("m_color",kin.m_Color);*/


	}



}
