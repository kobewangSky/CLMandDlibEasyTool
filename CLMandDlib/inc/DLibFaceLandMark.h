#pragma once
#include "FaceLandmarkInterface.h"
#include "FaceLandmarkDefine.h"


#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

class CGazeDetection;

namespace CLMandDlib
{
	class DLibFaceLandMark : public FaceLandmarkInterface
	{
	public:

		~DLibFaceLandMark()
		{
		}

		virtual void initializationDataSet(std::string strDataPath, int nCameraID) override;
		virtual void Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage) override;

	private:

		//Output

		int m_nCameraID;
		bool m_bisStreamOpen;
		bool m_bReversal = false;
		bool m_bIniRealData = false;

		// OpenCV fields
		cv::Mat* m_pframe;
		cv::VideoCapture* m_pstream;

		void UpdateFrame(cv::Mat& matImage);
		void UpdateFaceTrackingPoints();
		void SetupValue(std::vector<cv::Point3d>& vPoint3dLandmarkData);
		void Draw(std::vector< cv::Point3d >& kDraw);

		image_window m_kwin;
		frontal_face_detector m_kdetector;
		shape_predictor m_kpose_model;

		std::vector<full_object_detection> m_kshapes;

		cv::Point m_ptPupilLeft, m_ptPupilRight;
	};
}

