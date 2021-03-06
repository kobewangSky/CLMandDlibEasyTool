#include "DLibFaceLandMark.h"

using namespace CLMandDlib;


void DLibFaceLandMark::initializationDataSet(std::string strDataPath, int nCameraID)
{
	std::string strface_landmarks = strDataPath + "\\Dlib\\shape_predictor_68_face_landmarks.dat";
	m_nCameraID = nCameraID;
	m_bisStreamOpen = false;
	m_pstream = new cv::VideoCapture();
	m_pframe = new cv::Mat();

	m_pstream->open(nCameraID);

	if (m_pstream->isOpened())
	{
		m_bisStreamOpen = true;

		// Load face detection and pose estimation models.
		m_kdetector = get_frontal_face_detector();

		deserialize(strface_landmarks) >> m_kpose_model;
	}

}

void DLibFaceLandMark::Run(std::vector<cv::Point3d>& vPoint3dLandmarkData, cv::Mat& matImage, bool bShowResoult)
{
	if (m_bisStreamOpen)
	{
		UpdateFrame(matImage);
		UpdateFaceTracking(bShowResoult);
		SetupValue(vPoint3dLandmarkData);
	}
}


void DLibFaceLandMark::UpdateFrame(cv::Mat& matImage)
{
	if (m_bisStreamOpen)
	{
		m_pstream->read(*m_pframe);
	}

	if (m_bReversal)
	{
		cv::flip(*m_pframe, *m_pframe, 0);
	}
	matImage = *m_pframe;
}

void DLibFaceLandMark::UpdateFaceTracking(bool bShowResoult)
{
	// Turn OpenCV's Mat into something dlib can deal with.  Note that this just
	// wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
	// long as temp is valid.  Also don't do anything to temp that would cause it
	// to reallocate the memory which stores the image as that will make cimg
	// contain dangling pointers.  This basically means you shouldn't modify temp
	// while using cimg.

	cv_image<bgr_pixel> cimg(*m_pframe);

	// Detect faces 
	std::vector<rectangle> faces = m_kdetector(cimg);
	// Find the pose of each face.

	m_kshapes.clear();

	for (unsigned long i = 0; i < faces.size(); ++i)
		m_kshapes.push_back(m_kpose_model(cimg, faces[i]));

	// Display it all on the screen
	m_kwin.clear_overlay();
	m_kwin.set_image(cimg);
	m_kwin.add_overlay(render_face_detections(m_kshapes));

	if (m_kshapes.size() > 0)
	{
		std::vector< cv::Point3d > kSourceData;
		std::vector< cv::Point3d > kFilterData;


		for (int i = 0; i < m_kshapes[0].num_parts(); ++i)
		{
			kSourceData.push_back(cv::Point3d(m_kshapes[0].part(i).x(), m_kshapes[0].part(i).y(), m_kshapes[0].part(i).z()));
		}

		kSourceData.push_back(cv::Point3d(m_ptPupilLeft.x, m_ptPupilLeft.y, 0));
		kSourceData.push_back(cv::Point3d(m_ptPupilRight.x, m_ptPupilRight.y, 0));

		if ( bShowResoult )
		{
			Draw(kSourceData);
		}	

	}

}

void DLibFaceLandMark::Draw(std::vector< cv::Point3d >& kDraw)
{

	std::vector<image_window::overlay_circle> kcircle;

	for (int i = 0; i < kDraw.size(); ++i)
	{
		point DrawPoint(kDraw[i].x, kDraw[i].y);
		kcircle.push_back(image_window::overlay_circle(DrawPoint, 3, rgb_pixel(255, 0, 0)));
	}
	m_kwin.add_overlay(kcircle);
}


void DLibFaceLandMark::SetupValue(std::vector<cv::Point3d>& vPoint3dLandmarkData)
{
	vPoint3dLandmarkData.clear();
	if (m_kshapes.size() == 0)
	{
		return;
	}
	for (int i = 0; i < DlibLandmarkCount; ++i)
	{
		cv::Point3d DataPoint(m_kshapes[0].part(i).x(), m_kshapes[0].part(i).y(), m_kshapes[0].part(i).z());

		vPoint3dLandmarkData.push_back(DataPoint);
	}

}