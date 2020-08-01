#include <iostream>
#include <argparse/argparse.hpp>

#include "util.hpp"
#include "encode.hpp"
#include "decode.hpp"


using namespace std;
argparse::ArgumentParser parser("Tinicryptor Argument Parser");


void test();

int main(int argc, const char** argv) {
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
            break;
        }
    }
}

void test() {

}