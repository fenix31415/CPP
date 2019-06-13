#ifndef UTILITY
#define UTILITY

#include <iostream>

void zip(std::string input_filename, std::string target_filename);
void unzip(std::string input_filename, std::string target_filename);
void encode(std::istream& in, std::ostream& out);
void decode(std::istream& in, std::ostream& out);

#endif
