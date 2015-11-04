#include "StdAfx.h"
#include "FilteredDirectional.h"
#include "MinutiaeExtraction.h"
#include "Sobel.h"
#include <math.h>
#include <iostream>
#define MAX_SIZE 100 

using namespace cv;
using namespace std;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

namespace Lab7 {

MinutiaeExtraction::MinutiaeExtraction(void)
{
	//Inicialitzar variables internes (si cal)
}

MinutiaeExtraction::~MinutiaeExtraction(void)
{
}

cv::Mat * MinutiaeExtraction::skeletonize(cv::Mat originalImage){

	cv::Mat img = originalImage;
	//cv::Mat img;
	//cv::threshold(img, img, 127, 255, cv::THRESH_BINARY); 
	cv::threshold(img, img, 127, 255, cv::THRESH_BINARY_INV);
	cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
	cv::Mat temp;
	cv::Mat eroded;
 
	cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
 
	bool done;		
	do
	{
	  cv::erode(img, eroded, element);
	  cv::dilate(eroded, temp, element); // temp = open(img)
	  cv::subtract(img, temp, temp);
	  cv::bitwise_or(skel, temp, skel);
	  eroded.copyTo(img);
 
	  done = (cv::countNonZero(img) == 0);
	} while (!done);

	cv::threshold(skel, skel, 127, 255, cv::THRESH_BINARY_INV);
	IplImage imatge = (IplImage)skel;
	cvSaveImage("c:\\IB\\data\\skeletonized-sintetica-m.bmp",&imatge);
	namedWindow("Skeleton", CV_WINDOW_NORMAL);
	cv::imshow("Skeleton", skel);

	return &skel;
}


void Morphology_Operations( cv::Mat src, cv::Mat dst)
{
  // Since MORPH_X : 2,3,4,5 and 6
  int operation = morph_operator + 2;

  Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

  /// Apply the specified morphology operation
  morphologyEx( src, dst, operation, element );
  imshow( "Ultima", dst );
}

bool  detectDifferences(float first, float second){
	bool notEquals = false;

	return (first - second);
}

int incrementChanges (bool notEquals){

	if(notEquals){
		return 1;
	}
	else{
		return 0;
	}

}


void writeIntoFile(string name, char* text){
	FILE* file;
	std::string str = name;
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());

	if((file=fopen(cstr,"at"))==NULL){
		std::cout<<"Error al abrir el fichero"<<std::endl;
	}
	else{
		fputs(text, file);
	}
	// do stuff
	delete [] cstr;
	fclose(file);
}

void addMinutiae(string name, char type, int x, int y, int angle){
	char line[50];
	char type_c[15];
	char x_c[5];
	char y_c[5];
	char a_c[5];

	sprintf(type_c, "%c", type);
	sprintf(x_c, "%d", x);
	sprintf(y_c, "%d", y);
	sprintf(a_c, "%d", angle);
	strcpy(line,"");
	strcat(line,type_c);
	strcat(line,",");
	strcat(line, x_c);
	strcat(line,",");
	strcat(line,y_c);
	strcat(line,",");
	strcat(line,a_c);
	strcat(line,"\n");
	writeIntoFile(name, line);

	strcpy(line,"");
}


