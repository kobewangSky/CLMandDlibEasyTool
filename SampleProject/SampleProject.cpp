// SampleProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "FaceLandmarkInterface.h"
#include "FaceLandmarkDefine.h"
#include "FaceLandmarkAPI.h"

using namespace CLMandDlib;

int main()
{
	//Get Current Dir
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pwd);
	std::string strcurrent_working_dir(pwd);
	auto Endit = strcurrent_working_dir.find_last_of("\\");
	std::string  strModel = strcurrent_working_dir.substr(0, Endit);
	strModel = strModel + "\\CLMandDlibSDK";

	FaceLandmarkAPI Temp;							//new FaceLandmarkAPI
	Temp.CreateFaceLandmark(LandmarkType::CLM);		//Set Face landmark type
	Temp.initializationDataSet(strModel, 0);		//1.Set 3rdparty Data path, 2.Set Camera ID

	while (1)
	{
		std::vector<cv::Point3d> vPoint3dLandmarkData;
		cv::Mat matImage;
		Temp.Run(vPoint3dLandmarkData, matImage, true);	//1.Get Face landmark point, 2.Get Image, 3.Show result
	}
	


    return 0;
}

