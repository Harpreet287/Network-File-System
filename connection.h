#ifndef CONNECTION_H
#define CONNECTION_H
#include "imports.h"

extern int createSocket(int port);
extern void bindSocket(int sock, int port);
extern void listenSocket(int sock);

#endif 