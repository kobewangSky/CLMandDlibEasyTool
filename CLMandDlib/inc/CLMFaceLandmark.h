#pragma once
#include "FaceLandmarkInterface.h"
#include "LandmarkCoreIncludes.h"
#include "GazeEstimation.h"

// OpenCV includes
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <opencv2/videoio/videoio_c.h>  // Video write
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace CLMandDlib
{
	class CLMFaceLandmark : public FaceLandmarkInterface
	{
	public:

		~CLMFaceLandmark()
		{
			
		}

		virtual void initializationDataSet(std::string strDataPath, int nCameraID) override;
		virtual void Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage) override;

	private:

		//Output

		int m_nCameraID;
		bool m_bisStreamOpen;

		// OpenCV fields
		cv::Mat* m_pframe;
		cv::VideoCapture* m_pstream;

		int m_bframe_count = 0;
		float m_fx = 0;
		float m_fy = 0;
		float m_cx = 0;
		float m_cy = 0;

		LandmarkDetector::FaceModelParameters m_kdet_parameters;
		LandmarkDetector::CLNF m_kclnf_model;

		cv::Point3f m_kgazeDirection0;
		cv::Point3f m_kgazeDirection1;

		void UpdateFrame(cv::Mat& matImage);
		void UpdateFaceTracking();

		void visualise_tracking(cv::Mat& captured_image, cv::Mat_<float>& depth_image, const LandmarkDetector::CLNF& face_model, const LandmarkDetector::FaceModelParameters& det_parameters, cv::Point3f gazeDirection0, cv::Point3f gazeDirection1, int frame_count, double fx, double fy, double cx, double cy);
		void SetupValue(std::vector<cv::Point3d>& vPoint3dLandmarkData);

		float m_fJawfilterTemp;
		float m_fPuckerTemp;

		double m_fps_tracker = -1.0;
		int64 m_t0 = 0;


	};
}

