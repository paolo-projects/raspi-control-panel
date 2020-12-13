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

PointT<float> Processing::FirstDerivativeMethod(const std::vector<float>& dataset)
{
	std::vector<float> derivatives;
	derivatives.reserve(dataset.size() - 1);

	for (int i = 1; i < dataset.size(); i++)
	{
		derivatives.emplace_back(dataset[i] - dataset[i - 1]);
	}
	auto maxP = std::max_element(derivatives.begin(), derivatives.end());
	unsigned int bendIndex = std::distance(derivatives.begin(), maxP);

	PointT<float> maxPoint = {
		bendIndex,
		dataset[bendIndex]
	};
	return maxPoint;
}

PointT<float> Processing::BaselineAndTopLineMethod(const std::vector<float>& dataset)
{
	// Noise should be reduced to its minimum, through a smoothing algorithm, to avoid picking meaningless data
	auto minmax = std::minmax_element(dataset.begin(), dataset.end());

	float middleP = (*minmax.second - *minmax.first) / 2 + *minmax.first;

	// Supposing the curve is monothonic there should only be 1 x value that 
	// fullfills the requirement, otherwise the first one will be taken
	float bendIndex = 0;
	float distance = std::abs(dataset[0] - middleP);

	for (int i = 0; i < dataset.size(); i++)
	{
		float thisDistance = std::abs(dataset[i] - middleP);
		if (thisDistance < distance) {
			distance = thisDistance;
			bendIndex = i;
		}
	}

	PointT<float> maxPoint = {
		bendIndex,
		dataset[bendIndex]
	};
	return maxPoint;
}