void MinutiaeExtraction::minutiaeDetection (cv:: Mat *skeletonImage, cv:: Mat *segmentedMatrix, string name){
	
	int numTerminations, numBifurcations, numIntermediate, noDiferences, others;
	cv::Mat result;
	cv:: Mat mA = *segmentedMatrix;
	cv:: Mat iS = *skeletonImage;
	iS.convertTo(iS, CV_32F);
	int width = iS.rows;
	int height = iS.cols;
	int numChanges = 0;
	int angulo = 0;
	int notEquals[8];
	int i,j;
	float cn = 0;
	numTerminations = numBifurcations = numIntermediate = noDiferences = others = 0;

	//convertScaleAbs(iS, result);
	//namedWindow("MinutiaeDetectionOriginal", CV_WINDOW_NORMAL);
	//imshow("MinutiaeDetectionOriginal", result);


	//Como no debe superar el límite y necesitamos bloques de 3x3, hay que controlar para que no
	//intente consultar una posicion que no exista
	for(i=5;i<width-7;i++){
		for(j=5;j<height-7;j++){
			if(iS.at<float>(i+1, j+1) == 0){
				
				notEquals[0] = detectDifferences(iS.at<float>(i,j),iS.at<float>(i,j+1));
				numChanges = numChanges + incrementChanges(notEquals[0]);
			
				notEquals[1] = detectDifferences(iS.at<float>(i,j+1),iS.at<float>(i,j+2));
				numChanges = numChanges + incrementChanges(notEquals[1]);

				notEquals[2] = detectDifferences(iS.at<float>(i,j+2),iS.at<float>(i+1,j+2));
				numChanges = numChanges + incrementChanges(notEquals[2]);

				notEquals[3] = detectDifferences(iS.at<float>(i+1,j+2),iS.at<float>(i+2,j+2));
				numChanges = numChanges + incrementChanges(notEquals[3]);

				notEquals[4] = detectDifferences(iS.at<float>(i+2,j+2),iS.at<float>(i+2,j+1));
				numChanges = numChanges + incrementChanges(notEquals[4]);

				notEquals[5] = detectDifferences(iS.at<float>(i+2,j+1),iS.at<float>(i+2,j));
				numChanges = numChanges + incrementChanges(notEquals[5]);

				notEquals[6] = detectDifferences(iS.at<float>(i+2,j),iS.at<float>(i+1,j));
				numChanges = numChanges + incrementChanges(notEquals[6]);

				notEquals[7] = detectDifferences(iS.at<float>(i+1,j),iS.at<float>(i,j));
				numChanges = numChanges + incrementChanges(notEquals[7]);


				cn = numChanges / 2;
				//cout<<"cn: "<<cn<<endl;

				if((cn == 1)&&(numChanges%2 == 0)){
					numTerminations ++;
					circle( iS,Point(j,i),3.0,Scalar( 168, 168, 168 ), -1, 8);
					angulo = mA.at<float>(i+1,j+1);
					addMinutiae(name, 'T', i+1, j+1, angulo);
				
				//	cout<<"angulo: "<<angulo<<endl;
					/////
					//angulo = whichDifferent(notEquals);

				}
				else{
					if(cn == 2){
						numIntermediate ++;
					//	cout<<"minutia intermedia"<<endl;
					//	cout <<"x "<<i<<endl;
					//	cout <<"y "<<j<<endl;
					}
					else{
						if(cn >= 3){
							numBifurcations ++;
							circle( iS,Point(j,i),3.0,Scalar( 90, 90, 90), -1, 8);
							angulo = mA.at<float>(i+1,j+1);
							//cout<<"angulo: "<<angulo<<endl;
							addMinutiae(name, 'B', i+1, j+1, angulo);
				
							//acabo de cambiar para añadir los angulos
							//escribirlo para hacer matching dsps
						}
						else{
							if(cn == 0){
								noDiferences ++;
							}
						//	else{
						//		cout<<cn<<endl;
						//		others ++;
						//	}
						}
					}
				}
				for(int k=0;k<8;k++){
					notEquals[k]= 0;
				}
				numChanges = 0;
			}
		}
		
		for(int k=0;k<8;k++){
			notEquals[k]= 0;
		}
		numChanges = 0;
		
	}
	//cout<<i<<endl;
	//cout<<j<<endl;
	//cout<<" "<<endl;

	cout<<"NumTerminations"<<endl;
	cout<<numTerminations<<endl;
	cout<<"NumBifurcations"<<endl;
	cout<<numBifurcations<<endl;
	cout<<"NumIntermediate"<<endl;
	cout<<numIntermediate<<endl;
	cout<<"noDiferences"<<endl;
	cout<<noDiferences<<endl;
	//cout<<"Others"<<endl;
	//cout<<others<<endl;

	convertScaleAbs(iS, result);
	IplImage imatge = (IplImage)result;
	cvSaveImage("c:\\IB\\data\\minutiae-sintetica.bmp",&imatge);
	namedWindow("MinutiaeDetection", CV_WINDOW_NORMAL);
	imshow("MinutiaeDetection", result);

}

}