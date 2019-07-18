// MIT License (c) 2019 BYU PCCL see LICENSE file

#pragma once
#include <chrono>
#include <ctime>

/**
 * 
 */
class HOLODECK_API Benchmarker
{
public:
	Benchmarker();
	Benchmarker(float Alpha);
	~Benchmarker();

	/**
	  * Start
	  *
	  * Start before the function you want to benchmark
	  */
	void Start();

	/**
	  * End
	  *
	  * End after the function you want to benchmark
	  */
	void End();

	/**
	  * CalculateAvg
	  *
	  * Calculates the avg time for the function
	  */
	void CalculateAvg();

	/**
	  * Stat
	  *
	  * Returns the average
	  * @return the average
	  */
	FString Stat();

private:
	bool first_pass;
	float alpha;
	float avg;
	std::chrono::duration<float> time_span;
	std::chrono::high_resolution_clock::time_point t_start;
	std::chrono::high_resolution_clock::time_point t_end;
};
