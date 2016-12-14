#include "stdafx.h"
#include "Tools.h"
#include <windows.h>
#include <Mmsystem.h>//需要 Winmm.lib库的支持 ----timeGetTime()
//#include "Kinect.h"//主函数
#include "Defor.h"
#include <cstdlib>
#include "Transfer.h"

int main()
{
	printf_s("start analysis Kinect for Windows V2 body data....");
	Garment gar;
	gar.garmentInitial();
	Model mod;
	mod.modelInitial();
	CKinect kinect;
	kinect.InitKinect(mod);
	//Deformation* def= new Deformation();
	//Deformation def;
	//def->deformationInit(mod,kinect,gar);
	Transfer tra;
	while(1)
	{
		kinect.Update();
		tra.transComputing();
	}

	return 0;
}


