#include "FaceLandmarkAPI.h"
#include "CLMFaceLandmark.h"
#include "DLibFaceLandMark.h"

using namespace CLMandDlib;



void FaceLandmarkAPI::CreateFaceLandmark(LandmarkType type)
{
	if (type == CLM)
	{
		m_pFaceLandmarkInterface = std::make_shared<CLMFaceLandmark>();
	}
	else if (type == Dlib)
	{
		m_pFaceLandmarkInterface = std::make_shared<DLibFaceLandMark>();
	}
}

void FaceLandmarkAPI::initializationDataSet( std::string strDataPath, int nCameraID)
{
	m_pFaceLandmarkInterface->initializationDataSet( strDataPath, nCameraID);
}

void FaceLandmarkAPI::Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage, bool bShowResoult)
{
	m_pFaceLandmarkInterface->Run(vPoint3dLandmarkData, matImage, bShowResoult);
}

