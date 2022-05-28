#include "JeffersonDisk.hpp"

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