//============================================================================
// Name        : FilteredDirectional.cpp
// Author      : Tomas Gonzalez <TomasGiS>
// Version     :
// Copyright   : <Tomas.Gonzalez@urv.cat>
// Description : Filtered Directional Matrix
//============================================================================

#include "StdAfx.h"
#include "FilteredDirectional.h"
#include "Sobel.h"
#include "limits.h"
#include <math.h>
#include <iostream>
#include <list>
#define MAX_SIZE 100 

using namespace cv;
using namespace std;

namespace Lab2 {

FilteredDirectional::FilteredDirectional(void)
{
	//Inicialitzar variables internes (si cal)
}

FilteredDirectional::~FilteredDirectional(void)
{
}

cv::Mat * FilteredDirectional::getFilteredDirectionalMatrix(cv::Mat *Gx, cv::Mat *Gy)
{
	//Utilitzant Gx i Gy calcular la Matriu direccional filtrada
	//El resultat emmagatzemar-lo a la variable FilteredDirectional::filteredDirectionalMatrix
	cv::Mat deltaX = *Gx;
	cv::Mat deltaY = *Gy;
	cv::Mat result;
	float xx=0.0;
	float yy=0.0;
	float xy=0.0;
	float PI = 3.1415926535;
	float aux;
	filteredDirectionalMatrix = Mat::zeros(deltaX.rows, deltaY.cols, CV_32F);
	
	if(deltaX.rows == deltaY.rows && deltaX.cols == deltaY.cols){
			for(int i=8; i<deltaX.rows-8; i++){
				for(int j=8; j<deltaY.cols-8; j++){
				
					xx=0.0;
					yy=0.0;
					xy=0.0;

					for(int h = i-8; h<i+8; h++){
						for(int k = j-8; k<j+8; k++){
							xx+=pow(deltaX.at<float>(h, k), 2);
							yy+=pow(deltaY.at<float>(h, k), 2);
							xy+=deltaX.at<float>(h, k)*deltaY.at<float>(h, k);
						}
					}

					aux = ((PI/2) + (atan2f((2*xy),(xx-yy))/2))/PI*255;
					aux = (aux < 0) ? 0 : (aux > 255) ? 255 : aux;
					filteredDirectionalMatrix.at<float>(i, j)=aux;
				}
			}
	}
		convertScaleAbs(filteredDirectionalMatrix, result);
		IplImage imatge = (IplImage)result;
		//cvSaveImage("c:\\IB\\data\\filt-fingerprint.bmp",&imatge);
		namedWindow("Filtered Directional", CV_WINDOW_NORMAL);
		imshow("Filtered Directional", result);
		//filteredDirectionalMatrix = result;

	return &filteredDirectionalMatrix;
	//return &(FilteredDirectional::result);
}

cv::Mat * FilteredDirectional::getSegmentedMatrix(cv::Mat *originalImage, cv::Mat *filteredDirectional, int threshold, int n)
{
	cv::Mat fD = *filteredDirectional;
	cv::Mat result;
	cv::Mat oI = *originalImage;
	float var = 0.0;
	
	segmentedMatrix = Mat::zeros(fD.rows, fD.cols, CV_32F);
	
			for(int i=n; i<fD.rows-n; i++){
				for(int j=n; j<fD.cols-n; j++){
					var = fillSignature(i,j, fD.at<float>(i,j), oI, n);
					if(var<threshold){
						segmentedMatrix.at<float>(i, j)=0.0;
					}
					else{
						segmentedMatrix.at<float>(i, j)=fD.at<float>(i, j);
					}
					
				}
			}
		
		medianBlur(segmentedMatrix, segmentedMatrix, 5);	
		convertScaleAbs(segmentedMatrix, result);
		IplImage imatge = (IplImage)result;
		//cvSaveImage("c:\\IB\\data\\segm-fingerprint.bmp",&imatge);
		namedWindow("Segmented Image", CV_WINDOW_NORMAL);
		imshow("Segmented Image", result);
		
	return &segmentedMatrix;
}

cv::Mat * FilteredDirectional::getBinarizedMatrix(cv::Mat *originalImage, cv::Mat *filteredDirectional, int n)
{
	cv::Mat fD = *filteredDirectional;
	cv::Mat result;
	cv::Mat oI = *originalImage;
	float mean = 0.0;
	
	binarizedMatrix = Mat::zeros(fD.rows, fD.cols, CV_32F);
	
			for(int i=n; i<fD.rows-n; i++){
				for(int j=n; j<fD.cols-n; j++){
					mean = fillSignatureB(i,j, fD.at<float>(i,j), oI, n);
					if(oI.at<float>(i, j)<mean){
						binarizedMatrix.at<float>(i, j)=0.0;
					}
					else{
						binarizedMatrix.at<float>(i, j)=255.00;
					}
					
				}
			}
		
		convertScaleAbs(binarizedMatrix, result);
		IplImage imatge = (IplImage)result;
		//cvSaveImage("c:\\IB\\data\\bina-fingerprint.bmp",&imatge);
		namedWindow("Binarized Image", CV_WINDOW_NORMAL);
		imshow("Binarized Image", result);
		
	return &binarizedMatrix;
}

float FilteredDirectional::fillSignature(int i, int j, float pixel, cv::Mat originalImage, int n)
{
	int tam = (2*n)+1;
	float signature[17];
	float suma = 0.0;
	float xp;
	float yp;
	float mu = 0.0;
	
		//Proyección
		for(int h=i-n; h<=i+n; h++){
			signature[h-i+n] = 0;
			//for y=y0 -m to y0+m
			for(int k = j-n; k<=j+n; k++){
				xp = ((h-i)*cosf(pixel))-((k-j)*sinf(pixel))+i;
				yp = ((h-i)*sinf(pixel))+((k-j)*cosf(pixel))+j;
				if(xp>=0 && xp<originalImage.rows && yp>=0 && yp<originalImage.cols){
					signature[h-i+n] += originalImage.at<float>((int)xp,(int)yp);
				}
			}
			signature[h-i+n] = signature[h-i+n]/tam;
		}

		//Usar la proyección aquí
		for(int h = 0; h<tam; h++){
			mu += signature[h];
		}
		mu/=tam;
	
		for(int h = 0; h<tam; h++){
			//suma += pow((signature[h]-mu), 2);
			suma += (signature[h]-mu)*(signature[h]-mu);
		}
		suma/=tam;
		
		return suma;
}

float FilteredDirectional::fillSignatureB(int i, int j, float pixel, cv::Mat originalImage, int n)
{
	int tam = (2*n)+1;
	float signature[17];
	float suma = 0.0;
	float xp;
	float yp;
	float mu = 0.0;
	
		//Proyección
		for(int h=i-n; h<=i+n; h++){
			signature[h-i+n] = 0;
			//for y=y0 -m to y0+m
			for(int k = j-n; k<=j+n; k++){
				xp = ((h-i)*cosf(pixel))-((k-j)*sinf(pixel))+i;
				yp = ((h-i)*sinf(pixel))+((k-j)*cosf(pixel))+j;
				if(xp>=0 && xp<originalImage.rows && yp>=0 && yp<originalImage.cols){
					signature[h-i+n] += originalImage.at<float>((int)xp,(int)yp);
				}
			}
			signature[h-i+n] = signature[h-i+n]/tam;
		}

		//Usar la proyección aquí
		for(int h = 0; h<tam; h++){
			mu += signature[h];
		}
		mu/=tam;

		return mu;
}




cv::Mat * FilteredDirectional::getBlobColouring(cv::Mat *binarizedImage){
	
	int region = 25;
	int neighbours = 4;
	int n1, n2, n3, n4;
	binImage = *binarizedImage;
	binImage.convertTo(binImage, CV_32F);
	cout<<"Rows: "<<binImage.rows<<endl;
	cout<<"Cols: "<<binImage.cols<<endl;
	cv::Mat labels = cv::Mat::zeros(binImage.rows, binImage.cols, CV_32F);
	cout<<"Rows: "<<labels.rows<<endl;
	cout<<"Cols: "<<labels.cols<<endl;
	std::list <float> equi;
	
	for(int x=1; x<binImage.rows-1; x++){
		for(int y=1; y<binImage.cols-1; y++){
			//cout<<x<<","<<y<<endl;
			//(x-1,y-1) (x-1, y) (x-1, y+1)
			//(x  ,y-1) (x,   y)
			if(binImage.at<float>(x, y)!=0){
				cout<<binImage.at<float>(x,y)<<" ";
				neighbours = 4;
			    n1 = n2 = n3 = n4 = 1;
				if(binImage.at<float>(x-1, y-1) == binImage.at<float>(x, y)){
					n1--;
					neighbours--;
				}
				if(binImage.at<float>(x-1, y) == binImage.at<float>(x, y)){
					n2--;
					neighbours--;
				}
				if(binImage.at<float>(x-1, y+1) == binImage.at<float>(x, y)){
					n3--;
					neighbours--;
				}
				if(binImage.at<float>(x, y-1) == binImage.at<float>(x, y)){
					n4--;
					neighbours--;
				}

				if(neighbours == 4){
					labels.at<float>(x, y) = ++region;
				}else{
					if(neighbours == 3){
						labels.at<float>(x, y) = (n1==0)?labels.at<float>(x-1, y-1):
												 (n2==0)?labels.at<float>(x-1, y):
												 (n3==0)?labels.at<float>(x-1, y+1):labels.at<float>(x, y-1);
					}else{
						if(n1 == n2){
							equi.push_back(labels.at<float>(x-1, y-1));
							equi.push_back(labels.at<float>(x-1, y));
							equi.push_back(0.0);
							labels.at<float>(x, y) = min(labels.at<float>(x-1, y-1), labels.at<float>(x-1, y));
						}
						if(n2 == n3){
							equi.push_back(labels.at<float>(x-1, y));
							equi.push_back(labels.at<float>(x-1, y+1));
							equi.push_back(0.0);
							labels.at<float>(x, y) = min(labels.at<float>(x-1, y), labels.at<float>(x-1, y+1));
						}
						if(n3 == n4){
							equi.push_back(labels.at<float>(x-1, y+1));
							equi.push_back(labels.at<float>(x, y-1));
							equi.push_back(0.0);
							labels.at<float>(x, y) = min(labels.at<float>(x-1, y+1), labels.at<float>(x-1, y+1));
						}
						
					}	
				}
			}
		}
	}

	cout<<"labels: "<<region;

	cv::convertScaleAbs(labels, labels);
	cv::namedWindow("Blob Coloured Image", CV_WINDOW_NORMAL);
	cv::imshow("Blob Coloured Image", labels);
	//	
	return &binImage;
}

void FilteredDirectional::FillRegion(int pixel, int region, int x, int y){
	binImage.at<float>(x, y) = region;
	
	 if((x-1)>=0 && pixel==binImage.at<float>(x-1,y))
		 FillRegion (pixel, region, x-1, y);

	 if((y-1)>=0 && pixel==binImage.at<float>(x,y-1)) 
		 FillRegion (pixel, region, x, y-1);

	 if((x+1)<binImage.rows && pixel==binImage.at<float>(x+1,y)) 
		 FillRegion (pixel, region, x+1, y);

	 if((y+1)<binImage.cols)
		 if(pixel==binImage.at<float>(x,y+1))
			FillRegion (pixel, region, x, y+1);

	 if((y-1)>=0 && (x-1)>=0 && pixel==binImage.at<float>(x-1,y-1)) 
		 FillRegion (pixel, region, x-1, y-1);

	 if((x+1)<binImage.rows && (y-1)>=0 && pixel==binImage.at<float>(x+1,y-1)) 
		 FillRegion (pixel, region, x+1, y-1);

	 if((x+1)<binImage.rows && (y+1)<=binImage.cols) 
		 if(pixel==binImage.at<float>(x+1,y+1))
			 FillRegion (pixel, region, x+1, y+1);

	 if((x-1)>=0 && (y+1)<binImage.cols && pixel==binImage.at<float>(x-1,y+1)) 
		 FillRegion (pixel, region, x-1, y+1);

}

cv::Mat * FilteredDirectional::FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs)
{
    blobs.clear();

    // Fill the label_image with the blobs
	// 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground
	  
	cv::Mat result = cv::Mat::zeros(binary.size(), CV_8UC3);
    cv::Mat label_image;
	cv::threshold(binary, binary, 0.0, 1.0, cv::THRESH_BINARY);
	//cv::threshold(*binarizedMatrix, *binarizedMatrix, 0.0, 1.0, cv::THRESH_BINARY);
	binary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

			for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }

	 cout<<"Blob:"<<blobs.size()<<endl;
	  for(size_t i=0; i < blobs.size(); i++) {

		if(blobs[i].size()>30){
			for(size_t j=0; j < blobs[i].size(); j++) {
				int x = blobs[i][j].x;
				int y = blobs[i][j].y;

				result.at<cv::Vec3b>(y,x)[0] = 255;
				result.at<cv::Vec3b>(y,x)[1] = 255;
				result.at<cv::Vec3b>(y,x)[2] = 255;
			
			}
		}
	  }

	  //cv::threshold(result, result, 127, 255, cv::THRESH_BINARY_INV);
	 
	  cv::namedWindow("Blob Coloured Image", CV_WINDOW_NORMAL);
	  cv::imshow("Blob Coloured Image", result);
	  IplImage imatge = (IplImage)result;
	  cvSaveImage("c:\\IB\\data\\binarised-sintetica.bmp",&imatge);
	  return &result;
}

