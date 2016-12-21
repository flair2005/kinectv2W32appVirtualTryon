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
	cout << "|kin_m_body_size|=>" << m_user.size() << endl;
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
void Transfer::transComputing(){
	//cout << "|kin_m_body_size|=>" << kin.contourRect.size() << "  |mod_m_body_size|=>" << mod.m_body.size() << endl;
	//imshow("trans_m_body",mod.m_body);
	getmUser();
	getmModel();
	getRectUser();
	getrectModel();
	getmGarment();
	Mat m_model2(m_model.size(),m_model.type(),Scalar(255));
	if (!m_user.empty()){
		resize(m_model, m_model2, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
	}
	cout << "|m_model2_size|=>" << m_model2.size() << endl;
	imshow("m_model2", m_model2);

	Mat m_garment2(m_garment.size(),m_garment.type(),Scalar(255));
	m_garment2 = m_garment(rect_model);
	cout << "|m_garment2_size|=>" << m_garment2.size() << endl;
	if (!m_garment2.empty()) {
		//Mat m_garment3(m_garment.size(), m_garment.type(), Scalar(255));
		//resize(m_garment2, m_garment3, cv::Size(), m_user.cols*1.0 / m_model.cols, m_user.rows*1.0 / m_model.rows);
		//imshow("m_garment3", m_garment3);
		imshow("m_garment2", m_garment2);
	}
}
