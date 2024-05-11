#ifndef NAMESERVER_SS_H
#define NAMESERVER_SS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
const int BUFFER_SIZE = 1024;

int get_buffer(int connection, char* &buffer, int bufferSize, int flags);

void create_file(std::string path);

void delete_file(std::string path);

#endif 