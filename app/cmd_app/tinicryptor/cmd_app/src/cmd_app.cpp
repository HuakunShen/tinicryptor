#include <iostream>
#include <argparse/argparse.hpp>
#include <bitset>
#include <fstream>


#include "util.hpp"
#include "encode.hpp"
#include "decode.hpp"

// encode arguments
// - m encode - i in.txt - o out.txt

using namespace std;

/**
	Testing
*/
void test();

int main(int argc, const char** argv) {
	argparse::ArgumentParser parser("Tinicryptor Argument Parser");
	parse_arguments(parser, argc, argv);
	if (parser.present("--mode")) {
		string mode = parser.get<string>("--mode");
		const char* mode_c = mode.c_str();
		switch (mode_switch_hash(mode))
		{
		case MODE_ENCODE:
			encode_main(parser);
			break;
		case MODE_DECODE:
			decode_main(parser);
			break;
		case MODE_ENCRYPT:
			exit(ENOSYS);
			break;
		case MODE_DECRYPT:
			exit(ENOSYS);
			break;
		case MODE_TEST:
			test();
			break;
		default:
			cout << "--mode argument \"" + mode + "\" not valid" << endl;
			cout << "Choose from the following modes:" << endl;
			cout << ALL_MODES << endl;
			break;
		}
	}
	else {
		cout << "Error! --mode is required but missing, maybe not detected by argparse" << endl;
	}
}
void bitset_2_array(const std::bitset<256>& bits, int32_t n_set_size, char* buf, int32_t& n_bytes) {

	n_bytes = 0;
	for (int i = 0; i < n_set_size; i += 8) {
		char ch;
		for (int j = 0; j < 8; ++j) {
			if (bits.test(i + j))
				ch |= (1 << j);
			else
				ch &= ~(1 << j);
		}
		buf[n_bytes++] = ch;
	}
}
void test() {
	

}