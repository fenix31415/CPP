#include "huffman_coding.h"
#include <set>
#include <cassert>
#include <algorithm>

#include <iostream>

using namespace std;
const std::string ERROR_ENC_FILE = "Bad file";

void tree::count_symbs(std::istream& in) {
    unsigned char c;
    in >> noskipws;

    unsigned char buf[SIZE];
    while (in) {
        in.read(reinterpret_cast<char*>(buf), SIZE);
        size_t sz = in.gcount();
        for (size_t i = 0; i < sz; i++) {
            c = buf[i];
            counts[c]++;
        }
    }
}

tree tree::init(std::istream& in) {
    this->count_symbs(in);

    std::set <std::pair <int, int>> Q;
    for (int i = 0; i < 256; i++)
        if (counts[i]) {
            Q.insert({ counts[i], data.size() });
            data.push_back(node(0, i));
        }
    while (Q.size() > 1) {
        std::pair <int, int> a = *Q.begin();
        Q.erase(Q.begin());
        std::pair <int, int> b = *Q.begin();
        Q.erase(Q.begin());
        Q.insert({ a.first + b.first, data.size() });
        data.push_back(node(1 + std::max(data[a.second].depth, data[b.second].depth), 0, a.second, b.second));
    }
    root = data.size() - 1;
    return *this;
}

void tree::parse_vertexes(int i, vector <bool> & edges, string & symbols, int & e, int & s) {
    data.push_back(node());
    if (edges[e++]) {
        data[i] = node(0, symbols[s++]);
    } else {
        data[i].left = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
        data[i].right = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
    }
}

tree tree::read_tree(istream& in) {
    std::vector<bool> edges;
    string symbols = "";
    int sz, alph;
    in >> sz;
    if (!in)
        throw runtime_error(ERROR_ENC_FILE);
    in >> alph;
    if (!in)
        throw runtime_error(ERROR_ENC_FILE);
    unsigned char c;
    in >> noskipws >> c;
    if (!in)
        throw runtime_error(ERROR_ENC_FILE);
    if (c != ' ' || sz < 0 || alph < 0)
        throw runtime_error(ERROR_ENC_FILE);
    while (sz) {
       in >> c;
       if (c != '1' && c != '0' || !in)
           throw runtime_error(ERROR_ENC_FILE);
       edges.push_back(c == '1');
       --sz;
    }
    while (alph) {
        in >> c;
        if (!(0 <= c && c < 256) || (alph && !in)) throw runtime_error(ERROR_ENC_FILE);
        symbols.push_back(c);
        --alph;
    }
    root = 0;
    if (edges.empty() && !symbols.empty()) {
        data = { node(0, symbols[0]) };
    } else {
        int e, s;
        e = s = 0;
        parse_vertexes(0, edges, symbols, e, s);
    }
    return *this;
}

void tree::dfs_symbs(std::vector <unsigned char> & ans, int id, std::ostream& out) {
    node v = data[id];
    if (v.left != -1) {
        dfs_symbs(ans, v.left, out);
        dfs_symbs(ans, v.right, out);
    } else {
        ans.push_back((v.c));
    }
}

void tree::dfs_code(std::vector <unsigned char> & ans, int id, std::ostream& out) {
    node v = data[id];
    if (v.left != -1) {
        ans.push_back('0');
        dfs_code(ans, v.left, out);
        dfs_code(ans, v.right, out);
    } else {
        ans.push_back('1');
    }
}

void tree::write_tree(std::ostream& out) {
    std::vector<unsigned char> output;
    size_t tree_size = 0;
    if (root != -1) {
        dfs_code(output, root, out);
        tree_size = output.size();
        dfs_symbs(output, root, out);
    }
    out << tree_size << ' ' << output.size() - tree_size << ' ';
    for (auto c : output)
        out << c;
}

int tree::generate_code(int i, std::vector <std::vector <bool>> & code, std::vector <bool> & st) {
    if (data[i].depth == 0) {
        code[data[i].c] = st;
        //std::cout<<st.size() << counts[i]<<i<<"\n";
        return st.size() * counts[i];
    } else {
        st.push_back(0);
        int lcou = generate_code(data[i].left, code, st);
        st[st.size() - 1] = 1;
        int rcou = generate_code(data[i].right, code, st);
        st.pop_back();
        return lcou + rcou;
    }
}

void tree::encode(std::istream& in, ostream& out) {
    auto begin = out.tellp();
    out.put(' ');
    if(data.empty()) {
        return;
    }
    std::vector<vector<bool>> code(256);
    std::vector<bool> st;
    int lenCode = generate_code(root, code, st);
    //std::cout<<lenCode<<"\n";
    if (data.size() == 1)
        code[data[0].c] = { 0 };

    unsigned char buf[SIZE];
    char cur = 0;
    int cou = 0;
    while (in) {
        in.read(reinterpret_cast<char*>(buf), SIZE);
        size_t sz = in.gcount();
        for (size_t i = 0; i < sz; i++) {
            const auto& seq = code[buf[i]];
            for (bool b : seq) {
                cur |= (b << cou);
                cou++;
                if (cou == 8) {
                    out.put(cur);
                    cur = 0;
                    cou = 0;
                }
            }
        }
    }
    if (cou) {
        out.put(cur);
        cou = 8 - cou;
    }
    out.seekp(begin);
    out.put(char(cou));
}

void tree::decode(istream& in, ostream& out) {
    if (root == -1) {
        return;
    }
    int v = root;
    char buf[SIZE];
    char bad_bits;
    in >> noskipws >> bad_bits;
    if (!in) throw runtime_error(ERROR_ENC_FILE);
    while(in) {
        in.read(reinterpret_cast<char*>(buf), SIZE);
        size_t sz = in.gcount();
        for (size_t i = 0; i < sz; i++) {
            unsigned char symbol = buf[i];
            for (size_t j = 0; j < 8 - (!in && i == sz - 1) * static_cast<int>(bad_bits); j++) {
                bool c = static_cast<bool>(1 & (symbol >> j));
                if (data.size() > 1)
                    if (c == 0)
                        v = data[v].left;
                    else v = data[v].right;
                if (v == -1 || j > 8) {
                    throw runtime_error(ERROR_ENC_FILE);
                }
                if (data[v].depth == 0) {
                    out.put(data[v].c);
                    v = root;
                }
            }
        }
    }
}
