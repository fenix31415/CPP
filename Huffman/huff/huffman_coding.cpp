#include "huffman_coding.h"
#include <set>
#include <cassert>
#include <algorithm>
#include <bitset>

#include <iostream>

const std::string ERROR_ENC_FILE = "Bad file";

void tree::count_symbs() {
    unsigned char c;
    size_t sz = valid_bufsiz;
    for (size_t i = 0; i < sz; i++) {
        c = buffer[i];
        ++counts[c];
    }
}

void tree::nullcounts() {
    for(int i = 0; i < 256; ++i)
        counts[i] = 0;
}

void tree::init_tree() {
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

void tree::read_tree(size_t sz, size_t alph) {
    nullcounts();
    std::vector<bool> edges;
    std::string symbols = "";
    unsigned char c;

    if (sz < 0 || alph < 0)
        throw std::runtime_error(ERROR_ENC_FILE);

    root = 0;
    if(sz == 0 && alph == 0)
        root = -1;

    while (sz) {
       c = get_next();
       if (c != '1' && c != '0')
           throw std::runtime_error(ERROR_ENC_FILE);
       edges.push_back(c == '1');
       --sz;
    }

    while (alph) {
        c = get_next();
        if (!(0 <= c && c < 256))
            throw std::runtime_error(ERROR_ENC_FILE);
        symbols.push_back(c);
        --alph;
    }
    if (root != -1) {
        int e, s;
        e = s = 0;
        parse_vertexes(0, edges, symbols, e, s);
    }
}

void tree::dfs_symbs(std::vector <unsigned char> & ans, int id) {
    node v = data[id];
    if (v.left != -1) {
        dfs_symbs(ans, v.left);
        dfs_symbs(ans, v.right);
    } else {
        ans.push_back((v.c));
    }
}

void tree::dfs_code(std::vector <unsigned char> & ans, int id) {
    node v = data[id];
    if (v.left != -1) {
        ans.push_back('0');
        dfs_code(ans, v.left);
        dfs_code(ans, v.right);
    } else {
        ans.push_back('1');
    }
}

std::string tree::write_tree() {
    std::string ans;
    std::vector<unsigned char> output;
    unsigned int tree_size = 0;
    if (root != -1) {
        dfs_code(output, root);
        tree_size = output.size();
        dfs_symbs(output, root);
    }

    std::vector<unsigned char> v;
    append(v, tree_size);
    ans.insert(ans.end(), v.begin(), v.end());
    v.resize(0);
    append(v, output.size() - tree_size);
    ans.insert(ans.end(), v.begin(), v.end());
    for (auto c : output)
        ans.push_back(c);
    return ans;
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

void append(std::vector<unsigned char> & v, unsigned int a) {
    unsigned int mask = (1 << 8) - 1;
    for (int i = 0; i < sizeof(unsigned int); ++i) {
        unsigned char c = (unsigned char)((a & mask) >> i * 8);
        v.push_back(c);
        mask <<= 8;
    }
}

unsigned int vec_to_uint(std::vector<unsigned char> a) {
    unsigned int ans = 0;
    for(int i = 0; i < 4; ++i) {
        ans |= (unsigned int)a[i] << (i * 8);
    }
    return ans;
}

void tree::encode(std::vector<unsigned char>& ans) {
    std::vector<std::vector<bool>> code(256);
    std::vector<bool> st;
    if(root != -1)
        generate_code(root, code, st);
    unsigned int lenCode = 0;
    for(int i = 0; i < 256; ++i) {
        lenCode += counts[i] * code[i].size();
    }
    if (data.size() == 1) {
        code[data[0].c] = { 0 };
        lenCode = counts[data[0].c];
    }
    append(ans, lenCode);
    char cur = 0;
    int cou = 0;
    for (size_t i = 0; i < valid_bufsiz; i++) {
        const auto& seq = code[buffer[i]];
        for (bool b : seq) {
            cur |= (b << cou);
            cou++;
            if (cou == 8) {
                ans.push_back(cur);
                cur = 0;
                cou = 0;
            }
        }
    }
    if (cou) {
        ans.push_back(cur);
    }
}

char tree::get_next() {
    assert(valid_bufsiz > pos + 1);
    ++pos;
    return buffer[pos];
}

unsigned int tree::get_uint() {
    std::vector<unsigned char> v;
    for(int i = 0; i < 4; ++i) {
        char c = get_next();
        v.push_back((unsigned char)c);
    }
    return vec_to_uint(v);
}

std::string tree::decode(size_t count_bits) {
    if (root == -1) {
        return "";
    }
    int v = root;
    std::string ans;
    while (count_bits > 0) {
        char c;
        c = get_next();
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
                ans.push_back(data[v].c);
                v = root;
            }
        }
        if(count_bits < 8)
            break;
        count_bits -= 8;
    }
    return ans;
}
