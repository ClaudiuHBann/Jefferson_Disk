#include <iostream>
#include <vector>
#include <string>

#include <algorithm>
#include <random>
#include <chrono>

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHABET_LENGTH 26

class Random {
public:
	Random() {
		mt.seed(rd());
	}
	~Random() {}

	inline uint32_t GetUInt32(const uint32_t min, const uint32_t max) {
		return std::uniform_int_distribution<std::mt19937::result_type>(min, max)(mt);
	}

private:
	std::random_device rd;
	std::mt19937 mt;
};

class JeffersonDisk {
public:
	JeffersonDisk(const uint8_t disksCount = 12, const int64_t seed = std::chrono::system_clock::now().time_since_epoch().count()) {
		engineRandom.seed(seed);
		this->seed = seed;

		GenerateDisks(disksCount);
		GenerateKey();
	}

	~JeffersonDisk() {}

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

	inline int64_t GetSeed() const {
		return seed;
	}
	inline std::vector<uint8_t> GetKey() const {
		return key;
	}

	void PrintDisks(const bool withKey = false) {
		for(size_t i = 0; i < ALPHABET_LENGTH; i++) {
			for(size_t j = 0; j < disks.size(); j++) {
				std::cout << disks[(withKey) ? key[j] : j][i] << " ";
			}
			std::cout << std::endl;
		}
	}
	void PrintKey() {
		for(size_t i = 0; i < disks.size(); i++) {
			std::cout << (int)key[i] << " ";
		}
		std::cout << std::endl;
	}
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
	bool ValidateMessage(const std::string& message) const {
		for(size_t i = 0; i < message.length(); i++) {
			if(message[i] < ALPHABET[0] ||
			   message[i] > ALPHABET[ALPHABET_LENGTH - 1]) {
				return false;
			}
		}

		return true;
	}

	void GenerateDisks(const uint8_t disksCount = 12) {
		for(size_t i = 0; i < disksCount; i++) {
			std::string disk(ALPHABET);
			std::shuffle(disk.begin(), disk.end(), engineRandom);
			disks.push_back(disk);
		}
	}
	void GenerateKey() {
		for(uint8_t i = 0; i < disks.size(); i++) {
			key.push_back(i);
		}
		std::shuffle(key.begin(), key.end(), engineRandom);
	}
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

	std::string GetDisksRow(std::vector<std::string>& disks, uint8_t row) {
		// Pe prima linie se va forma mesajul deci nu o poate lua ca si mesaj criptat
		// si nu poate lua un numar mai mare decat alfabet pentru ca nu exista
		if(row == 1 || --row > ALPHABET_LENGTH) {
			throw std::invalid_argument("'row' must be between 1 and " ALPHABET);
		}

		std::string diskRow(disks.size(), 0);
		for(size_t i = 0; i < disks.size(); i++) {
			diskRow[i] = disks[i][row];
		}

		return diskRow;
	}
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

	int64_t seed = 0;
	std::mt19937 engineRandom;
	Random random;

	std::vector<std::string> disks;
	std::vector<uint8_t> key;
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