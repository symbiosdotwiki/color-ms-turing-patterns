#include "turing.h"
#include <iostream>
#include <algorithm>
#include <Windows.h>

Turing::Turing(bool sym, int type, int w, int h){
	symm = sym;
	color = true;
	colors = 3;
	addType = type;
	InitTuring(aDef, iDef, saDef, wDef, sDef, cIntDef, w, h);
	InitRand(w, h);
}

Turing::Turing(bool sym){
	Turing(sym, 0, 500, 500);
}

Turing::Turing(sf::Image img, int rgbC, bool sym){
	symm = sym;
	color = false;
	colors = 1;
	rgb = rgbC;
	InitTuring(aDef, iDef, saDef, wDef, sDef, NULL, img.getSize().y, img.getSize().x);
	InitImg(img.getSize().y, img.getSize().x, img);
}

Turing::Turing(sf::Image img, bool sym){
	symm = sym;
	color = true;
	colors = 3;
	InitTuring(aDef, iDef, saDef, wDef, sDef, cIntDef, img.getSize().y, img.getSize().x);
	InitImg(img.getSize().y, img.getSize().x, img);
}

Turing::Turing(const std::vector<int> acts, const std::vector<int> inhibs, const std::vector<double> smalls,
	const std::vector<int>w, const std::vector<int>syms, const int resW, const int resH){
	symm = true;
	color = false;
	colors = 1;
	InitTuring(acts, inhibs, smalls, w, syms, NULL, resW, resH);
	InitRand(resW, resH);
}

void Turing::InitTuring(const std::vector<int> acts, const std::vector<int> inhibs,
	const std::vector<double> smalls, const std::vector<int> w, const std::vector<int> syms,
	const double cInts[3], const int resW, const int resH){
	activators = (int*)malloc(scales*sizeof(int));
	inhibitors = (int*)malloc(scales*sizeof(int));
	weights = (int*)malloc(scales*sizeof(int));
	symmetries = (int*)malloc(scales*sizeof(int));
	smallAmounts = (double*)malloc(scales*sizeof(double));

	//Define main variables
	for (int i = 0; i < scales; i++){
		activators[i] = acts.at(i);
		inhibitors[i] = inhibs.at(i);
		smallAmounts[i] = smalls.at(i);
		weights[i] = w.at(i);
		symmetries[i] = syms.at(i);
	}

	if (color){
		colorInt = (double*)malloc(3 * sizeof(double));
		for (int i = 0; i < 3; i++){
			colorInt[i] = cInts[i];
		}
	}

	resWidth = resW;
	resHeight = resH;

	//Setting array sizes
	pattern = (double***)malloc(colors*sizeof(double**));
	dummy = (double***)malloc(colors*sizeof(double**));
	for (int j = 0; j < colors; j++){
		pattern[j] = (double**)malloc(resWidth*sizeof(double*));
		dummy[j] = (double**)malloc(resWidth*sizeof(double*));
		for (int k = 0; k < resWidth; k++){
			pattern[j][k] = (double*)malloc(resHeight*sizeof(double));
			dummy[j][k] = (double*)malloc(resHeight*sizeof(double));
		}
	}

	activatorM = (double****)malloc(scales*sizeof(double***));
	inhibitorM = (double****)malloc(scales*sizeof(double***));
	variationM = (double****)malloc(scales*sizeof(double***));
	aiDiffM = (double****)malloc(scales*sizeof(double***));
	for (int i = 0; i < scales; i++){
		activatorM[i] = (double***)malloc(colors*sizeof(double**));
		inhibitorM[i] = (double***)malloc(colors*sizeof(double**));
		variationM[i] = (double***)malloc(colors*sizeof(double**));
		aiDiffM[i] = (double***)malloc(colors*sizeof(double**));
		for (int j = 0; j < colors; j++){
			activatorM[i][j] = (double**)malloc(resWidth*sizeof(double*));
			inhibitorM[i][j] = (double**)malloc(resWidth*sizeof(double*));
			variationM[i][j] = (double**)malloc(resWidth*sizeof(double*));
			aiDiffM[i][j] = (double**)malloc(resWidth*sizeof(double*));
			for (int k = 0; k < resWidth; k++){
				activatorM[i][j][k] = (double*)malloc(resHeight*sizeof(double));
				inhibitorM[i][j][k] = (double*)malloc(resHeight*sizeof(double));
				variationM[i][j][k] = (double*)malloc(resHeight*sizeof(double));
				aiDiffM[i][j][k] = (double*)malloc(resHeight*sizeof(double));
			}
		}
	}
}

void Turing::InitRand(int resWidth, int resHeight){
	//Initial randomization of main array
	for (int i = 0; i < colors; i++){
		for (int j = 0; j < resWidth; j++){
			for (int k = 0; k < resHeight; k++){
				pattern[i][j][k] = CalcSA(i, j, k) * (double)rand() / RAND_MAX;
			}
		}
	}
}

