#include "../huff/huffman_coding.h"

#include <iostream>

void zip(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ofstream out;
    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");

    encode(in, out);


    out.close();
}

void unzip(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ofstream out;

    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");


    decode(in, out);

    in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::runtime_error("Usage: -type(-e/-d) in out");
    }
    std::string type(argv[1]);
    if (type == "-e") {
        zip(std::string(argv[2]), std::string(argv[3]));
    } else if (type == "-d") {
        unzip(std::string(argv[2]), std::string(argv[3]));
    }

    return 0;
}
