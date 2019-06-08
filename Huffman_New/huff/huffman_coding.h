#ifndef HUFFMAN_CODING
#define HUFFMAN_CODING

#include<vector>
#include<fstream>

int const SIZE = (1 << 10);

class tree {
public:
    tree() : root(0), counts(256, 0) {}
    void read_buf(std::istream& in);
    tree init_tree();
    void nullcounts();
    void count_symbs();
    tree read_tree(std::istream& in);
    void write_tree(std::ostream& out);
    void encode(std::ostream& out);
    void decode(std::istream&in, std::ostream& out);
private:
    std::vector<int> counts;
    char buffer[SIZE];
    int cur_bufsiz;
    struct node {
        bool isLeaf;
        unsigned char c;
        int left, right;
        node() : isLeaf(0), c(0), left(-1), right(-1) {}
        node(bool b, unsigned char c, int l = -1, int r = -1) : isLeaf(b), c(c), left(l), right(r) {}
    };
    int root;
    std::vector<node> data;
    void dfs_code(std::vector <unsigned char> & output, int i, std::ostream& out);
    void dfs_symbs(std::vector <unsigned char> & output, int i, std::ostream& out);
    void parse_vertexes(int i, std::vector<bool> & a, std::string & b, int & id1, int & id2);
    int generate_code(int i, std::vector<std::vector<bool>> & codes, std::vector<bool> & cur);
};

#endif // HUFFMAN_CODING
