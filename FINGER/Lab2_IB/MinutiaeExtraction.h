#ifndef MINUTIAE_EXTRACTION_H_
#define MINUTIAE_EXTRACTION_H_

#include "stdafx.h"
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

namespace Lab7 {

class MinutiaeExtraction
{
public:
	MinutiaeExtraction(void);
	~MinutiaeExtraction(void);
	cv::Mat * MinutiaeExtraction::skeletonize(cv::Mat originalImage);
	void MinutiaeExtraction::minutiaeDetection(cv::Mat *skeletonImage, cv::Mat *segmentedMatrix, string name);
	void Morphology_Operations( cv::Mat src, cv::Mat dst );

private:

};

}/* namespace Lab7 */


#endif 
