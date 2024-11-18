#include "PercolationStats.h"
#include <cmath>

PercolationStats::PercolationStats() : mean(0.0f), stdDev(0.0f),
				confidenceLo(0.0f), confidenceHi(0.0f) {}

PercolationStats::~PercolationStats() {}


void PercolationStats::addOpenSite(int openSite) {
	openSites.push_back(openSite);
}

void PercolationStats::compute() {
	int numberOfTimes = openSites.size();

	if (numberOfTimes != 0) {

		// Compute for mean
		{
			float total = 0;

			for (int i = 0; i < numberOfTimes; i++) {
				total += openSites.at(i);
			}

			mean = static_cast<float>(total / numberOfTimes);
		}

		// Compute for standard deviation
		{
			stdDev = 0;

			for (int i = 0; i < numberOfTimes; i++) {
				float diff = openSites.at(i) - mean;
				stdDev += diff * diff; // Square the differences
			}
			stdDev /= (numberOfTimes - 1); // Divide by (n-1) for sample standard deviation
			stdDev = std::sqrt(stdDev);   // Take the square root

		}


		// Compute for confidence levels
		{
			float marginOfError = 1.96f * stdDev / std::sqrt(static_cast<float>(numberOfTimes));
			confidenceLo = mean - marginOfError;
			confidenceHi = mean + marginOfError;
		}
	}
}

const int& PercolationStats::getTimes() const {
	return openSites.size();
}

const float& PercolationStats::getMean() const {
	return mean;
}

const float& PercolationStats::getStdDev() const {
	return stdDev;
}

const float& PercolationStats::getConfidenceLo() const {
	return confidenceLo;
}

const float& PercolationStats::getConfidenceHi() const {
	return confidenceHi;
}
