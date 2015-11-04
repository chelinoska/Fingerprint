//============================================================================
// Name        : Principal.cpp
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Directional Filtered Matrix
//============================================================================

#include "StdAfx.h"

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Sobel.h"
#include "FilteredDirectional.h"
#include "MinutiaeExtraction.h"

using namespace std;
using namespace cv;

#ifdef WIN32

	static std::string arxiuEntrada = "c:\\IB\\data\\sintetica-p.bmp";
	static std::string arxiuBinariz = "c:\\IB\\data\\binarised-sintetica.bmp";
	
	static std::string arxiuSkel = "c:\\IB\\data\\skeletonized-sintetica-m.bmp";
	static std::string arxiuToWrite = "c:\\IB\\data\\minutiae_A.txt";
#else
	static std::string arxiuEntrada = "/tmp/image.png";
#endif

int main() {
	cout << "Lab 3 - Identificació biometrica" << endl;

	  cv::Mat dst, src, seg, fdi, srcGrey, segGrey, fdiGrey, fdiDst, segDst, skel;
	  Lab1::Sobel sobelFilter;
	  Lab2::FilteredDirectional filteredDirectional;
	  Lab7::MinutiaeExtraction minutiaeExtraction;
	  

	  //TODO:: identificar tipus d'imatge d'entrada
	  src = imread( arxiuEntrada );
	  //seg = imread( arxiuSegment );
	  //fdi = imread( arxiuFiltere );

	  if (!src.data ){
		  cout << "No es pot carregar la imatge de l'arxiu " << endl; // prints Lab 1 - Identificació biometrica
	  }

	  /// Convert it to gray
	  cvtColor( src, srcGrey, CV_RGB2GRAY );
	  //cvtColor( seg, segGrey, CV_RGB2GRAY );
	  //cvtColor( fdi, fdiGrey, CV_RGB2GRAY );

	  //equalizeHist(srcGrey, srcGrey);
	  
	  srcGrey.convertTo(dst, CV_32F);
	  //segGrey.convertTo(segDst, CV_32F);
	  //fdiGrey.convertTo(fdiDst, CV_32F);
	
	  namedWindow("Original Image", CV_WINDOW_NORMAL);
	  imshow("Original Image", src);
	  //imshow("Segmented", seg);
	  //imshow("Binarized", bin);

	  cv::Mat *Gx = sobelFilter.getDeltaHorizontalMatrix(dst);
	  cv::Mat *Gy = sobelFilter.getDeltaVerticalMatrix(dst);
	  cv::Mat *filteredMatrix = filteredDirectional.getFilteredDirectionalMatrix(Gx, Gy);
	  cv::Mat *segmentedMatrix = filteredDirectional.getSegmentedMatrix(&dst, filteredMatrix, 10, 8);
	  cv::Mat *binarizedMatrix = filteredDirectional.getBinarizedMatrix(&dst, segmentedMatrix, 8);
	  std::vector < std::vector<cv::Point2i > > blobs;
	  cv::Mat *colouredMatrix = filteredDirectional.FindBlobs(*binarizedMatrix, blobs);
	  cv::Mat bin = cv::imread(arxiuBinariz, 0);
	  cv::Mat *imgSkeletonized = minutiaeExtraction.skeletonize(bin);
	  skel = cv::imread(arxiuSkel, 0);
	  filteredDirectional.getSingularities(&dst, segmentedMatrix);
	  minutiaeExtraction.minutiaeDetection(&skel, segmentedMatrix, arxiuToWrite);

  	waitKey(0);
	return 0;
}