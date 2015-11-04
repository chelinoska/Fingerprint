//============================================================================
// Name        : Lab2_IB.cpp
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Directional Filtered Matrix
//============================================================================

#include "stdafx.h"

#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Sobel.h"
#include "FilteredDirectional.h"

using namespace std;
using namespace cv;

#ifdef WIN32
	static std::string arxiuEntrada = "c:\\IB\\data\\imagen.jpg";
#else
	static std::string arxiuEntrada = "/tmp/image.png";
#endif

int main() {
	cout << "Lab 2 - Identificació biometrica" << endl;

	  cv::Mat dst, src, srcGrey;

	  //TODO:: identificar tipus d'imatge d'entrada
	  src = imread( arxiuEntrada );
	  if (!src.data ){
		  cout << "No es pot carregar la imatge de l'arxiu " << endl; // prints Lab 1 - Identificació biometrica
	  }

	  /// Convert it to gray
	  cvtColor( src, srcGrey, CV_RGB2GRAY );
	  srcGrey.convertTo(dst, CV_32F);


	  Lab1::Sobel sobelFilter;
	  cv::Mat deltaX;
	  cv::Mat deltaY;
	  cv::Mat consistencyMat = src.clone();
	  cv::Mat directionalMat = src.clone();

	  bool result = sobelFilter.getConsistencyMatrix(&dst,&consistencyMat,deltaX,deltaY);
	  bool result2 = sobelFilter.getDirectionalMatrix(&dst, &directionalMat, deltaX, deltaY);
	  cv::Mat *Gx = sobelFilter.getDeltaHorizontalMatrix();
	  cv::Mat *Gy = sobelFilter.getDeltaVerticalMatrix();

	  //imshow("Gx", *Gx);
	  //imshow("Gy", *Gy);
	  Lab2::FilteredDirectional filteredDirectional;
	 
	  cv::Mat *filteredMatrix = filteredDirectional.getFilteredDirectionalMatrix(Gx, Gy);
	  cv::Mat *segmentedMatrix = filteredDirectional.getSegmentedMatrix(&dst, filteredMatrix, 10, 8); 
	  //Mostrar resultat
	  waitKey(0);

	return 0;
}