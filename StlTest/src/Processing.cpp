#include "Processing.h"

PointT<float> Processing::GetBendPoint(const std::vector<float>& dataset)
{
	// Supposing the shadow edge is displayed as a sigmoid curve on the CCD sensor
	// we can select the bend point of the sigmoid by taking the maximum of its
	// first derivative
	// This method should be fairly accurate, but the curve should be smooth and clean
	// for it to work

	// Another method would be to take the baseline y value (corresponding to the shadow
	// zone) and the top line y value (corresponding to the light zone), and picking the x
	// value corresponding to the point in the middle of these two lines. 
	// e.g. yM = (yT - yB) / 2 + yB
	return { 0, 0 };
}

void Processing::WriteToCSV(const std::vector<float>& dataset, const std::string& filename)
{
	FILE* outf = fopen(filename.c_str(), "w");
	fprintf(outf, "Pixel,Intensity (ADC scale)\r\n");
	for (int i = 0; i < dataset.size(); i++)
	{
		fprintf(outf, "%d,%.0f\r\n", i, dataset[i]);
	}
	fclose(outf);
}

float Processing::transformDataPoint(int x, float y)
{
	return 4096 - y;
}

PointT<float> Processing::FirstDerivativeMethod(const XYDataset& dataset)
{
	std::vector<float> derivatives, indexes;
	indexes.reserve(dataset.second.size() - 1);
	derivatives.reserve(dataset.second.size() - 1);

	for (int i = 1; i < dataset.first.size(); i++)
	{
		if (dataset.second[i] != 0 && dataset.second[i - 1] != 0)
		{
			derivatives.emplace_back(dataset.second[i] - dataset.second[i - 1]);
			indexes.emplace_back(dataset.first[i]);
		}
	}
	auto maxP = std::max_element(derivatives.begin(), derivatives.end());
	unsigned int bendIndex = std::distance(derivatives.begin(), maxP);

	PointT<float> maxPoint = {
		indexes.at(bendIndex),
		dataset.second[indexes.at(bendIndex)]
	};
	return maxPoint;
}

PointT<float> Processing::BaselineAndTopLineMethod(const XYDataset& dataset)
{
	// Noise should be reduced to its minimum, through a smoothing algorithm, to avoid picking meaningless data
	auto minmax = std::minmax_element(dataset.second.begin(), dataset.second.end());

	float middleP = (*minmax.second - *minmax.first) / 2 + *minmax.first;

	// Supposing the curve is monothonic there should only be 1 x value that 
	// fullfills the requirement, otherwise the first one will be taken
	float bendIndex = 0;
	float distance = std::abs(dataset.second[0] - middleP);

	for (int i = 0; i < dataset.second.size(); i++)
	{
		float thisDistance = std::abs(dataset.second[i] - middleP);
		if (thisDistance < distance) {
			distance = thisDistance;
			bendIndex = i;
		}
	}

	PointT<float> maxPoint = {
		dataset.first.at(bendIndex),
		dataset.second.at(dataset.first.at(bendIndex))
	};
	return maxPoint;
}

