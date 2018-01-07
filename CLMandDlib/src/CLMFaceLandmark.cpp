#include "CLMFaceLandmark.h"
#include "FaceLandmarkDefine.h"

// Boost includes
#include <filesystem.hpp>
#include <filesystem/fstream.hpp>
#include <stdlib.h>

using namespace CLMandDlib;


void CLMFaceLandmark::initializationDataSet( std::string strDataPath, int nCameraID)
{
	m_nCameraID = nCameraID;
	m_bisStreamOpen = false;
	m_pstream = new cv::VideoCapture();
	m_pframe = new cv::Mat();

	m_pstream->open(m_nCameraID);

	if (m_pstream->isOpened())
	{
		m_bisStreamOpen = true;
		cv::Mat TempImage;
		UpdateFrame(TempImage);

		//FaceTracking
		vector<string> arguments;

		// Some initial parameters that can be overriden from command line	
		vector<string> files, depth_directories, output_video_files, out_dummy;

		// By default try webcam 0
		int device = 0;

		arguments.push_back(strDataPath);

		m_kdet_parameters.Initial(arguments);

		// Get the input output file parameters

		// Indicates that rotation should be with respect to world or camera coordinates
		bool u;
		LandmarkDetector::get_video_input_output_params(files, depth_directories, out_dummy, output_video_files, u, arguments);

		// The modules that are being used for tracking
		m_kclnf_model.Initial(m_kdet_parameters.model_location);

		// Grab camera parameters, if they are not defined (approximate values will be used)

		// Get camera parameters
		LandmarkDetector::get_camera_params(device, m_fx, m_fy, m_cx, m_cy, arguments);

		// If cx (optical axis centre) is undefined will use the image size/2 as an estimate
		bool cx_undefined = false;
		bool fx_undefined = false;
		if (m_cx == 0 || m_cy == 0)
		{
			cx_undefined = true;
		}
		if (m_fx == 0 || m_fy == 0)
		{
			fx_undefined = true;
		}

		m_kdet_parameters.track_gaze = true;

		if (cx_undefined)
		{
			m_cx = m_pframe->cols / 2.0f;
			m_cy = m_pframe->rows / 2.0f;
		}
		// Use a rough guess-timate of focal length
		if (fx_undefined)
		{
			m_fx = 500 * (m_pframe->cols / 640.0);
			m_fy = 500 * (m_pframe->rows / 480.0);

			m_fx = (m_fx + m_fy) / 2.0;
			m_fy = m_fx;
		}

		int n = m_kclnf_model.pdm.mean_shape.rows / 3;

	}
}

void CLMFaceLandmark::Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage)
{
	while (1)
	{
		if (m_bisStreamOpen)
		{
			UpdateFrame(matImage);
			UpdateFaceTracking();
			SetupValue(vPoint3dLandmarkData);
		}
	}

}


void CLMFaceLandmark::visualise_tracking(cv::Mat& captured_image, cv::Mat_<float>& depth_image, const LandmarkDetector::CLNF& face_model, const LandmarkDetector::FaceModelParameters& det_parameters, cv::Point3f gazeDirection0, cv::Point3f gazeDirection1, int frame_count, double fx, double fy, double cx, double cy)
{
	// Drawing the facial landmarks on the face and the bounding box around it if tracking is successful and initialised
	double detection_certainty = face_model.detection_certainty;
	bool detection_success = face_model.detection_success;

	double visualisation_boundary = 0.2;

	// Only draw if the reliability is reasonable, the value is slightly ad-hoc
	if (detection_certainty < visualisation_boundary)
	{
		LandmarkDetector::Draw(captured_image, face_model);

		double vis_certainty = detection_certainty;
		if (vis_certainty > 1)
			vis_certainty = 1;
		if (vis_certainty < -1)
			vis_certainty = -1;

		vis_certainty = (vis_certainty + 1) / (visualisation_boundary + 1);

		// A rough heuristic for box around the face width
		int thickness = (int)std::ceil(2.0* ((double)captured_image.cols) / 640.0);

		cv::Vec6d pose_estimate_to_draw = LandmarkDetector::GetCorrectedPoseWorld(face_model, fx, fy, cx, cy);

		// Draw it in reddish if uncertain, blueish if certain
		LandmarkDetector::DrawBox(captured_image, pose_estimate_to_draw, cv::Scalar((1 - vis_certainty)*255.0, 0, vis_certainty * 255), thickness, fx, fy, cx, cy);

		if (det_parameters.track_gaze && detection_success && face_model.eye_model)
		{
			FaceAnalysis::DrawGaze(captured_image, face_model, gazeDirection0, gazeDirection1, fx, fy, cx, cy);
		}
	}

	// Work out the framerate
	if (frame_count % 10 == 0)
	{
		double t1 = cv::getTickCount();
		m_fps_tracker = 10.0 / (double(t1 - m_t0) / cv::getTickFrequency());
		m_t0 = t1;
	}

	// Write out the framerate on the image before displaying it
	char fpsC[255];
	std::sprintf(fpsC, "%d", (int)m_fps_tracker);
	string fpsSt("FPS:");
	fpsSt += fpsC;

	cv::putText(captured_image, fpsSt, cv::Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255, 0, 0));

	if (!det_parameters.quiet_mode)
	{

		cv::namedWindow("tracking_result", 1);
		cv::imshow("tracking_result", captured_image);

		if (!depth_image.empty())
		{
			// Division needed for visualisation purposes
			imshow("depth", depth_image / 2000.0);
		}

	}

}


