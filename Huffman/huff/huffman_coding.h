#ifndef HUFFMAN_CODING
#define HUFFMAN_CODING

#include<vector>
#include<fstream>

int const SIZE = (1 << 20);

class tree {
public:
    tree() : root(0), counts(256, 0) {}
    tree init(std::istream& in);
    void count_symbs(std::istream& in);
    tree read_tree(std::istream& in);
    void write_tree(std::ostream& out);
    void encode(std::istream&in, std::ostream& out);
    void decode(std::istream&in, std::ostream& out);
    std::vector<int> counts;
private:
    struct node {
        int depth;
        unsigned char c;
        int left, right;
        node() : depth(0), c(0), left(-1), right(-1) {}
        node(int d, unsigned char c, int l = -1, int r = -1) : depth(d), c(c), left(l), right(r) {}
    };
    int root;
    std::vector<node> data;
    void dfs_code(std::vector <unsigned char> & output, int i, std::ostream& out);
    void dfs_symbs(std::vector <unsigned char> & output, int i, std::ostream& out);
    void parse_vertexes(int i, std::vector<bool> & a, std::string & b, int & id1, int & id2);
    int generate_code(int i, std::vector<std::vector<bool>> & codes, std::vector<bool> & cur);
};

#endif // HUFFMAN_CODING
