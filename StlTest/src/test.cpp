#ifdef TEST_ENV
#include <stdio.h>
#include "SavitskyGolay.h"

int main(int argc, char** argv)
{
	Matrix<float> coeffs = SavitskyGolay::Coefficients(5, 3);
	printf("Coeffs matrix:\n");
	for (int i = 0; i < coeffs.rows(); i++)
	{
		for (int j = 0; j < coeffs.cols(); j++)
			printf("%.3f\t", coeffs(i, j));

		printf("\n");
	}
	getc(stdin);
}
#endif