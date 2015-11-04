//============================================================================
// Name        : Sobel.cpp
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Sobel Masks
//============================================================================

#include "StdAfx.h"

#include "Sobel.h"
#include <Math.h>

using namespace cv;

namespace Lab1 {

Sobel::Sobel() {
	// TODO Auto-generated constructor stub

}

Sobel::~Sobel() {
	// TODO Auto-generated destructor stub
}


bool Sobel::applyFilter(cv::Mat *src, cv::Mat *dst, cv::Mat deltaX, cv::Mat deltaY){
if ((src == 0) || (dst ==0) ){
		return false;
	}

	/////
	//TODO: Aplicar màscara de Sobel
	
	/// Generate grad_x and grad_y
	  Mat abs_grad_x, abs_grad_y;
	  
	  /// Gradient X
	  cv::Sobel(*src, deltaX, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
	  convertScaleAbs(deltaX, abs_grad_x );

	  /// Gradient Y
	  cv::Sobel(*src, deltaY, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
	  convertScaleAbs( deltaY, abs_grad_y );

	  /// Total Gradient (approximate)
	  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, *dst);
	  
	  horizontalMatrix = abs_grad_x;
	  verticalMatrix = abs_grad_y;
	  original = *src;
	  
	////
	return true;
}

bool Sobel::getConsistencyMatrix(cv::Mat *src, cv::Mat *consistency, cv::Mat deltaX, cv::Mat deltaY){
	if ((src == 0) || (consistency ==0) ){
			return false;
		}
	
	original = *src;
	float x;
	float y;
		/////
		//TODO: Aplicar màscara de Sobel
		cv::Sobel(*src, deltaX, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
		cv::Sobel(*src, deltaY, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
		cv::Mat result = Mat::zeros(deltaX.rows, deltaY.cols, CV_32F); 
		horizontalMatrix = deltaX;
		verticalMatrix = deltaY;

	
		if(deltaX.rows == deltaY.rows && deltaX.cols == deltaY.cols){
			for(int i=0; i<deltaX.rows; i++){
				for(int j=0; j<deltaY.cols; j++){
				
					x = pow(deltaX.at<float>(i, j), 2);
					y = pow(deltaY.at<float>(i, j), 2);
					result.at<float>(i, j)=sqrt(x+y);
				}
			}

			for(int i=0; i<deltaX.rows; i++){
				for(int j=0; j<deltaY.cols; j++){
				deltaX.at<float>(i, j) = deltaX.at<float>(i, j)+128.0;
				deltaY.at<float>(i, j) = deltaY.at<float>(i, j)+128.0;
				}
			}
					
			//*consistency = result;
			convertScaleAbs(result, *consistency ); //Regresar a 8
			convertScaleAbs(deltaX, deltaX);
			convertScaleAbs(deltaY, deltaY);
		
			imshow("DX", deltaX);
			imshow("DY", deltaY);
			imshow("Consistency", *consistency);
		}	  
	return true;
}

bool Sobel::getDirectionalMatrix(cv::Mat *src, cv::Mat *directional, cv::Mat deltaX, cv::Mat deltaY){
	float x;
	float y;
	float div;
	float PI = 3.1415926535;
	float aux;
	
	//Mat abs_grad_x, abs_grad_y;
		/////
		//TODO: Aplicar màscara de Sobel
		cv::Sobel(*src, deltaX, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
		cv::Sobel(*src, deltaY, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
		cv::Mat result = Mat::zeros(deltaX.rows, deltaY.cols, CV_32F); 
	
		if(deltaX.rows == deltaY.rows && deltaX.cols == deltaY.cols){
			for(int i=0; i<deltaX.rows; i++){
				for(int j=0; j<deltaY.cols; j++){
				
					x = deltaX.at<float>(i, j);
					y = deltaY.at<float>(i, j);
		
					aux = (atan2f(x,y)/PI)*255;
					aux = (aux < 0) ? 0 : (aux > 255) ? 255 : aux;
					result.at<float>(i, j)=aux;
				}
			}
			*directional = result;
			convertScaleAbs(result, *directional ); 
			imshow("Directional", *directional);
		}

	  horizontalMatrix = deltaX;
	  verticalMatrix = deltaY;
	  original = *src;

	return true;

}

cv::Mat * Sobel::getDeltaHorizontalMatrix(cv::Mat src)
{
	original = src;
	cv::Sobel(original, horizontalMatrix, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT );
	return &horizontalMatrix;
}

cv::Mat * Sobel::getDeltaVerticalMatrix(cv::Mat src)
{
	original = src;
	cv::Sobel(original, verticalMatrix, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT );
	return &verticalMatrix;
}

} /* namespace Lab1 */
