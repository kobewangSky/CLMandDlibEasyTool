#pragma once
#include "FaceLandmarkInterface.h"
#include "FaceLandmarkDefine.h"
#include <memory>

namespace CLMandDlib
{
	class FaceLandmarkAPI
	{
	public:
		void CreateFaceLandmark(LandmarkType type);
		void initializationDataSet(std::string strDataPath, int nCameraID);
		void Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage, bool bShowResoult);

	private: 
		std::shared_ptr< FaceLandmarkInterface > m_pFaceLandmarkInterface;
	};
}