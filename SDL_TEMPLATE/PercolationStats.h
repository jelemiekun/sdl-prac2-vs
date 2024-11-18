#pragma once
#include <vector>

class PercolationStats {
private:
	std::vector<int> openSites;

	float mean;
	float stdDev;
	float confidenceLo;
	float confidenceHi;

public:
	PercolationStats();
	~PercolationStats();

	void addOpenSite(int openSite);
	void compute();

	const int& getTimes() const;
	const float& getMean() const;
	const float& getStdDev() const;
	const float& getConfidenceLo() const;
	const float& getConfidenceHi() const;
};

