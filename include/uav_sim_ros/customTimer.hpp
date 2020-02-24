#ifndef _CUSTOM_TIMER_H_
#define _CUSTOM_TIMER_H_

#include <iostream>
#include <numeric>
#include <iterator>
#include <chrono>

class CyberTimer
{
public:
	CyberTimer():
	CyberTimer(1)
	{};

	CyberTimer(const uint32_t averageLen):
	averageLen_(averageLen<1?1:averageLen),
	i_(0),
	iterCount_(0),
	running_(false),
	dt_(0.0)
	{
		buffer_ = new double[averageLen];
	};

	~CyberTimer()
	{
		delete[] buffer_;
	};

	void tic(void)
	{
		running_ = true;
		t_start_ = std::chrono::high_resolution_clock::now();
	};

	double toc(void)
	{
		if(running_)
		{
			running_ = false;
			const auto t_stop = std::chrono::high_resolution_clock::now();
			const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t_stop - t_start_);
			dt_ = static_cast<double>(duration.count())*1.0e-9;
			
			buffer_[i_] = dt_;
			i_++;
			if(i_>=averageLen_)
				i_=0;
			iterCount_++;
		}

		return dt_;
	};

	double getAverage(void) const
	{
		if(iterCount_==0)
			return -1.0;

		if(iterCount_<averageLen_)
			return std::accumulate(buffer_, buffer_+iterCount_, 0.0)/iterCount_;

		return std::accumulate(buffer_, buffer_+averageLen_, 0.0)/averageLen_;
	};

	double getAverageCount(void) const
	{
		if(iterCount_<averageLen_)
			return iterCount_;
		else
			return averageLen_;
	};

	bool isRunnning(void) const
	{
		return running_;
	};

	double getLatest(void) const
	{
		if(iterCount_==0)
			return -1.0;
		else
			return dt_;
	};

private:
	const uint32_t averageLen_;
	uint32_t i_;
	uint32_t iterCount_;
	bool running_;
	double dt_;
	std::chrono::time_point<std::chrono::high_resolution_clock> t_start_;
	double *buffer_;
};


#endif