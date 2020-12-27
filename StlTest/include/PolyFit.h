#pragma once
#include <vector>
#include "Matrix.h"

template<typename T>
class PolyFit
{
public:
	// Custom exception
	class FitException : public std::exception {
	public:
		FitException(std::string msg) : msg(msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		std::string msg;
	};

	/**
	 * @brief Fit goodness statistical parameters
	*/
	template<typename E>
	struct Parameters {
		double RSquared;
		std::vector<E> residuals;
	};

	/**
	 * @brief Constructor
	 * @param X The X data
	 * @param Y The Y data
	 * @param order The order of the polynomial
	*/
	PolyFit<T>(const std::vector<T>& X, const std::vector<T>& Y, int order) : X(X), Y(Y), order(order) {
		if (X.size() < 2 || Y.size() < 2)
			throw new FitException("X and Y vectors must contain at least 2 elements");
		if (X.size() != Y.size())
			throw new FitException("X and Y vectors must be of same size");
		doFit();
	}

	/**
	 * @brief Get the coefficients
	 * @return The coefficients. The index corresponds to the order of the variable
	*/
	const std::vector<double> getCoefficients() const { return coefficients; }

	/**
	 * @brief Get the statistical parameters
	 * @return The parameters
	*/
	const Parameters<T> getParameters() const { return params; }


	const T estimate(T x) const {
		T out = 0.;
		for (int i = 0; i < coefficients.size(); i++)
			out += pow(x, i) * coefficients.at(i);
		return out;
	}
private:
	int order;
	std::vector<T> X, Y;
	std::vector<double> coefficients;
	Parameters<T> params;

	double getMean(std::vector<T> values) {
		double t = 0.;
		for (T v : values)
			t += v;
		return t / values.size();
	}

	void doFit() {
		coefficients.resize(order + 1);
		Matrix<double> A(order + 1, order + 1);
		std::vector<double> B;
		B.resize(order + 1);

		for (int i = 0; i < X.size(); i++) {
			for (int m = 0; m <= order; m++) {
				// Build A matrix
				for (int n = 0; n <= order; n++) {
					T x = X.at(i);
					A(m, n) += pow(x, m) * pow(x, n);
				}
				// Build B column vector
				B.at(m) += pow(X.at(i), m) * Y.at(i);
			}
		}
		double Adet = A.determinant();
		// Build replaced matrices and fill coefficients vector with the result given by Cramer method
		// Which is Ci = det(Ai)/det(A)
		for (int i = 0; i <= order; i++) {
			Matrix<double> Ai = A.columnReplaced(B, i);
			double Aidet = Ai.determinant();
			coefficients.at(i) = Aidet / Adet;
		}

		double yMean = getMean(Y);
		double ESS = 0.;
		double TSS = 0.;
		for (int i = 0; i < Y.size(); i++) {
			T yEstim = estimate(X.at(i)); // iastim cchiù ca ato
			ESS += pow(yEstim - yMean, 2);
			TSS += pow(Y.at(i) - yMean, 2);
			params.residuals.emplace_back(yEstim - Y.at(i));
		}

		params.RSquared = ESS / TSS;

	}
};