cv::Mat FilteredDirectional::poincare(cv::Mat *originalImage, cv:: Mat *filteredDirectional){
	
	cv::Mat fD = *filteredDirectional;
	cv::Mat result;

	cv::Mat fN = Mat::zeros(fD.rows - 2, fD.cols - 2, CV_32F); //hay que quitar la primera y última fila y columna
	cv::Mat oI = *originalImage;
	int width = fD.rows;
	int height = fD.cols;
	float PI = 3.1416;
	
	int noSignature=0, whorl=0, loop=0, delta=0, menosWhorl=0, dobleWhorl=0, otro=0;
	float color;
	float values[8];
	float suma = 0.0;
	//cout<<"previo"<<endl;
	
	for(int i=1; i<width-1; i++){
		for(int j=1; j<height-1; j++){
			//fD.at<float>(i,j);
			suma = 0.0;
			values[0] = fD.at<float>(i+1,j-1) - fD.at<float>(i+1,  j);
			values[1] = fD.at<float>(i  ,j-1) - fD.at<float>(i+1,j-1);
			values[2] = fD.at<float>(i-1,j-1) - fD.at<float>(i  ,j-1);
			values[3] = fD.at<float>(i-1,  j) - fD.at<float>(i-1,j-1);
			values[4] = fD.at<float>(i-1,j+1) - fD.at<float>(i-1,  j);
			values[5] = fD.at<float>(  i,j+1) - fD.at<float>(i-1,j+1);
			values[6] = fD.at<float>(i+1,j+1) - fD.at<float>(  i,j+1);
			values[7] = fD.at<float>(i+1,  j) - fD.at<float>(i+1,j+1);

			for(int k = 0; k<8; k++){
				if (values[k]< -90){
				values[k] = values[k] + 180;
				}
				else if (values[k] > 90){
					values[k] = values[k] - 180;
				}
				suma+=values[k];
				
			}

			//esto era lo que creo que no estaba bien, ya que le quitaba dos posiciones, intenta acceder a una que no existe, la j-1, que ya no existe, la ultima es j-2
			//por lo que petaba.
			fN.at<float>(i-1,j-1) = suma;

			if(suma == -180 || suma == 180 || suma == 360 || suma==-360){
				circle( oI,Point(j,i),5.0,Scalar( 128, 128, 128 ), -1, 8);
			}
			
			switch((int)suma){
				case 0: noSignature++; color = 255; break;
                case 180: loop++; color = 0; break; 
                case 360: whorl++; color = 0; break;
                case 720: dobleWhorl++; color = 255; break;
                case -180: delta++; color = 0; break;
                case -360: menosWhorl++; color = 255; break;
                default: otro++; color = 255; break;

			}

		}
	}

	convertScaleAbs(oI, oI);
	namedWindow("Pointcaré", CV_WINDOW_NORMAL);
	imshow("Pointcaré", oI);
	//convertScaleAbs(fN, result);
	return fN;

}

