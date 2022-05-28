#include "JeffersonDisk.hpp"

JeffersonDisk::JeffersonDisk(const uint8_t disksCount /*= 12*/, const int64_t seed /*= std::chrono::system_clock::now().time_since_epoch().count()*/):
	seed(seed) {
	engineRandom.seed((uint32_t)seed);

	GenerateDisks(disksCount);
	GenerateKey();
}

void JeffersonDisk::Decrypt(std::vector<uint8_t>& key, const std::string& message) {
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

void JeffersonDisk::PrintDisks(const bool withKey /*= false*/) {
	for(size_t i = 0; i < ALPHABET_LENGTH; i++) {
		for(size_t j = 0; j < disks.size(); j++) {
			std::cout << disks[(withKey) ? key[j] : j][i] << " ";
		}
		std::cout << std::endl;
	}
}

void JeffersonDisk::PrintKey() {
	for(size_t i = 0; i < disks.size(); i++) {
		std::cout << (int)key[i] << " ";
	}
	std::cout << std::endl;
}

void JeffersonDisk::PrintAll() {
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

bool JeffersonDisk::ValidateMessage(const std::string& message) const {
	for(size_t i = 0; i < message.length(); i++) {
		if(message[i] < ALPHABET[0] ||
		   message[i] > ALPHABET[ALPHABET_LENGTH - 1]) {
			return false;
		}
	}

	return true;
}

void JeffersonDisk::GenerateDisks(const uint8_t disksCount /*= 12*/) {
	for(size_t i = 0; i < disksCount; i++) {
		std::string disk(ALPHABET);
		std::shuffle(disk.begin(), disk.end(), engineRandom);
		disks.push_back(disk);
	}
}

void JeffersonDisk::GenerateKey() {
	for(uint8_t i = 0; i < disks.size(); i++) {
		key.push_back(i);
	}
	std::shuffle(key.begin(), key.end(), engineRandom);
}

void JeffersonDisk::GenerateOffsets(std::vector<uint8_t>& offsets, const std::string& message) {
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

std::string JeffersonDisk::GetDisksRow(std::vector<std::string>& disks, uint8_t row) {
	if(row == 1 || --row > ALPHABET_LENGTH) {
		throw std::invalid_argument("'row' must be between 1 and " ALPHABET);
	}

	std::string diskRow(disks.size(), 0);
	for(size_t i = 0; i < disks.size(); i++) {
		diskRow[i] = disks[i][row];
	}

	return diskRow;
}

void JeffersonDisk::GetDisksFinal(std::vector<std::string>& disksNew, std::vector<uint8_t> disksOffsets) {
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