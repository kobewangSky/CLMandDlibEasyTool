#include "FaceLandmarkAPI.h"
#include "CLMFaceLandmark.h"

using namespace CLMandDlib;


FaceLandmarkAPI::FaceLandmarkAPI(LandmarkType type)
{
	if (type == CLM)
	{
		m_pFaceLandmarkInterface = std::make_shared<CLMFaceLandmark>();
	}
	else if ( type == Dlib )
	{

	}

}

void FaceLandmarkAPI::initializationDataSet( std::string strDataPath, int nCameraID)
{
	m_pFaceLandmarkInterface->initializationDataSet( strDataPath, nCameraID);
}

void FaceLandmarkAPI::Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage)
{
	m_pFaceLandmarkInterface->Run(vPoint3dLandmarkData, matImage);
}