void Turing::InitImg(int resWidth, int resHeight, sf::Image img){
	//Initial image as array
	for (int i = 0; i < resWidth; i++){
		for (int j = 0; j < resHeight; j++){
			if (color){
				pattern[0][i][j] = (double)(img.getPixel(j, i).r) / 255;
				pattern[1][i][j] = (double)(img.getPixel(j, i).g) / 255;
				pattern[2][i][j] = (double)(img.getPixel(j, i).b) / 255;
			}
			else{
				if (rgb == 0)
					pattern[0][i][j] = (double)(img.getPixel(j, i).r) / 255;
				if (rgb == 1)
					pattern[0][i][j] = (double)(img.getPixel(j, i).g) / 255;
				if (rgb > 1)
					pattern[0][i][j] = (double)(img.getPixel(j, i).b) / 255;
			}
		}
	}
}

void Turing::iterate(){
	Activator();
	if (symm)
		RActivator();
	Inhibitor();
	if (symm)
		RInhibitor();
	AIDiff();
	Variation();
	UpdatePixels();
	Normalize();
}

double *** Turing::GetArray(){
	return pattern;
}

double Turing::GetPixel(int c, int x, int y){
	return GetMVal(pattern[c], x, y);
}

double Turing::GetMVal(double **mat, int x, int y){
	if (x >= resWidth || y >= resHeight)
		return GetMVal(mat, x%resWidth, y%resHeight);
	if (x < 0)
		return GetMVal(mat, x + (abs(x / resWidth) + 1)*resWidth, y);
	if (y < 0)
		return GetMVal(mat, x, y + (abs(y / resHeight) + 1)*resHeight);
	//std::cout<< x << " " << y << std::endl;
	return mat[x][y];
}

double Turing::CalcSA(int color, int x, int y){
	/*if(color == 0)
	return sqrt((cos((double)((x-resWidth/2)*(x-resWidth/2) +
	(y-resHeight/2)*(y-resHeight/2))/6000.0)+1.5));
	if(color == 1)
	return sqrt((cos((double)(y-resHeight/2)*
	(double)(x-resWidth/2)/4000.0)+1.5));
	else
	return sqrt((cos((double)(x-resWidth/2)/200)+1.5));*/
	return 1.0;
}

void Turing::Activator(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			Activator(i, j);
		}
	}
}

void Turing::Activator(int sn, int cn){
	SetA(sn, cn);
}

void Turing::RActivator(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			RActivator(i, j);
		}
	}
}

void Turing::RActivator(int sn, int cn){
	RAvgM(activatorM[sn][cn], sn);
}

void Turing::Inhibitor(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			Inhibitor(i, j);
		}
	}
}

void Turing::Inhibitor(int sn, int cn){
	SetI(sn, cn);
}

void Turing::RInhibitor(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			RInhibitor(i, j);
		}
	}
}

void Turing::RInhibitor(int sn, int cn){
	RAvgM(inhibitorM[sn][cn], sn);
}

void Turing::Variation(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			Variation(i, j);
		}
	}
}

void Turing::Variation(int sn, int cn){
	SetV(sn, cn);
}

void Turing::AIDiff(){
	for (int i = 0; i < scales; i++){
		for (int j = 0; j < colors; j++){
			AIDiff(i, j);
		}
	}
}

void Turing::AIDiff(int sn, int cn){
	MatrixSubtract(activatorM[sn][cn], inhibitorM[sn][cn], aiDiffM[sn][cn], resWidth, resHeight);
}

void Turing::UpdatePixels(){
	//Go through every pixel
	double * wVars = (double*)malloc(scales*sizeof(double));
	int * minElems = (int*)malloc(addType*sizeof(int));
	for (int c = 0; c < colors; c++){
		for (int i = 0; i < resWidth; i++){
			for (int j = 0; j < resHeight; j++){
				//Find scale with smallest variation
				for (int k = 0; k < scales; k++)
					wVars[k] = variationM[k][c][i][j];
				minElements(wVars, scales, minElems, addType);
				//Update each pixel
				for (int l = 0; l < addType; l++){
					//std::cout << minElems[l] << std::endl;
					//Sleep(500);
					pattern[c][i][j] += sgn(wVars[minElems[l]]) * smallAmounts[minElems[l]] / addType;
				}
			}
		}
	}

	free(wVars);
	free(minElems);
}

void Turing::minElements(double *list, int listSize, int *minElems, int numElems){
	if (listSize >= numElems){
		double * listABS = (double*)malloc(listSize*sizeof(double));
		for (int i = 0; i < listSize; i++){
			listABS[i] = fabs(list[i]);
		}
		int maxInd = std::distance(listABS, std::max_element(listABS, listABS + listSize));
		for (int i = 0; i < numElems; i++){
			minElems[i] = maxInd;
			//std::cout << maxInd << std::endl;
			//Sleep(500);
		}
		for (int i = 0; i < listSize; i++){
			for (int j = 0; j < numElems; j++){
				if (listABS[i] < listABS[minElems[j]]){
					if (j + 1 < numElems)
						minElems[j + 1] = minElems[j];
					minElems[j] = i;
					//std::cout << i;
					break;
				}
			}
		}

		free(listABS);
	}
}

