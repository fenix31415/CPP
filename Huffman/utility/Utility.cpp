#include "../huff/huffman_coding.h"

using namespace std;

void encode(string input_filename, string target_filename) {
    ifstream in;
    ofstream out;
    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open() || !out.is_open()) throw runtime_error("Could not open files");

    tree current;

    current.init(in);
    current.write_tree(out);

    in.close();
    in.open(input_filename);

    current.encode(in, out);

    in.close();
    out.close();
}

void decode(string input_filename, string target_filename) {
    ifstream in;
    ofstream out;

    in.open(input_filename);
    out.open(target_filename);

    if (!in.is_open() || !out.is_open()) throw runtime_error("Could not open files");

    tree current;
    current.read_tree(in);

    current.decode(in, out);

    in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    try {
        string type(argv[1]);
        if (type == "-e") {
            encode(string(argv[2]), string(argv[3]));
        } else if (type == "-d") {
            decode(string(argv[2]), string(argv[3]));
        }
    } catch (...) {
        throw runtime_error("Wrong query");
    }

    return 0;
}
