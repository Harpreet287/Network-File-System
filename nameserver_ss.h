#ifndef NAMESERVER_SS_H
#define NAMESERVER_SS_H

#include "imports.h"
const int BUFFER_SIZE = 1024;

int get_buffer(int connection, char* &buffer, int bufferSize, int flags);

void create_file(std::string path);

void delete_file(std::string path);

#endif 