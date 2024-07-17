#ifndef CLIENT_SS_H
#define CLIENT_SS_H

#include "imports.h"

namespace fs = std::filesystem;

std::string read_file(std::string path);
int write_file(std::vector<std::string> query);
std::string get_permissions(std::string path);
int checkQuery(std::vector<std::string>& words);

#endif 
