#pragma once
#include <argparse/argparse.hpp>

/**
    parse arguments with different options
    @param parser: argument parser reference
    @param argc: number of arguments
    @param argv: argument list
*/
void parse_arguments(argparse::ArgumentParser& parser, int argc, const char** argv);


enum mode_choices {
    MODE_ENCODE,
    MODE_DECODE,
    MODE_ENCRYPT,
    MODE_DECRYPT
};

mode_choices mode_switch_hash(std::string mode);
