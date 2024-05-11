#include "connection.h"
#include "client_ss.h"

constexpr int PORT_NAME_SERVER = 8080;
constexpr int PORT_CLIENT = 8000;
constexpr int BUFFER_SIZE = 1024;
constexpr int TIMEOUT_SECONDS = 5;

void handleConnectionNameServer(int sock, const std::string &connectionType)
{
  struct sockaddr_in client;
  socklen_t clientLen = sizeof(client);

  while (true)
  {
    int connection = accept(sock, (struct sockaddr *)&client, &clientLen);
    if (connection >= 0)
    {
      std::cout << "Received connection from " << connectionType << "\n";
    }
    else
    {
      std::cout << "No connection from " << connectionType << "\n";
    }
  }
}



void 
handleConnectionClient(int sock, const std::string &connectionType)
{
  struct sockaddr_in client;
  socklen_t clientLen = sizeof(client);

  while (true)
  {
    int connection = accept(sock, (struct sockaddr *)&client, &clientLen);
    if (connection >= 0)
    {
      std::cout << "Received connection from " << connectionType << "\n";

      char buffer[BUFFER_SIZE];
      int bytesReceived = recv(connection, buffer, BUFFER_SIZE, 0);
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
      std::stringstream ss(buffer);
      std::istream_iterator<std::string> begin(ss), end;
      std::vector<std::string> words(begin, end);
      int f = checkQuery(words);

      if (words[0] == "read" and f > 0)
      {
        std::string result = read_file(words[1]);
        std::cout << "result: " <<result<< std::endl;
      }
      else if (words[0] == "write" and f > 0)
      {

        int response = write_file(words);
        if (response == 0)
        {
          std::cout << "Success:: writing to the file" << std::endl;
        }
      }
      else if (words[0] == "getperm" and f > 0)
      {
        int flag = checkQuery(words);
        std::string result = "hiERR";
        result = get_permissions(words[1]);
        std::cout << "result: " << result << std::endl;
      }
      else if(f>0)
      {
        std::cout << "Sorry What?" << std::endl;
      }

      close(connection);
    }
    else
    {
      std::cout << "No connection from, probably some Error " << connectionType << "\n";
    }
  }
}

int main()
{
  int nameServerSock = createSocket(PORT_NAME_SERVER);
  int clientSock = createSocket(PORT_CLIENT);

  int opt = 1;
  if (setsockopt(nameServerSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 ||
      setsockopt(clientSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    std::cerr << "Error setting socket options\n";
    exit(1);
  }

  bindSocket(nameServerSock, PORT_NAME_SERVER);
  bindSocket(clientSock, PORT_CLIENT);

  listenSocket(nameServerSock);
  listenSocket(clientSock);

  std::thread nameServerThread(handleConnectionNameServer, nameServerSock, "name server");
  std::thread clientThread(handleConnectionClient, clientSock, "client");

  nameServerThread.join();
  clientThread.join();

  close(nameServerSock);
  close(clientSock);

  return 0;
}
