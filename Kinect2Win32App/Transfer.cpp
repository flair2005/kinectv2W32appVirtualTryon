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
	this->rect_model = mod.contourRect;
	this->rect_user = kin.contourRect;
	this->m_model = mod.m_body;
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
	m_model = mod.m_body;
	cout << "|model_m_body_size|=>" << m_model.size() << endl;
}

void Transfer::getrectModel(){
	rect_model = mod.contourRect;
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
Mat Transfer::combineMat(Mat x, Mat y, Mat mask){
	Mat dst = x.clone();
	for (int i = 0; i < x.rows; i++){
		for (int j = 0; j < x.cols; j++){
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
		user_x.x = kin.mapJoints[JointType_Neck].x - kin.contourRect.x;
		user_x.y = kin.mapJoints[JointType_Neck].y - kin.contourRect.y;
		//user_x.x = kin.mapJoints[JointType_Neck].x;
		//user_x.y = kin.mapJoints[JointType_Neck].y;
	}
	return user_x;
}
Point Transfer::getPointModel_y(){
	Point model_y(0, 0);
	if (!mod.fpt->featurePt.empty()){
		model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2 - mod.contourRect.x;
		model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2 - mod.contourRect.y;
		//model_y.x = (mod.fpt->featurePt[0].x + mod.fpt->featurePt[26].x) / 2;
		//model_y.y = (mod.fpt->featurePt[0].y + mod.fpt->featurePt[26].y) / 2;
	}
	return model_y;
}

Mat Transfer::positionCorrect(Point user_x, Point model_y, Mat src){
	Mat dst;
	const int rows = src.rows;
	const int cols = src.cols;
	int dx = user_x.x - model_y.x;
	int dy = user_x.y - model_y.y;
	dst.create(rows, cols, src.type());

	Vec3b *p;
	for (int i = 0; i < rows; i++)
	{
		p = dst.ptr<Vec3b>(i);
		for (int j = 0; j < cols; j++)
		{
			//平移后坐标映射到原图像
			int x = j + dx;
			int y = i + dy;

			//保证映射后的坐标在原图像范围内
			if (x >= 0 && y >= 0 && x < cols && y < rows)
				p[j] = src.ptr<Vec3b>(y)[x];
		}
	}
	return dst;
}

void Transfer::transComputing(){
	//cout << "|kin_m_body_size|=>" << kin.m_Color.size() << "  |mod_m_body_size|=>" << mod.m_body.size() << endl;
	//imshow("trans_m_body",mod.m_body);
	getmUser();
	getmModel();
	getRectUser();
	getrectModel();
	getmGarment();
	Mat m_model2(m_model.size(), m_model.type(), Scalar(255));
	if (!m_user.empty()){
		resize(m_model, m_model2, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
	}
	cout << "|m_model2_size|=>" << m_model2.size() << endl;
	imshow("m_model2", m_model2);
	mod.getFpt(m_model2);
	//Mat m_garment2(m_garment.size(),m_garment.type(),Scalar(255));
	Mat m_garment2;
	m_garment2 = m_garment(rect_model);
	cout << "|m_garment2_size|=>" << m_garment2.size() << endl;
	Mat m_garment3(m_garment.size(), m_garment.type(), Scalar(255));
	if (!m_garment2.empty() && !m_user.empty()) {
		resize(m_garment2, m_garment3, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
		//imshow("m_garment3", m_garment3);
		cout << "|m_garment3_size|=>" << m_garment3.size() << endl;
		Point user_x = getPointUser_x();
		Point model_y = getPointModel_y();
		Mat m_garment4(m_garment3.size(), m_garment3.type());
		m_garment4 = positionCorrect(user_x,model_y,m_garment3);
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
