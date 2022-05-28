// Bonus file, not necessary, but here we go

#pragma once

#include <random>

/**
 * Abstraction for c++ random number generator
 */
class Random {
public:
	/**
	* Default constructor which initializes engine's seed
	*/
	Random() {
		mt.seed(rd());
	}

	/**
	* Default destructor
	*/
	~Random() {}

	/**
	* Generates a random positive number between two numbers
	*
	* @param[in] min left domain
	* @param[in] max right domain
	*
	* @return the random number
	*/
	inline uint32_t GetUInt32(const uint32_t min, const uint32_t max) {
		return std::uniform_int_distribution<std::mt19937::result_type>(min, max)(mt);
	}

private:
	std::random_device rd; ///< device for random numbers
	std::mt19937 mt; ///< the engine for generating real random numbers
};