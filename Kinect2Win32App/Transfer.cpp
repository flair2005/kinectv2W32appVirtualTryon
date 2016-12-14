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
}
void Transfer::transComputing(){

}