void Turing::Normalize(){
	for (int i = 0; i < colors; i++)
		Normalize(i);
}

void Turing::Normalize(int c){
	double min, max;
	double v = GetPixel(c, 0, 0);

	min = v;
	max = v;

	//Find min and max values over pattern array
	for (int i = 0; i < resWidth; i++){
		for (int j = 0; j < resHeight; j++){
			v = GetPixel(c, i, j);
			if (v < min)
				min = v;
			else
				if (v > max)
					max = v;
		}
	}

	//Normalize each pixel of the pattern
	for (int i = 0; i < resWidth; i++){
		for (int j = 0; j < resHeight; j++){
			pattern[c][i][j] = (GetPixel(c, i, j) - min) / (max - min);
		}
	}
}

void Turing::SetA(int s, int c){
	//Sets the scale s activator
	AvgH(activators[s], pattern[c], dummy[c]);
	AvgV(activators[s], dummy[c], activatorM[s][c]);
	for (int i = 0; i < blurnum - 1; i++){
		AvgH(activators[s], activatorM[s][c], dummy[c]);
		AvgV(activators[s], dummy[c], activatorM[s][c]);
	}
}

void Turing::SetI(int s, int c){
	//Sets the scale s inhibitor
	AvgH(inhibitors[s], pattern[c], dummy[c]);
	AvgV(inhibitors[s], dummy[c], inhibitorM[s][c]);
	for (int i = 0; i < blurnum - 1; i++){
		AvgH(inhibitors[s], inhibitorM[s][c], dummy[c]);
		AvgV(inhibitors[s], dummy[c], inhibitorM[s][c]);
	}
}

void Turing::SetV(int s, int c){
	//Sets the scale s variation
	AvgH(varrad, aiDiffM[s][c], dummy[c]);
	AvgV(varrad, dummy[c], variationM[s][c]);
	for (int i = 0; i < blurnum - 1; i++){
		AvgH(varrad, variationM[s][c], dummy[c]);
		AvgV(varrad, dummy[c], variationM[s][c]);
	}
}

void Turing::RAvgM(double **list, int s){
	//Symmetrizes s activator
	double theta = 6.283185 / symmetries[s];
	for (int i = 0; i < resWidth; i++){
		for (int j = 0; j < resHeight; j++){
			double total = GetMVal(list, i, j);
			for (int r = 1; r < symmetries[s]; r++){
				//Normal
				int x = i - resWidth / 2;
				int y = resHeight / 2 - j;
				double rad = sqrt((double)x*x + (double)y*y);
				double phi = atan2(y, x);
				total += GetMVal(list, (int)(rad*cos(phi + r*theta)) + resWidth / 2, resHeight / 2 - (int)(rad*sin(phi + r*theta)));

				//Other
				/*int x = i;
				int y = j;
				double rad = sqrt((double)x*x+(double)y*y);
				double phi = atan2(y,x);
				total += GetMVal(list[s], (int)(rad*cos(phi+r*theta)), (int)(rad*sin(phi+r*theta)));
				*/
			}
			list[i][j] = total / symmetries[s];
		}
	}
}

void Turing::MatrixSubtract(double **pos, double **neg, double **dest, int x, int y){
	for (int i = 0; i < x; i++){
		for (int j = 0; j < y; j++){
			dest[i][j] = GetMVal(pos, i, j) - GetMVal(neg, i, j);
		}
	}
}

void Turing::AvgH(int r, double **source, double **dest){
	for (int i = 0; i < resWidth; i++){
		// Process entire window for first pixel
		double total = 0;
		for (int y = -r; y <= r; y++)
			total += GetMVal(source, i, y);
		dest[i][0] = total / (double)(r * 2 + 1);
		// Subsequent pixels just update window total
		for (int j = 1; j < resHeight; j++){
			// Subtract pixel leaving window
			total -= GetMVal(source, i, j - r - 1);

			// Add pixel entering window
			total += GetMVal(source, i, j + r);

			dest[i][j] = total / (double)(r * 2 + 1);
		}
	}
}

void Turing::AvgV(int r, double **source, double **dest){
	for (int j = 0; j < resHeight; j++){
		// Process entire window for first pixel
		double total = 0;
		for (int x = -r; x <= r; x++)
			total += GetMVal(source, x, j);
		dest[0][j] = total / (double)(r * 2 + 1);

		// Subsequent pixels just update window total
		for (int i = 1; i < resWidth; i++){
			// Subtract pixel leaving window
			total -= GetMVal(source, i - r - 1, j);

			// Add pixel entering window
			total += GetMVal(source, i + r, j);

			dest[i][j] = total / (double)(r * 2 + 1);
		}
	}
}

int Turing::sgn(double val) {
	return (0 < val) - (val < 0);
}
