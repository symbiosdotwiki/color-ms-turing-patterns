#ifndef TURING_H
#define TURING_H

#include <random>
#include <ctime>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

class Turing
{
public:
	Turing(bool sym);
	Turing(bool sym, int type, int w, int h);
	Turing(sf::Image img, int rgbC, bool sym);
	Turing(sf::Image img, bool sym);
	Turing(std::vector<int> acts, std::vector<int> inhibs, std::vector<double> smalls,
		std::vector<int> w, std::vector<int> syms, const int resW, const int resH);

	~Turing() {
		free(activators);
		free(inhibitors);
		free(weights);
		free(symmetries);
		free(smallAmounts);
		free(pattern);
		free(activatorM);
		free(inhibitorM);
		free(variationM);
		free(aiDiffM);
		free(dummy);
	}

	void iterate();
	double *** GetArray();
	double GetPixel(int c, int x, int y);
	void MatrixSubtract(double **pos, double **neg, double **dest, int x, int y);

private:
	static const int scales, varrad, blurnum;
	static std::vector<int> aDef, iDef, wDef, sDef;
	static std::vector<double>  saDef;
	static const double cIntDef[3];

	bool symm, color;
	int *activators, *inhibitors, *weights, *symmetries, resWidth, resHeight,
		colors, rgb, addType;
	double *smallAmounts, *colorInt;
	double ***pattern, ****activatorM, ****inhibitorM, ****variationM, ****aiDiffM, ***dummy;

	void InitTuring(const std::vector<int> acts, const std::vector<int> inhibs, const std::vector<double> smalls,
		const std::vector<int> w, const std::vector<int> syms,
		const double cInts[3], const int resW, const int resH);
	void InitRand(int resW, int resH);
	void InitImg(int resW, int resH, sf::Image img, int rgbC);
	void InitImg(int resW, int resH, sf::Image img);

	void Activator(), Activator(int sn, int cn);
	void RActivator(), RActivator(int sn, int cn);
	void Inhibitor(), Inhibitor(int sn, int cn);
	void RInhibitor(), RInhibitor(int sn, int cn);
	void Variation(), Variation(int sn, int cn);
	void AIDiff(), AIDiff(int sn, int cn);
	void UpdatePixels();
	void Normalize(), Normalize(int c);

	void minElements(double *list, int listSize, int *minElems, int numElems);

	double CalcSA(int color, int x, int y);
	int sgn(double val);

	void SetA(int s, int c);
	void SetI(int s, int c);
	void SetV(int s, int c);
	void RAvgM(double **list, int s);

	double GetMVal(double** mat, int x, int y);


	void AvgH(int r, double **source, double **dest);
	void AvgV(int r, double **source, double **dest);
};

#endif
