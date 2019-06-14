#ifndef HUFFMAN_CODING
#define HUFFMAN_CODING

#include<vector>
#include<fstream>
#include<cassert>
#include<iostream>

size_t const SIZE = (1 << 20);

class tree {
public:
    tree() : root(0), counts(256, 0), pos(-1), buffer(new unsigned char[SIZE]), cur_bufsiz(SIZE) {}
    ~tree() { delete[] buffer; }
    void init_tree();
    void nullcounts();
    void count_symbs();
    void read_tree(size_t sz, size_t alph);
    std::string write_tree();
    void encode(std::vector<unsigned char> & ans);
    std::string decode(size_t count_bits);
    unsigned char *buffer;
    size_t cur_bufsiz;
    size_t valid_bufsiz;
    size_t pos;
private:
    std::vector<int> counts;
    struct node {
        bool isLeaf;
        unsigned char c;
        int left, right;
        node() : isLeaf(0), c(0), left(-1), right(-1) {}
        node(bool b, unsigned char c, int l = -1, int r = -1) : isLeaf(b), c(c), left(l), right(r) {}
    };
    char get_next();
    int root;
    std::vector<node> data;
    void dfs_code(std::vector <unsigned char> & output, int i);
    void dfs_symbs(std::vector <unsigned char> & output, int i);
    void parse_vertexes(int i, std::vector<bool> & a, std::string & b, int & id1, int & id2);
    int generate_code(int i, std::vector<std::vector<bool>> & codes, std::vector<bool> & cur);
    unsigned int get_uint();
};

void append(std::vector<unsigned char> & v, unsigned int a);
unsigned int vec_to_uint(std::vector<unsigned char> a);

#endif // HUFFMAN_CODING