void FilteredDirectional::getSingularities (cv::Mat *originalImage, cv:: Mat *filteredDirectional){
	CvFont font; 
	CvPoint pt1; 
	cv::Mat fD = *filteredDirectional;
	cv::Mat result;

	//cv::Mat fN = Mat::zeros(fD.rows - 2, fD.cols - 2, CV_32F); //hay que quitar la primera y última fila y columna
	cv::Mat filtrada = Mat::zeros(fD.rows - 2, fD.cols - 2, CV_32F);
	cv::Mat filtrada2 = Mat::zeros(fD.rows - 2, fD.cols - 2, CV_32F);
	cv::Mat oI = *originalImage;
	int width = fD.rows;
	int height = fD.cols;
	float PI = 3.1416;
	
	int noSignature=0, whorl=0, loop=0, delta=0, menosWhorl=0, dobleWhorl=0, otro=0;
	float color;
	float values[8];
	float suma = 0.0;

	//Filter again
	float xx, yy, aux;


	//crear una matriz para guardar los resultados del poincare
	//para los bordes de la matriz coger y ponerle -1, o un valor distinto
	//luego dentro aplicamos el poincare para cada punto y guardamos los valores que da, ya sea 0, 180, 360,...
	//Después, esa misma matriz, la tenemos que pasar por el filter again
	//segun Tomas solo habría que aplicarlo una vez

	
	cv::Mat fN = poincare(originalImage, filteredDirectional);

	//Creo que aquí habría que cambiar las X e Y
	//le quita 8 posiciones, y después los dos bucles de dentro don desde i-8 hasta i+8, no se el porque ese bucle
	//estaba de i = 8, por eso después hace el H = i - 8
	//Aunque deberia cambiarse a i = 0; ya que sino después habrá problemas al calcular la filtrada.at<float>(i,j)

	for(int i = 8; i<fN.rows-8; i++){
		for(int j = 8; j<fN.cols-8; j++){
					xx=0.0;
					yy=0.0;
					
					for(int h = i-8; h<i+8; h++){
						for(int k = j-8; k<j+8; k++){
							xx+=cosf(2*fN.at<float>(h, k));
							yy+=sinf(2*fN.at<float>(h, k));
						}
					}

					xx = 1/64*xx;
					yy = 1/64*yy;

					aux = (atan2f(xx, yy)/2)/PI*255;
					aux = (aux < 0) ? 0 : (aux > 255) ? 255 : aux;
					filtrada.at<float>(i-8, j-8)=aux;
		}
	}
	
	

	//BUCLE REPETIDO, PARA VER SI PUEDE FUNCIONAR. LO DEJO ASÍ PARA NO BORRAR EL OTRO

	/*for(int i = 2; i<width-2;i++){
		for(int j=2; j<height-2;j++){
			xx=0.0;
			yy=0.0;

			for(int h = i-2;h<i+1;h++){
				for(int k = j-2;k<j+1;k++){
					xx+= cosf(2*fN.at<float>(h,k));
					yy+= sinf(2*fN.at<float>(h,k));
				}
			}

			xx = xx/64;
			yy = yy/64;

			aux = (atan2f(xx,yy)/2);
			aux = (aux < 0) ? 0 : (aux > 255) ? 255 : aux;
			filtrada.at<float>(i,j) = aux;
		}
	}*/
	

	// despues de esta, habría que aplicar de nuevo el poinCare y entonces sacar los puntos, a ver si sale bien.

	fN = poincare(originalImage, &filtrada);

	//convertScaleAbs(filtrada, result);
	//namedWindow("Pointcaré", CV_WINDOW_NORMAL);
	//imshow("Pointcaré", result);
	for(int i = 8; i<fN.rows-8; i++){
		for(int j = 8; j<fN.cols-8; j++){
					xx=0.0;
					yy=0.0;
					
					for(int h = i-8; h<i+8; h++){
						for(int k = j-8; k<j+8; k++){
							xx+=cosf(2*fN.at<float>(h, k));
							yy+=sinf(2*fN.at<float>(h, k));
						}
					}

					xx = 1/64*xx;
					yy = 1/64*yy;

					aux = (atan2f(xx, yy)/2)/PI*255;
					aux = (aux < 0) ? 0 : (aux > 255) ? 255 : aux;
					filtrada2.at<float>(i-8, j-8)=aux;
		}
	}

	fN = poincare(originalImage, &filtrada2);

}



	



}


    