void CLMFaceLandmark::UpdateFrame(cv::Mat& matImage)
{
	if (m_bisStreamOpen)
	{
		m_pstream->read(*m_pframe);
	}
	matImage = m_pframe->clone();
}

void CLMFaceLandmark::UpdateFaceTracking()
{

	// Reading the images
	cv::Mat_<float> depth_image;
	cv::Mat_<uchar> grayscale_image;

	cv::cvtColor(*m_pframe, grayscale_image, CV_BGR2GRAY);

	// The actual facial landmark detection / tracking
	bool detection_success = LandmarkDetector::DetectLandmarksInVideo(grayscale_image, depth_image, m_kclnf_model, m_kdet_parameters);

	// Visualising the results
	// Drawing the facial landmarks on the face and the bounding box around it if tracking is successful and initialised
	double detection_certainty = m_kclnf_model.detection_certainty;

	// Gaze tracking, absolute gaze direction
	m_kgazeDirection0 = { 0, 0, -1 };
	m_kgazeDirection1 = { 0, 0, -1 };


	if (m_kdet_parameters.track_gaze && detection_success && m_kclnf_model.eye_model)
	{
		FaceAnalysis::EstimateGaze(m_kclnf_model, m_kgazeDirection0, m_fx, m_fy, m_cx, m_cy, true);
		FaceAnalysis::EstimateGaze(m_kclnf_model, m_kgazeDirection1, m_fx, m_fy, m_cx, m_cy, false);
	}

	visualise_tracking(*m_pframe, depth_image, m_kclnf_model, m_kdet_parameters, m_kgazeDirection0, m_kgazeDirection1, m_bframe_count, m_fx, m_fy, m_cx, m_cy);


	// detect key presses
	char character_press = cv::waitKey(1);

	// restart the tracker
	if (character_press == 'r')
	{
		m_kclnf_model.Reset();
	}

	// Update the frame count
	m_bframe_count++;
}

void CLMFaceLandmark::SetupValue(std::vector<cv::Point3d>& vPoint3dLandmarkData)
{
	vPoint3dLandmarkData.clear();
	cv::Mat_<double> kRealTimelandmarks_3D;

	m_kclnf_model.pdm.CalcShape3D(kRealTimelandmarks_3D, m_kclnf_model.params_local);

	int n = kRealTimelandmarks_3D.rows / 3;

	m_fJawfilterTemp = 0;
	m_fPuckerTemp = 0;


	for (int i = 0; i < CLMLandmarkCount - 2; ++i)
	{
		cv::Point3d DataPoint(kRealTimelandmarks_3D.at<double>(i), kRealTimelandmarks_3D.at<double>(i + n), kRealTimelandmarks_3D.at<double>(i + n * 2));

		vPoint3dLandmarkData.push_back(DataPoint);

	}

	cv::Point3d kLeftEye(m_kgazeDirection0.x, m_kgazeDirection0.y, m_kgazeDirection0.z);
	cv::Point3d kRightEye(m_kgazeDirection1.x, m_kgazeDirection1.y, m_kgazeDirection1.z);

	vPoint3dLandmarkData.push_back(kLeftEye);
	vPoint3dLandmarkData.push_back(kRightEye);

}