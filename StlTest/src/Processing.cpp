#include "Processing.h"

PointT<float> Processing::GetBendPoint(const std::vector<float>& dataset)
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