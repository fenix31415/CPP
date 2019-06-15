#include "../huff/huffman_coding.h"

#include <iostream>
#include <string>
#include <cassert>
#include <exception>
#include "Utility.h"

const std::string TESTS_PATH = "Tests/";
const int TESTS_COUNT = 6;
const std::string ERROR_ENC_FILE = "Bad file";

bool compare(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ifstream out;
    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open " + input_filename + " file");
    if (!out.is_open())
        throw std::runtime_error("Can't open " + target_filename + " file");

    bool ans = true;
    while (in && out) {
        char inc = '1', outc = '1';
        in.get(inc);
        out.get(outc);
        ans &= ((unsigned char)inc == (unsigned char)outc);
    }
    ans &= !in && !out;
    return ans;
}

void test(int step) {
    std::cout << "testing " << step << " test..." << std::endl;
    std::string in = TESTS_PATH + "zip" + std::to_string(step) + ".in";
    std::string tmp = TESTS_PATH + "zip" + std::to_string(step) + ".tmp";
    std::string out = TESTS_PATH + "zip" + std::to_string(step) + ".out";
    zip(in, tmp);
    unzip(tmp, out);
    bool b = compare(in, out);
    if (b)
        std::cout<<"OK\n";
    else std::cout<<"FAIL\n";
    assert(b);
}

void test_compare() {
    std::cout<<"Testing comparing..\n";
    assert(compare(TESTS_PATH+"comp1.in", TESTS_PATH+"comp1.out"));
    assert(compare(TESTS_PATH+"comp2.in", TESTS_PATH+"comp2.out"));
    assert(!compare(TESTS_PATH+"comp3.in", TESTS_PATH+"comp3.out"));
    assert(compare(TESTS_PATH+"comp4.in", TESTS_PATH+"comp4.out"));
    assert(!compare(TESTS_PATH+"comp5.in", TESTS_PATH+"comp5.out"));
    std::cout<<"Comparing done!\n\n";
}

void test_zipping() {
    std::cout<<"Testing zip..\n";
    for(int step = 1; step <= TESTS_COUNT; ++step) {
        test(step);
    }
    std::cout<<"Zipping done!\n\n";
}

void test_zipping_() {
    int step = 0;
    std::cout<<"testing " << step << " test..." << std::endl;
    std::string in = TESTS_PATH + "zip" + std::to_string(step) + ".in";
    std::string tmp = TESTS_PATH + "zip" + std::to_string(step) + ".tmp";
    std::string out = TESTS_PATH + "zip" + std::to_string(step) + ".out";
    zip(in, tmp);
    unzip(tmp, out);
    std::cout<<"Ogk"<<std::endl;
}

void test_unzipping_(int step) {
    std::cout << "testing " << step << " test..." << std::endl;
    std::string in = TESTS_PATH + "unzip" + std::to_string(step) + ".in";
    std::string out = TESTS_PATH + "unzip" + std::to_string(step) + ".out";
    int norm = 1;
    try {
        unzip(in, out);
    } catch (...) {
        norm = 0;
    }
    assert(norm);
    std::cout<<"OK\n";
}

void test_unzipping() {
    std::cout<<"Testing unzip..\n";

    for(int i = 1; i < 0; ++i) {
        test_unzipping_(i);
    }

    std::cout<<"Unzipping done!\n\n";
}

void TEST() {
    std::cout<<"Testing..\n\n";

    test_compare();

    test_zipping_();

    test_zipping();

    test_unzipping();

    std::cout<<"Done!\n";
}

size_t read(std::istream& in, unsigned char *& buffer, size_t & size, size_t need, bool strong) {
    if (size < need || buffer == nullptr) {
        if(buffer != nullptr) {
            delete[] buffer;
        }
        size_t nsz = std::max(need, SIZE);
        buffer = new unsigned char[nsz];
        size = nsz;
    }
    if (!strong) {
        in.read(reinterpret_cast<char *>(buffer), need);
        return in.gcount();
    }
    size_t readed = 0;
    while (need > 0) {
        if (!in) throw std::runtime_error("Unexpected end of file");
        in.read(reinterpret_cast<char *>(buffer + readed), need);
        readed += in.gcount();
        need -= in.gcount();
    }
    return readed;
}

void encode(std::istream& in, std::ostream& out) {
    while (in) {
        tree current;
        out.put('\0');

        size_t cur_bufsiz = current.get_cur_bufsiz();
        size_t valid_bufsiz = read(in, current.buffer, cur_bufsiz, SIZE, false);
        current.init_buffer(valid_bufsiz, cur_bufsiz);

        std::vector<unsigned char> tree;
        std::vector<unsigned char> ans;
        current.encode(tree, ans);

        for (auto i : tree)
            out << i;
        for (auto i : ans)
            out << i;
    }
    out.put('\1');
}

unsigned int read_uint(std::istream& in) {
    unsigned int ans = 0;
    for(int i = 0; i < 4; ++i) {
        char c;
        in.get(c);
        ans |= (unsigned int)(unsigned char)c << (i * 8);
    }
    return ans;
}

void decode(std::istream& in, std::ostream& out) {
    char cc;
    bool started = false;
    while (in >> cc) {
        started = true;
        if(cc == '\1')
            break;
        tree current;
        size_t sz = read_uint(in);
        size_t alph = read_uint(in);
        size_t need = sz + alph;

        size_t cur_bufsiz = current.get_cur_bufsiz();
        size_t valid_bufsiz = read(in, current.buffer, cur_bufsiz, need, true);
        current.init_buffer(valid_bufsiz, cur_bufsiz);

        current.read_tree(sz, alph);

        size_t lencode = read_uint(in);
        need = 0;
        if (lencode > 0)
            need = (lencode - 1) / 8 + 1;

        cur_bufsiz = current.get_cur_bufsiz();
        valid_bufsiz = read(in, current.buffer, cur_bufsiz, need, true);
        current.init_buffer(valid_bufsiz, cur_bufsiz);

        out << current.decode_text(lencode);
    }
    if (!started)
        throw std::runtime_error(ERROR_ENC_FILE);
}

void zip(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    in.open(input_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");

    std::ofstream out(target_filename);
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");

    encode(in, out);

    out.close();
}

void unzip(std::string input_filename, std::string target_filename) {
    std::ifstream in;

    in.open(input_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");


    std::ofstream out(target_filename);
    if (!out.is_open())
        throw std::runtime_error("Can't open out file");

    try {
        decode(in, out);
    } catch (...) {
        std::cout<<ERROR_ENC_FILE<<std::endl;
    };
}

int main(int argc, char* argv[]) {
    if(argc == 2) {
        std::string type(argv[1]);
        if (type == "-t") {
            TEST();
        }
        return 0;
    }
    if (argc != 4) {
        std::cout<<"Usage: -type(-e/-d) in out"<<std::endl;
        return 0;
    }
    std::string type(argv[1]);
    if (type == "-e") {
        try {
            zip(std::string(argv[2]), std::string(argv[3]));
        } catch (const std::runtime_error& e) {
            std::cout<<"Error: "<<e.what()<<std::endl;
            return 0;
        }
    } else if (type == "-d") {
        try {
            unzip(std::string(argv[2]), std::string(argv[3]));
        } catch (const std::runtime_error& e) {
            std::cout<<"Error: "<<e.what()<<std::endl;
            return 0;
        }
    }

    return 0;
}
