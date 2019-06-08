#include "../huff/huffman_coding.h"

#include <iostream>

void encode(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ofstream out;
    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");

    tree current;

    current.init(in);
    current.write_tree(out);

    //std::cout<<"tree writed"<<"\n";

    in.close();
    in.open(input_filename);

    current.encode(in, out);

    in.close();
    out.close();
}

void decode(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ofstream out;

    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");

    tree current;
    current.read_tree(in);

    current.decode(in, out);

    in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::runtime_error("Usage: -type(-e/-d) in out");
    }
    std::string type(argv[1]);
    if (type == "-e") {
        encode(std::string(argv[2]), std::string(argv[3]));
    } else if (type == "-d") {
        decode(std::string(argv[2]), std::string(argv[3]));
    }

    return 0;
}
