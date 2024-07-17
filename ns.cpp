#include "connection.h"
#include "imports.h"
#include <vector>
#include <iostream>
#include <thread>

/**
 * 2 listen (one for client on 8090, one for ss on 8080)
 * 1 connect (8000)
 */
constexpr int BUFFER_SIZE = 1024;
constexpr int PORT_CLIENT = 8090;         // listen to client
constexpr int PORT_STORAGE_SERVER = 8080; // listen new ss
constexpr int PORT_CSS = 8088;            // connect to existing ss

std::vector<int> storage_server_sockets;

int get_buffer(int connection, char *&buffer, int bufferSize, int flags)
{
  int bytesReceived = recv(connection, buffer, bufferSize, flags);
  if (bytesReceived > 0)
  {
    buffer[bytesReceived] = '\0';
    std::cout << "Received data from client: " << buffer << "\n";
  }
  else if (bytesReceived == 0)
  {
    std::cout << "Client closed connection\n";
  }
  else
  {
    std::cerr << "Error receiving data from client\n";
  }
  return bytesReceived;
}

void handleConnectionStorageServer(int sock, const std::string &connectionType)
{
  struct sockaddr_in client;
  socklen_t clientLen = sizeof(client);

  while (true)
  {
    int connection = accept(sock, (struct sockaddr *)&client, &clientLen);
    if (connection >= 0)
    {
      std::cout << "Received connection from " << connectionType << "\n";
      storage_server_sockets.push_back(connection);
      // Handle storage server connection here
    }
    else
    {
      std::cout << "No connection from " << connectionType << "\n";
    }
  }
}

void handleConnectionClient(int sock, const std::string &connectionType, int port)
{
  struct sockaddr_in client;
  socklen_t clientLen = sizeof(client);
  int socki = socket(AF_INET, SOCK_STREAM, 0);
  if(socki==-1){
    std::cout<<"ERRR"<<std::endl;
    return;
  }
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
  server.sin_port = htons(port);

  if (connect(socki, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    std::cout << "Connection failed to port " << port << "\n";
    return;
  }
  std::cout << "Connected to port " << port << "\n";

  while (true)
  {
    int connection = accept(sock, (struct sockaddr *)&client, &clientLen);
    if (connection >= 0)
    {
      std::cout << "Received connection from " << connectionType << "\n";
      char buffer[BUFFER_SIZE];
      char *ptr = buffer;
      int bytesReceived = get_buffer(connection, ptr, BUFFER_SIZE, 0);
      std::stringstream ss(buffer);
      std::istream_iterator<std::string> begin(ss), end;
      std::vector<std::string> words(begin, end);

      if (words[0] == "create")
      {
      }
      else if (words[0] == "delete")
      {
      }
      else if (words[0] == "copy")
      {
      }
      else if (words[0] == "read")
      {
      }
      else if (words[0] == "write")
      {
        std::cout << "BOYAH" << std::endl;
      }
      else if (words[0] == "getperm")
      {
      }
    }
    else
    {
      std::cout << "No connection from " << connectionType << "\n";
    }
  }
}

int main()
{
  int clientSock = createSocket(PORT_CLIENT);
  int storageServerSock = createSocket(PORT_STORAGE_SERVER);

  int opt = 1;
  if (setsockopt(clientSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ||
      setsockopt(storageServerSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    std::cerr << "Error setting socket options\n";
    exit(1);
  }

  bindSocket(clientSock, PORT_CLIENT);
  bindSocket(storageServerSock, PORT_STORAGE_SERVER);

  listenSocket(clientSock);
  listenSocket(storageServerSock);

  std::thread storageServerThread(handleConnectionStorageServer, storageServerSock, "storage server");
  std::thread clientThread(handleConnectionClient, clientSock, "client", PORT_CSS);

  storageServerThread.join();
  clientThread.join();

  close(clientSock);
  close(storageServerSock);
  // close(clientSock);

  return 0;
}