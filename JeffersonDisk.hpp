#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <algorithm>
#include <random>
#include <chrono>

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHABET_LENGTH 26

/**
 * Implementation of Jefferson Disk
 */
class JeffersonDisk {
public:
	/**
	* Default constructor which creates the jeff disk
	*
	* @param[in] disksCount the number of disks from the jeff disk
	* @param[in] seed for generating random numbers (default as the 1970 timestamp in seconds)
	*/
	JeffersonDisk(const uint8_t disksCount = 12, const int64_t seed = std::chrono::system_clock::now().time_since_epoch().count());

	/**
	* Default destructor
	*/
	~JeffersonDisk() {}

	/**
	* Encrypts the given message and returns the encrypted message from the specified row
	*
	* @param[in] message to be encrypted
	* @param[in] row to take the encrypted message from
	*
	* @return the encrypted message
	*/
	inline std::string Encrypt(const std::string& message, const uint8_t row = 2) {
		if(!ValidateMessage(message)) {
			throw std::invalid_argument("'message' must contain only " ALPHABET);
		}

		std::vector<uint8_t> disksOffsets;
		GenerateOffsets(disksOffsets, message);

		std::vector<std::string> disksNew;
		GetDisksFinal(disksNew, disksOffsets);
		return GetDisksRow(disksNew, row);
	}

	/**
	* Decrypts the given message with the specified key and prints disks
	*
	* @param[in] key for decryption
	* @param[in] message to be decrypted
	*/
	void Decrypt(std::vector<uint8_t>& key, const std::string& message);

	/**
	* Returns the given seed
	*
	* @return the seed
	*/
	inline int64_t GetSeed() const {
		return seed;
	}

	/**
	* Returns the randomly generated key
	*
	* @return the key
	*/
	inline std::vector<uint8_t> GetKey() const {
		return key;
	}

	/**
	* Prints the jeff disks with(out) the key applied
	*
	* @param[in] withKey prints with the key applied
	*/
	void PrintDisks(const bool withKey = false);

	/**
	* Prints the jeff disk key
	*/
	void PrintKey();

	/**
	* Prints all the information that holds
	*/
	void PrintAll();

private:
	/**
	* Checks if a message can be decrypted or encrypted
	*
	* @param[in] message to be validated
	*
	* @return true if ok else flase
	*/
	bool ValidateMessage(const std::string& message) const;

	/**
	* Generates random disks
	*
	* @param[in] disksCount how many disks to be generated
	*/
	void GenerateDisks(const uint8_t disksCount = 12);

	/**
	* Generates a random key
	*/
	void GenerateKey();

	/**
	* Initializes a vector to be filled with the disks offsets for the message
	*
	* @param[out] offsets the disks offsets when looking for the message
	* @param[in] message to be looked for
	*/
	void GenerateOffsets(std::vector<uint8_t>& offsets, const std::string& message);

	/**
	* Returns the disks row
	*
	* @param[in] disks to be looked for
	* @param[in] row to be taken
	*
	* @return the row of the jeff disk
	*/
	std::string GetDisksRow(std::vector<std::string>& disks, uint8_t row);

	/**
	* Creates the final jeff disks (with the key and message offsets)
	*
	* @param[out] disksNew the final disks
	* @param[in] disksOffsets the disk message offsets
	*/
	void GetDisksFinal(std::vector<std::string>& disksNew, std::vector<uint8_t> disksOffsets);

	int64_t seed = 0; ///< the seed of the engine
	std::mt19937 engineRandom; ///< the engine to shuffle things up and more

	std::vector<std::string> disks; ///< the jeff disks
	std::vector<uint8_t> key; ///< the key for the disks
};