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

    while (in) {
        tree current;
        out.put('\0');
        current.read_buf(in);
        current.init_tree();
        current.write_tree(out);
        current.encode(out);
    }
    out.put('\1');


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
    int cou = 0;


    while (in && !in.eof()) {
        char cc;
        in >> cc;
        if(cc == '\1')
            break;
        tree current;
        current.read_tree(in);
        current.decode(in, out);
        ++cou;
        char c;
        in >> c;
    }

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