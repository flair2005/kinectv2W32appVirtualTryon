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
void Transfer::getKinect(CKinect kinect){
	kin = kinect;
	//rect_user = kinect.contourRect;
	//m_user = kinect.m_body;
}
void Transfer::transComputing(){
	cout << "|kin_m_body_size|=>" << kin.contourRect.size() << "  |mod_m_body_size|=>" << mod.m_body.size() << endl;
	//imshow("trans_m_body",mod.m_body);
}
