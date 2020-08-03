#include <argparse/argparse.hpp>
#include "util.hpp"
#include <string>

mode_choices mode_switch_hash(std::string mode) {
	if (mode == "encode") return MODE_ENCODE;
	if (mode == "decode") return MODE_DECODE;
	if (mode == "encrypt") return MODE_ENCRYPT;
	if (mode == "decrypt") return MODE_DECRYPT;
	if (mode == "test") return MODE_TEST;
	return MODE_DNE;
}

void parse_arguments(argparse::ArgumentParser& parser, int argc, const char** argv) {
	parser.add_argument("-i", "--input")
		.help("Input filename/address");
	parser.add_argument("-o", "--output")
		.help("Output filename/address");
	parser.add_argument("-m", "--mode")
		.required()
		.help("Mode of this program: encode/decode/encrypt/decrypt");
	parser.add_argument("-p", "--password")
		.help("Password for encrypt/decrypt");

	try {
		parser.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cout << err.what() << std::endl;
		std::cout << parser;
		exit(0);
	}
}

bool file_exists(const std::string filename) {
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}