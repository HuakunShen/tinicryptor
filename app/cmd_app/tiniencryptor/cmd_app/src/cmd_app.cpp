#include <iostream>
#include <argparse/argparse.hpp>

#include "util.hpp"
#include "encode.hpp"


using namespace std;
argparse::ArgumentParser parser("Tinicryptor Argument Parser");


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

            break;
        case MODE_ENCRYPT:

            break;
        case MODE_DECRYPT:

            break;
        default:
            break;
        }
    }
    

}

