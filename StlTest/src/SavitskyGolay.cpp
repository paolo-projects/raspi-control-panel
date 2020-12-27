#include "SavitskyGolay.h"

Matrix<float> SavitskyGolay::JMatrix(int m, int degree)
{
	std::vector<float> zVals;
	for (int z = (1 - m) / 2; z <= (m - 1) / 2; z++)
		zVals.emplace_back(z);

	Matrix<float> jMatrix(zVals.size(), degree + 1);

	for (int r = 0; r < zVals.size(); r++)
	{
		for (int c = 0; c < degree + 1; c++)
		{
			int z = zVals.at(r);
			jMatrix(r, c) = std::pow(z, c);
		}
	}

	return jMatrix;
}

Matrix<float> SavitskyGolay::Coefficients(int m, int degree)
{
	assert(m % 2 > 0); // Odd window size
	assert(degree < m); // Polynomial degree has to be less than window size

	Matrix<float> J = SavitskyGolay::JMatrix(m, degree);
	Matrix<float> Jt = J.transposed();
	Matrix<float> JtJ = Jt * J;
	Matrix<float> JtJi = JtJ.inverse();
	Matrix<float> coeffs = JtJi * Jt;
	return coeffs.row(0);
}

std::vector<float> SavitskyGolay::applyHardcodedFilter(const std::vector<float>& data)
{
	// 2nd order poly - window size: 7
	const int coeffs[] = { -2, 3, 6, 7, 6, 3, -2 };
	const int normalization = 21;


	std::vector<float> result;
	int sidePoints = 3;

	for (int i = 0; i < sidePoints; i++)
		result.emplace_back(0);

	for (int i = sidePoints; i < data.size() - sidePoints; i++)
	{
		float accumulated = 0;
		for (int n = i - sidePoints; n <= i + sidePoints; n++)
		{
			accumulated += data.at(n) * coeffs[n - i + sidePoints];
		}
		accumulated /= normalization;

		result.emplace_back(accumulated);
	}

	for (int i = data.size() - sidePoints; i < data.size(); i++)
		result.emplace_back(0);

	return result;
}

float SavitskyGolay::getSmoothPoint(const std::vector<float>& data, const Matrix<float>& coefficients, int x, int sidePoints)
{
	float accumulated = 0;
	for (int i = x - sidePoints; i <= x + sidePoints; i++)
	{
		accumulated += data.at(i) * coefficients.get(0, i - x + sidePoints);
	}
	return accumulated;
}

std::vector<float> SavitskyGolay::applyFilter(const std::vector<float> data, const Matrix<float>& coefficients)
{
	assert(coefficients.cols() % 2 > 0);
	std::vector<float> result;
	int window = coefficients.cols();
	int sidePoints = (window - 1) / 2;

	for (int i = 0; i < sidePoints; i++)
		result.emplace_back(data.at(i));

	for (int i = sidePoints; i < data.size() - sidePoints; i++)
	{
		result.emplace_back(getSmoothPoint(data, coefficients, i, sidePoints));
	}

	for (int i = data.size() - sidePoints; i < data.size(); i++)
		result.emplace_back(data.at(i));

	return result;
}

