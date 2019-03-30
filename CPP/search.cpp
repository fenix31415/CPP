#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>

void prefix_find(FILE* fp, char* c, std::vector<int> *p) {
	size_t l;
	(*p)[0] = 0;
	for (l = 1; c[l]; ++l) {
		size_t j = (*p)[l - 1];
		while ((j > 0) && (c[l] != c[j]))
			j = (*p)[j - 1];
		if (c[l] == c[j])
			++j;
		(*p)[l] = j;
	}
	size_t curr = 0;
	int ch, ind = 0;
	while ((ch = std::fgetc(fp)) != EOF) {
		while ((curr > 0) && (ch != c[curr]))
			curr = (*p)[curr - 1];
		if (ch == c[curr])
			++curr;
		if (curr == l) {
			std::cout << ind - l + 1;
			return;
		}
		++ind;
	}
	std::cout << "NO";
}

int main(int argc, char* argv[]) {
	FILE* fp = std::fopen(argv[1], "r");
	if (!fp) {
		std::cout<<"Can't open";
		return 0;
	}
	std::vector<int> p;
	int cLen;
	//std::cout<<(argv[1]);
	for (cLen = 0; argv[2][cLen]; ++cLen) {};
	p.resize(cLen,0);
	prefix_find(fp, argv[2], &p);

	std::fclose(fp);
}