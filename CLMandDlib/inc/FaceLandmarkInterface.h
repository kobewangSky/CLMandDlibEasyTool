#pragma once

#include "FaceLandmarkDefine.h"
#include <opencv2/opencv.hpp>

namespace CLMandDlib
{
	class FaceLandmarkInterface
	{
	public:
		virtual void initializationDataSet( std::string strDataPath, int nCameraID) = 0;
		virtual void Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage, bool bShowResoult) = 0;
	};
}