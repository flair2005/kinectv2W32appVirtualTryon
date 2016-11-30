
#include "stdafx.h"
#include "Tools.h"
#include <windows.h>
#include <Mmsystem.h>//需要 Winmm.lib库的支持 ----timeGetTime()
#include "Kinect.h"//主函数

int main()
{
	printf_s("start analysis Kinect for Windows V2 body data....");
	CKinect kinect;
	kinect.InitKinect();
	while(1)
	{
		kinect.Update();
	}
	return 0;
}


