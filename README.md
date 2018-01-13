# FaceLandmark_CLMandDlibEasyTool in C++
Easy to use CLM or Dlib get landmark 

# Start Compiling CLMandDlib C++ example programs

1. Clone this project

2. Open CLMandDlib.sln

3.Set as start up prject to Sample project
![image](https://github.com/kobewangSky/FaceLandmark_CLMandDlibEasyTool/blob/master/image/Set_as_startup_project.png)

4.change solution Configurations to "Release"

5.Run

# Change Dlib to CLM face landmark

Change code CreateFaceLandmark(LandmarkType::Dlib) to CreateFaceLandmark(LandmarkType::CLM) in SampleProject.cpp

# Get Face landmark Api

FaceLandmarkAPI Temp;  
//new FaceLandmarkAPI

Temp.CreateFaceLandmark(LandmarkType::CLM);      
//Set Face landmark type

Temp.initializationDataSet(strModel, 0);          
//1.Set 3rdparty Data path, 2.Set Camera ID

Temp.Run(vPoint3dLandmarkData, matImage, true);   
//1.Get Face landmark point, 2.Get Image, 3.Show resoult

# Code architecture

![image](https://github.com/kobewangSky/FaceLandmark_CLMandDlibEasyTool/blob/master/image/Code_architecture.png)

# Conclusion

Merge CLM ans Dlib face landmark SDK，use virtual interface class to design API.
