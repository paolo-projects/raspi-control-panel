#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include "Point.h"

class Processing
{
public:
	static PointT<float> GetBendPoint(const std::vector<float>& dataset);
	/**
	 * @brief Write the dataset to a CSV
	 * @param dataset
	 * @param filename
	*/
	static void WriteToCSV(const std::vector<float>& dataset, const std::string& filename);
	/**
	 * @brief This function takes care of providing a more meaningful data point for plotting on the screen
	 * As of now it just inverts the read value from a blackness-like measure to an intensity-like measure
	 * Since the values are in ADC scale (12-bit, 0-4095 range) we just subtract it from the maximum of the range
	 * @param x The index value
	 * @param y The 12-bit ADC sampled voltage value
	 * @return The transformed data point
	*/
	static float transformDataPoint(int x, float y);
	static PointT<float> FirstDerivativeMethod(const std::vector<float>& dataset);
	static PointT<float> BaselineAndTopLineMethod(const std::vector<float>& dataset);
private:
};
