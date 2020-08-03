#pragma once

#include <argparse/argparse.hpp>
#include <string>
#define ALL_MODES "1. encode\n" \
                  "2. decode\n" \
                  "3. encrypt\n" \
                  "4. decrypt"


/**
    parse arguments with different options
    @param parser: argument parser reference
    @param argc: number of arguments
    @param argv: argument list
*/
void parse_arguments(argparse::ArgumentParser &parser, int argc, const char **argv);


/**
    Choices of --mode
 */
enum mode_choices {
    MODE_ENCODE,
    MODE_DECODE,
    MODE_ENCRYPT,
    MODE_DECRYPT,
    MODE_TEST,
    MODE_DNE
};


/**
    Take in a string from --mode argument and return the corresponding mode_choices type
    @param mode: --mode argument
*/
mode_choices mode_switch_hash(std::string mode);


bool file_exists(const std::string filename);