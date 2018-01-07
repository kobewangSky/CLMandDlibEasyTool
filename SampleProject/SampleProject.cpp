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

	FaceLandmarkAPI Temp;
	Temp.CreateFaceLandmark(LandmarkType::Dlib);
	Temp.initializationDataSet(strModel, 0);

	while (1)
	{
		std::vector<cv::Point3d> vPoint3dLandmarkData;
		cv::Mat matImage;
		Temp.Run(vPoint3dLandmarkData, matImage, true);
	}
	


    return 0;
}

