#pragma once
#include <vector>
#include <cassert>


/**
 * @brief A matrix of type T with R rows and C columns
 * @tparam T The data type
*/
template<typename T>
class Matrix
{
public:
	Matrix(int rows, int columns) : R(rows), C(columns)
	{
		values.resize(R * C);
	}
	Matrix(int rows, int columns, const std::vector<T>& arr)
		: values(arr), R(rows), C(columns)
	{
	}
	Matrix(const Matrix<T>& copy)
		: values(copy.values), R(copy.R), C(copy.C)
	{}
	// Getters, setters
	T get(int row, int column) const
	{
		return values[column + row * C];
	}
	void set(int row, int column, T value)
	{
		values[column + row * C];
	}
	T& at(int row, int column)
	{
		return values.at(column + row * C);
	}
	// Operators
	T& operator()(int row, int column)
	{
		return values.at(row * C + column);
	}
	Matrix<T> operator*(T scalar) const
	{
		Matrix<T> result(*this);
		for (int i = 0; i < result.values.size(); i++)
			result.values[i] *= scalar;
		return result;
	}
	Matrix<T> operator*(const Matrix<T>& rhs) const
	{
		assert(C == rhs.R);
		Matrix<T> result(R, rhs.C);
		for (int i = 0; i < result.R; i++)
		{
			for (int j = 0; j < result.C; j++)
			{
				result(i, j) = 0;
				for (int k = 0; k < C; k++)
					result(i, j) += get(i, k) * rhs.get(k, j);
			}
		}
		return result;
	}
	// Operations
	Matrix<T> transposed() const
	{
		Matrix<T> transp(C, R);
		for (int i = 0; i < R; i++)
		{
			for (int j = 0; j < C; j++)
			{
				transp(j, i) = get(i, j);
			}
		}
		return transp;
	}
	Matrix<T> inverse() const
	{
		assert(R == C);
		T det = determinant();
		Matrix<T> cofatt = cofactors();
		Matrix<T> transp = cofatt.transposed();
		Matrix<T> inverse = transp * (1 / det);
		return inverse;
	}
	Matrix<T> inverseFactorized() const
	{
		assert(R == C);
		T det = determinant();
		Matrix<T> cofatt = cofactors();
		Matrix<T> transp = cofatt.transposed();
		return transp;
	}
	int rows() const
	{
		return R;
	}
	int cols() const
	{
		return C;
	}
	Matrix<T> row(int r) const
	{
		Matrix<T> res(1, C);
		for (int j = 0; j < C; j++)
		{
			res(0, j) = get(r, j);
		}
		return res;
	}
	Matrix<T> column(int c) const
	{
		Matrix<T> res(R, 1);
		for (int i = 0; i < R; i++)
		{
			res(i, 0) = get(i, c);
		}
		return res;
	}
	T determinant() const
	{
		assert(R == C);
		if (R == 1)
		{
			return values[0];
		}
		else if (R == 2)
		{
			return get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0);
		}
		else if (R > 2)
		{
			T determinant = 0;
			for (int j = 0; j < R; j++)
			{
				T pickVal = get(0, j); // Value for determinant picked from first row
				Matrix<T> subMatrix(R - 1, R - 1);
				for (int i1 = 0; i1 < R; i1++)
				{
					for (int j1 = 0; j1 < R; j1++)
					{
						if (i1 > 0)
						{
							if (j1 < j)
							{
								subMatrix(i1 - 1, j1) = get(i1, j1);
							}
							else if (j1 > j)
							{
								subMatrix(i1 - 1, j1 - 1) = get(i1, j1);
							}
						}
					}
				}
				determinant += ((j % 2 > 0) ? -1 : 1) * pickVal * subMatrix.determinant();
			}
			return determinant;
		}
		else return 0;
	}
	Matrix<T> cofactors() const
	{
		assert(R == C);
		Matrix<T> cofMatrix(R, R);
		for (int i = 0; i < R; i++)
		{
			for (int j = 0; j < R; j++)
			{
				Matrix<T> subMatrix(R - 1, R - 1);
				for (int i1 = 0; i1 < R; i1++)
				{
					for (int j1 = 0; j1 < R; j1++)
					{
						if (i1 < i)
						{
							if (j1 < j)
							{
								subMatrix(i1, j1) = get(i1, j1);
							}
							else if (j1 > j)
							{
								subMatrix(i1, j1 - 1) = get(i1, j1);
							}
						}
						else if (i1 > i)
						{
							if (j1 < j)
							{
								subMatrix(i1 - 1, j1) = get(i1, j1);
							}
							else if (j1 > j)
							{
								subMatrix(i1 - 1, j1 - 1) = get(i1, j1);
							}
						}
					}
				}
				int flatIndex = i * R + j;
				T dett = ((flatIndex % 2) > 0 ? -1 : 1) * subMatrix.determinant();
				cofMatrix(i, j) = dett;
			}
		}
		return cofMatrix;
	}
	Matrix<T> columnReplaced(const Matrix<T> columnVector, int column) const
	{
		Matrix<T> ret(*this);
		for (int r = 0; r < rows(); r++)
		{
			ret(r, column) = columnVector(r, 0);
		}
		return ret;
	}
	Matrix<T> columnReplaced(const std::vector<T>& columnV, int column) const
	{
		Matrix<T> ret(*this);
		for (int r = 0; r < rows(); r++)
		{
			ret(r, column) = columnV.at(r);
		}
		return ret;
	}
	Matrix<T> rowReplaced(const Matrix<T> rowVector, int row) const
	{
		Matrix<T> ret(*this);
		for (int c = 0; c < cols(); c++)
		{
			ret(row, c) = rowVector(0, c);
		}
		return ret;
	}
	Matrix<T> rowReplaced(const std::vector<T>& rowV, int row) const
	{
		Matrix<T> ret(*this);
		for (int c = 0; c < cols(); c++)
		{
			ret(row, c) = rowV.at(c);
		}
		return ret;
	}

private:
	const int R, C;
	std::vector<T> values;
};