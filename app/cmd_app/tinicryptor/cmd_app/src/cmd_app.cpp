#include <iostream>
#include <argparse/argparse.hpp>

#include "util.hpp"
#include "encode.hpp"
#include "decode.hpp"


using namespace std;



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

void test() {

}