//============================================================================
// Name        : FilteredDirectional.h
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Filtered Directional Matrix
//============================================================================


#ifndef FILTERED_DIRECTIONAL_H_
#define FILTERED_DIRECTIONAL_H_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;

namespace Lab2 {

class FilteredDirectional
{
public:
	FilteredDirectional(void);
	~FilteredDirectional(void);

	float fillSignature(int i, int j, float pixel, cv::Mat originalImage, int n);
	float fillSignatureB(int i, int j, float pixel, cv::Mat originalImage, int n);
	cv::Mat * getFilteredDirectionalMatrix(cv::Mat *Gx, cv::Mat *Gy);
	cv::Mat * FilteredDirectional::getSegmentedMatrix(cv::Mat *originalImage, cv::Mat *filteredDirectional, int threshold, int n);
	cv::Mat * FilteredDirectional::getBinarizedMatrix(cv::Mat *originalImage, cv::Mat *filteredDirectional, int n);
	cv::Mat * FilteredDirectional::getBlobColouring(cv::Mat *binarizedImage);
	void FilteredDirectional::FillRegion(int pixel, int region, int x, int y);
	void BlobErase2D(int x, int y, int c);
	void BlobColor2D(int x, int y, int c);
	void FilteredDirectional::simpleBlob();
	cv::Mat FilteredDirectional::poincare(cv::Mat *originalImage, cv:: Mat *filteredDirectional);
	void FilteredDirectional::getSingularities(cv::Mat *originalImage, cv::Mat *filteredDirectional);
	cv::Mat * FilteredDirectional::FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs);
private:

	cv::Mat filteredDirectionalMatrix;
	cv::Mat segmentedMatrix;
	cv::Mat binarizedMatrix;
	cv::Mat binImage;
	float meanX, meanY;
	int Size;
	int Color;
	int MinSize;
	int MaxSize;
	int xDim, yDim;
	int output;
	cv::Mat data1;
	cv::Mat data2;

};

}/* namespace Lab1 */


#endif /* FILTERED_DIRECTIONAL_H_ */
