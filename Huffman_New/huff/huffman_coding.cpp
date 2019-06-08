#include "huffman_coding.h"
#include <set>
#include <cassert>
#include <algorithm>

#include <iostream>

const std::string ERROR_ENC_FILE = "Bad file";

void tree::count_symbs() {
    unsigned char c;
    size_t sz = cur_bufsiz;
    for (size_t i = 0; i < sz; i++) {
        c = buffer[i];
        counts[c]++;
    }
}

void tree::read_buf(std::istream& in) {
    in >> std::noskipws;
    in.read(buffer, SIZE);
    cur_bufsiz = in.gcount();
}

void tree::nullcounts() {
    for(int i = 0; i < 256; ++i)
        counts[i] = 0;
}

tree tree::init_tree() {
    nullcounts();
    count_symbs();

    std::set <std::pair <int, int>> Q;
    for (int i = 0; i < 256; i++)
        if (counts[i]) {
            Q.insert({ counts[i], data.size() });
            data.push_back(node(true, i));
        }
    while (Q.size() > 1) {
        std::pair <int, int> a = *Q.begin();
        Q.erase(Q.begin());
        std::pair <int, int> b = *Q.begin();
        Q.erase(Q.begin());
        Q.insert({ a.first + b.first, data.size() });
        data.push_back(node(false,
                0, a.second, b.second));
    }
    root = data.size() - 1;
    return *this;
}

void tree::parse_vertexes(int i, std::vector <bool> & edges, std::string & symbols, int & e, int & s) {
    data.push_back(node());
    if (edges[e++]) {
        data[i] = node(true, symbols[s++]);
    } else {
        data[i].left = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
        data[i].right = data.size();
        parse_vertexes(data.size(), edges, symbols, e, s);
    }
}

tree tree::read_tree(std::istream& in) {
    nullcounts();
    std::vector<bool> edges;
    std::string symbols = "";
    int sz, alph;
    in >> std::skipws;
    in >> sz;
    if (!in)
        throw std::runtime_error(ERROR_ENC_FILE);

    in >> alph;
    if (!in)
        throw std::runtime_error(ERROR_ENC_FILE);
    unsigned char c;
    in >> std::noskipws >> c;
    if (!in)
        throw std::runtime_error(ERROR_ENC_FILE);
    if (c != ' ' || sz < 0 || alph < 0)
        throw std::runtime_error(ERROR_ENC_FILE);
    while (sz) {
       in >> c;
       if (c != '1' && c != '0' || !in)
           throw std::runtime_error(ERROR_ENC_FILE);
       edges.push_back(c == '1');
       --sz;
    }
    while (alph) {
        in >> c;
        if (!(0 <= c && c < 256) || (alph && !in))
            throw std::runtime_error(ERROR_ENC_FILE);
        symbols.push_back(c);
        --alph;
    }
    root = 0;
    if (edges.empty() && !symbols.empty()) {
        data = { node(true, symbols[0]) };
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
    if (data[i].isLeaf) {
        code[data[i].c] = st;
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

void tree::encode(std::ostream& out) {
    if(data.empty()) {
        return;
    }
    std::vector<std::vector<bool>> code(256);
    std::vector<bool> st;
    generate_code(root, code, st);
    int lenCode = 0;
    for(int i = 0; i < 256; ++i) {
        lenCode += counts[i] * code[i].size();
    }
    if (data.size() == 1) {
        code[data[0].c] = { 0 };
        lenCode = counts[data[0].c];
    }
    out << ' ' << lenCode << ' ';
    char cur = 0;
    int cou = 0;
    for (size_t i = 0; i < cur_bufsiz; i++) {
        const auto& seq = code[buffer[i]];
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
    if (cou) {
        out.put(cur);
    }
    out.put('\0');
}

void tree::decode(std::istream& in, std::ostream& out) {
    if (root == -1) {
        return;
    }
    int v = root;
    char cc;
    int count_bits;
    in >> cc;
    if(cc != ' ')
        throw std::runtime_error(ERROR_ENC_FILE);
    in >> count_bits;
    if (!in)
        throw std::runtime_error(ERROR_ENC_FILE);
    in >> cc;
    if(cc != ' ')
        throw std::runtime_error(ERROR_ENC_FILE);
    while (in && count_bits > 0) {
        char c;
        in >> std::noskipws >> c;
        unsigned char symbol = c;
        for (size_t j = 0; j < 8 - (count_bits < 8) * (8 - count_bits); j++) {
            bool c = static_cast<bool>(1 & (symbol >> j));
            if (data.size() > 1) {
                if (c == 0)
                    v = data[v].left;
                else
                    v = data[v].right;
            }
            if (v == -1 || j > 8) {
                throw std::runtime_error(ERROR_ENC_FILE);
            }
            if (data[v].isLeaf) {
                out.put(data[v].c);
                v = root;
            }
        }
        count_bits -= 8;
    }
}
