// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "Benchmarker.h"

Benchmarker::Benchmarker()
{
	first_pass = true;
	alpha = 0.5f;
}

Benchmarker::Benchmarker(float Alpha)
{
	first_pass = true;
	alpha = Alpha;
}

Benchmarker::~Benchmarker()
{
}

void Benchmarker::Start()
{
	t_start = std::chrono::high_resolution_clock::now();
}

void Benchmarker::End()
{
	t_end = std::chrono::high_resolution_clock::now();
}

void Benchmarker::CalculateAvg()
{
	time_span = std::chrono::duration_cast<std::chrono::duration<float>>(t_end - t_start);

	if (first_pass)
	{
		avg = (time_span.count());
		first_pass = false;
	}
	else
		avg = (1 - alpha) * avg + (alpha * time_span.count());
}

FString Benchmarker::Stat()
{
	return FString::SanitizeFloat(avg);
}