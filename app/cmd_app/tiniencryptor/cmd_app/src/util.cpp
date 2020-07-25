#include "util.hpp"
#include <argparse/argparse.hpp>


void parse_arguments(argparse::ArgumentParser& parser, int argc, const char** argv) {
    parser.add_argument("-i", "--input")
        .help("Input filename/address");
    parser.add_argument("-o", "--output")
        .help("Output filename/address");
    parser.add_argument("-m", "--mode")
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