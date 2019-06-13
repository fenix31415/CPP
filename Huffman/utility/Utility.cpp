#include "../huff/huffman_coding.h"

#include <iostream>
#include <string>
#include <cassert>
#include "Utility.h"

const std::string TESTS_PATH = "Tests/";
const int TESTS_COUNT = 6;

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
    if (compare(in, out))
        std::cout<<"OK\n";
    else std::cout<<"FAIL\n";
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

void test_bits_(unsigned int a) {
    std::vector<unsigned char> v;
    append(v, a);
    unsigned int b = vec_to_uint(v);
    assert(a == b);
}

void test_bits() {
    std::cout<<"Testing bits..\n";
    test_bits_(0);
    test_bits_(1);
    test_bits_(45);
    test_bits_(128);
    test_bits_(256);
    test_bits_(31415926);
    test_bits_(546841);
    test_bits_(2251556);
    test_bits_(4294967168);
    std::cout<<"Bits done!\n\n";
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

void TEST() {
    std::cout<<"Testing..\n\n";

    //test_compare();

    //test_bits();

    //test_zipping_();

    test_zipping();

    std::cout<<"Done!\n";
}

void encode(std::istream& in, std::ostream& out) {
    while (in) {
        tree current;
        out.put('\0');

        in >> std::noskipws;
        in.read(current.buffer, SIZE);
        current.bufsiz = in.gcount();

        current.init_tree();

        out << current.write_tree();
        std::vector<unsigned char> ans;
        current.encode(ans);
        for (auto i : ans)
            out << i;

    }
    out.put('\1');
}

void decode(std::istream& in, std::ostream& out) {
    char cc;
    while (in >> cc) {
        //std::cout<<"decode while"<<std::endl;
        if(cc == '\1')
            break;
        tree current;
        std::vector<unsigned char> v;
        for(int i = 0; i < 4; ++i) {
            char c;
            in.get(c);
            v.push_back(c);
        }
        unsigned int sz = vec_to_uint(v);


        v.resize(0);
        for(int i = 0; i < 4; ++i) {
            char c;
            in.get(c);
            v.push_back(c);
        }
        unsigned int alph = vec_to_uint(v);

        in >> std::noskipws;
        unsigned int need = sz + alph;
        in.read(current.buffer, need);
        assert(need == in.gcount());
        current.bufsiz = need;
        current.pos = -1;

        //std::cout<<"Reading tree"<<sz<<" "<<alph<<std::endl;
        current.read_tree(sz, alph);

        //std::cout<<"Readed tree"<<std::endl;

        v.resize(0);
        for(int i = 0; i < 4; ++i) {
            char c;
            in.get(c);
            v.push_back(c);
        }
        unsigned int lencode = vec_to_uint(v);
        //std::cout<<lencode<<std::endl;
        in >> std::noskipws;
        need = 0;
        if (lencode > 0)
            need = (lencode - 1) / 8 + 1;
        in.read(current.buffer, need);
        //std::cout<<need<<" "<<in.gcount()<<std::endl;
        assert(need == in.gcount());
        current.bufsiz = need;
        current.pos = -1;
        out << current.decode(lencode);

        //std::cout<<"QQQ2"<<std::endl;
    }
}

void zip(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    in.open(input_filename);
    std::ofstream out(target_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");

    encode(in, out);

    out.close();
}

void unzip(std::string input_filename, std::string target_filename) {
    std::ifstream in;
    std::ofstream out(target_filename);

    in.open(input_filename);

    if (!in.is_open())
        throw std::runtime_error("Can't open in file");

    decode(in, out);

    in.close();
    out.close();
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