PointT<float> Processing::RegressionBasedMethod(const XYDataset& dataset, float DERIV_TRESHOLD)
{
	/*
	* This method works by fitting the top line of the curve
	*
	*  - - - -------------- <== this one
	*       /
	*      /
	* ----/
	*
	* and the shadow edge line
	*          /
	*         /
	*        -----------
	*       /
	*      / <== this one
	* ----/
	*    /
	*   /
	* and computing the intersection between the two,
	* which will be taken as the extracted point
	*
	*    ==> X-----------
	*       /
	*      /
	* ----/
	*
	*/

	/*
	* First step is to extract the points belonging to the top line
	* the regression can tolerate some noise. It can even tolerate some
	* points belonging to the bend, but it will increase the error on the result
	*/

	/*
	* We start by detecting the tone of the curve, i.e. if it's ascending or descending
	* or in other words, if the shadow strikes one side or the other of the CCD
	*/
	auto derivs = Derivatives(dataset.second);
	auto minMax = std::minmax_element(derivs.begin(), derivs.end());

	bool ascending = std::abs(*minMax.second) > std::abs(*minMax.first);

	/*
	* If the curve is ascending we take the last N points,
	* otherwise we take the first N points
	*
	* We get the amount N of points to take by looking at the derivatives,
	* when they get over a certain treshold we pick the point
	*/
	int N;
	XYDataset toplinedata; \
		if (ascending)
		{
			N = derivs.size();
			for (int i = derivs.size() - 1; i >= 0; i--)
			{
				if (derivs.at(i) > DERIV_TRESHOLD)
				{
					N = i;
					break;
				}
				toplinedata.first.insert(toplinedata.first.begin(), i);
				toplinedata.second.insert(toplinedata.second.begin(), dataset.second.at(i));
			}
		}
		else
		{
			N = 0;
			for (int i = 0; i < derivs.size(); i++)
			{
				if (derivs.at(i) < -DERIV_TRESHOLD)
				{
					N = i - 1;
					break;
				}
				toplinedata.first.emplace_back(i - 1);
				toplinedata.second.emplace_back(dataset.second.at(i - 1));
			}
		}

	PolyFit<float> topLine(toplinedata.first, toplinedata.second, 1);

	XYDataset vertlinedata;
	if (ascending)
	{
		for (int i = N - 1; i >= 0; i--)
		{
			if (derivs.at(i) < DERIV_TRESHOLD)
			{
				break;
			}
			vertlinedata.first.insert(vertlinedata.first.begin(), i);
			vertlinedata.second.insert(vertlinedata.second.begin(), dataset.second.at(i));
		}
	}
	else
	{
		for (int i = N; i < derivs.size(); i++)
		{
			if (derivs.at(i) > -DERIV_TRESHOLD)
			{
				break;
			}
			vertlinedata.first.emplace_back(i - 1);
			vertlinedata.second.emplace_back(dataset.second.at(i - 1));
		}
	}

	PolyFit<float> vertLine(vertlinedata.first, vertlinedata.second, 1);

	/*
	* Intersection
	*     C0' - C0
	* x = --------
	*     C1 - C1'
	*/
	float x = (vertLine.getCoefficients().at(0) - topLine.getCoefficients().at(0)) / (topLine.getCoefficients().at(1) - vertLine.getCoefficients().at(1));
	float y = vertLine.estimate(x);

	return PointT<float>{x, y};
}

std::pair<std::vector<float>, std::vector<float>> Processing::DerivativeBasedFilter(const std::pair<std::vector<float>, std::vector<float>>& dataset)
{
	std::pair<std::vector<float>, std::vector<float>> result;
	result.first.reserve(dataset.second.size());
	result.second.reserve(dataset.second.size());

	std::vector<float> derivatives, indexes;
	indexes.reserve(dataset.second.size() - 1);
	derivatives.reserve(dataset.second.size() - 1);

	for (int i = 1; i < dataset.first.size(); i++)
	{
		if (dataset.second[i] != 0 && dataset.second[i - 1] != 0)
		{
			derivatives.emplace_back(dataset.second[i] - dataset.second[i - 1]);
			indexes.emplace_back(dataset.first[i]);
		}
	}

	for (int i = 1; i < derivatives.size(); i++)
	{
		if (derivatives.at(i - 1) < 30)
		{
			result.first.emplace_back(indexes.at(i - 1));
			result.second.emplace_back(dataset.second.at(indexes.at(i - 1)));
		}
	}
	return result;
}

std::vector<float> Processing::Derivatives(const std::vector<float>& data)
{
	std::vector<float> derivatives;
	for (int i = 1; i < data.size(); i++)
	{
		derivatives.emplace_back(data.at(i) - data.at(i - 1));
	}
	return derivatives;
}
