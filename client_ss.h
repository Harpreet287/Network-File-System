#ifndef CLIENT_SS_H
#define CLIENT_SS_H

#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <filesystem>

namespace fs = std::filesystem;

std::string read_file(std::string path);
int write_file(std::vector<std::string> query);
std::string get_permissions(std::string path);
int checkQuery(std::vector<std::string> words);

#endif 
