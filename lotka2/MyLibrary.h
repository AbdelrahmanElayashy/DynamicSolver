#ifndef MyLibrary_H
#define MyLibrary_H

#include "EquationContainer.h"
#include <map>
#include <chrono>
#include <algorithm>
#include <iterator>


class MyLibrary
{
public:

	typedef std::chrono::high_resolution_clock::time_point pointTime;
	
	

	/*
	* To start the timer.
	*/
	inline static pointTime startTimer() {
		return std::chrono::high_resolution_clock::now();
	};
	/*
	* To stop the timer.
	*/
	inline static pointTime stopTimer() {
		return std::chrono::high_resolution_clock::now();
	};

	/*
	* To calculate the duration between two point times.
	*/
	inline static double durationTime(pointTime start, pointTime stop) {
		return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	};


	/*
	* To flip the pair.
	*/
	template<typename A, typename B>
	static std::pair<B, A> flip_pair(const std::pair<A, B>& p)
	{
		return std::pair<B, A>(p.second, p.first);
	}

	/*
	 * To flip the map. The keys will be values and values will be keys.
	 */
	template<typename A, typename B>
	static std::multimap<B, A> flip_map(const std::map<A, B>& src)
	{
		std::multimap<B, A> dst;
		std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), flip_pair<A, B>);
		return dst;
	}

	/*
	* To compare between two equation container on the last number step.
	*/
	struct comp {
		bool operator()(const EquationContainer& a, const EquationContainer& b) {
			return a.get_last_number_step() > b.get_last_number_step();
		}
	};

	
		 uint64_t  shuffle_table[4] = { 1,2,3,4 };

		/*
		* Represents the algorithm of Xoroshiro to generate random numbers.
		*/
		inline uint64_t next(void)
		{
			uint64_t s1 = shuffle_table[0];
			uint64_t s0 = shuffle_table[1];
			uint64_t result = s0 + s1;
			shuffle_table[0] = s0;
			s1 ^= s1 << 23;
			shuffle_table[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
			return result;
		}
	


};

#endif