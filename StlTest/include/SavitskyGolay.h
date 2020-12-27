#pragma once
#include <cmath>
#include <Matrix.h>
#include <TouchCP/Point.h>

class SavitskyGolay
{
public:
	static Matrix<float> JMatrix(int m, int degree);
	static Matrix<float> Coefficients(int m, int degree);
	static std::vector<float> applyFilter(const std::vector<float> data, const Matrix<float>& coefficients);
	static float getSmoothPoint(const std::vector<float>& data, const Matrix<float>& coefficients, int x, int sidePoints);
	static std::vector<float> applyHardcodedFilter(const std::vector<float>& data);
};
