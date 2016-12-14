
#include "stdafx.h"
#include "Tools.h"
#include <windows.h>
#include <Mmsystem.h>//需要 Winmm.lib库的支持 ----timeGetTime()
#include "Kinect.h"//主函数
#include "Deformation.h"
#include <cstdlib>

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
	//def->deformationInit(mod,kinect,gar);
	while(1)
	{
		kinect.Update();
		//kinect.deformation();
		//def->computing();
	}
	//system("pause");
	//delete def;
	return 0;
}


 