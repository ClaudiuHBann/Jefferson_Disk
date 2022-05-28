#include <iostream>
#include <vector>
#include <string>

#include <algorithm>
#include <random>
#include <chrono>

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHABET_LENGTH 26

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
	JeffersonDisk(const uint8_t disksCount = 12, const int64_t seed = std::chrono::system_clock::now().time_since_epoch().count()) {
		engineRandom.seed(seed);
		this->seed = seed;

		GenerateDisks(disksCount);
		GenerateKey();
	}

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
	void Decrypt(std::vector<uint8_t>& key, const std::string& message) {
		if(!ValidateMessage(message)) {
			throw std::invalid_argument("'message' must contain only " ALPHABET);
		}

		std::vector<uint8_t> disksOffsets;
		GenerateOffsets(disksOffsets, message);

		for(size_t i = 0; i < ALPHABET_LENGTH; i++) {
			for(size_t j = 0; j < disks.size(); j++) {
				std::cout << disks[key[j]][disksOffsets[j]++] << " ";

				if(disksOffsets[j] == ALPHABET_LENGTH) {
					disksOffsets[j] = 0;
				}
			}
			std::cout << std::endl;
		}
	}

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
	void PrintDisks(const bool withKey = false) {
		for(size_t i = 0; i < ALPHABET_LENGTH; i++) {
			for(size_t j = 0; j < disks.size(); j++) {
				std::cout << disks[(withKey) ? key[j] : j][i] << " ";
			}
			std::cout << std::endl;
		}
	}

	/**
	* Prints the jeff disk key
	*/
	void PrintKey() {
		for(size_t i = 0; i < disks.size(); i++) {
			std::cout << (int)key[i] << " ";
		}
		std::cout << std::endl;
	}

	/**
	* Prints all the information that holds
	*/
	void PrintAll() {
		std::cout << "Seed: " << seed << std::endl << std::endl;

		std::cout << "Disks: " << std::endl;
		PrintDisks();
		std::cout << std::endl;

		std::cout << "Key: ";
		PrintKey();
		std::cout << std::endl;

		std::cout << "Disks with key: " << std::endl;
		PrintDisks(true);
		std::cout << std::endl;
	}

private:
	/**
	* Checks if a message can be decrypted or encrypted
	*
	* @param[in] message to be validated
	*
	* @return true if ok else flase
	*/
	bool ValidateMessage(const std::string& message) const {
		for(size_t i = 0; i < message.length(); i++) {
			if(message[i] < ALPHABET[0] ||
			   message[i] > ALPHABET[ALPHABET_LENGTH - 1]) {
				return false;
			}
		}

		return true;
	}

	/**
	* Generates random disks
	*
	* @param[in] disksCount how many disks to be generated
	*/
	void GenerateDisks(const uint8_t disksCount = 12) {
		for(size_t i = 0; i < disksCount; i++) {
			std::string disk(ALPHABET);
			std::shuffle(disk.begin(), disk.end(), engineRandom);
			disks.push_back(disk);
		}
	}

	/**
	* Generates a random key
	*/
	void GenerateKey() {
		for(uint8_t i = 0; i < disks.size(); i++) {
			key.push_back(i);
		}
		std::shuffle(key.begin(), key.end(), engineRandom);
	}

	/**
	* Initializes a vector to be filled with the disks offsets for the message
	*
	* @param[out] offsets the disks offsets when looking for the message
	* @param[in] message to be looked for
	*/
	void GenerateOffsets(std::vector<uint8_t>& offsets, const std::string& message) {
		if(!ValidateMessage(message)) {
			throw std::invalid_argument("'message' must contain only " ALPHABET);
		}

		for(size_t i = 0; i < key.size(); i++) {
			uint8_t moves = 0;
			if(i < message.length()) {
				moves = (uint8_t)disks[key[i]].find(message[i]);
			}

			offsets.push_back(moves);
		}
	}

	/**
	* Returns the disks row
	*
	* @param[in] disks to be looked for
	* @param[in] row to be taken
	*
	* @return the row of the jeff disk
	*/
	std::string GetDisksRow(std::vector<std::string>& disks, uint8_t row) {
		if(row == 1 || --row > ALPHABET_LENGTH) {
			throw std::invalid_argument("'row' must be between 1 and " ALPHABET);
		}

		std::string diskRow(disks.size(), 0);
		for(size_t i = 0; i < disks.size(); i++) {
			diskRow[i] = disks[i][row];
		}

		return diskRow;
	}

	/**
	* Creates the final jeff disks (with the key and message offsets)
	*
	* @param[out] disksNew the final disks
	* @param[in] disksOffsets the disk message offsets
	*/
	void GetDisksFinal(std::vector<std::string>& disksNew, std::vector<uint8_t> disksOffsets) {
		disksNew.resize(disks.size(), ALPHABET);

		for(size_t i = 0; i < ALPHABET_LENGTH; i++) {
			for(size_t j = 0; j < disks.size(); j++) {
				disksNew[j][i] = disks[key[j]][disksOffsets[j]++];

				if(disksOffsets[j] == ALPHABET_LENGTH) {
					disksOffsets[j] = 0;
				}
			}
		}
	}

	int64_t seed = 0; ///< the seed of the engine
	std::mt19937 engineRandom; ///< the engine to shuffle things up and more

	std::vector<std::string> disks; ///< the jeff disks
	std::vector<uint8_t> key; ///< the key for the disks
};

int main() {
	JeffersonDisk jd;

	jd.PrintAll();

	std::string message("HEIILHITTLER");
	std::cout << "Message: " << message << std::endl << std::endl;

	auto key = jd.GetKey();
	auto messageEncrypted = jd.Encrypt(message, 14);
	std::cout << "Encrypted message: " << messageEncrypted << std::endl << std::endl;

	std::cout << "Disks with key and offsets:" << std::endl;
	jd.Decrypt(key, messageEncrypted);

	return 0;
}