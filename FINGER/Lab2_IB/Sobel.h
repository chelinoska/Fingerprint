//============================================================================
// Name        : Sobel.h
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Sobel Masks
//============================================================================


#ifndef SOBEL_H_
#define SOBEL_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

namespace Lab1 {

class Sobel {
public:
	Sobel();
	virtual ~Sobel();

	bool applyFilter(cv::Mat *src, cv::Mat *dst, cv::Mat deltaX, cv::Mat deltaY);

	//or

	bool getConsistencyMatrix(cv::Mat *src, cv::Mat *consistency, cv::Mat deltaX, cv::Mat deltaY);
	bool getDirectionalMatrix(cv::Mat *src, cv::Mat *directional, cv::Mat deltaX, cv::Mat deltaY);
	cv::Mat * getDeltaHorizontalMatrix(cv::Mat src);
	cv::Mat * getDeltaVerticalMatrix(cv::Mat src);

private:
	cv::Mat horizontalMatrix;
	cv::Mat verticalMatrix;
	cv::Mat original;
};

} /* namespace Lab1 */


#endif /* SOBEL_H_ */
