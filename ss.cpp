#include "connection.h"
#include "client_ss.h"

constexpr int PORT_NAME_SERVER = 8088; // bind krna ispe to listen to ns
constexpr int PORT_CLIENT = 8000;      // bind krna ispe to listen to client
constexpr int PORT_CNS = 8080;         // connect krna ispe to send all the details to ns
constexpr int BUFFER_SIZE = 1024;
constexpr int TIMEOUT_SECONDS = 5;

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

void create_file(std::string path)
{
  std::ofstream outfile(path);
  if (outfile.is_open())
  {
    std::cout << "File created successfully: " << path << std::endl;
    outfile.close();
  }
  else
  {
    std::cerr << "Failed to create file: " << path << std::endl;
  }
}

void delete_file(std::string path)
{
  std::ifstream infile(path);
  if (!infile.good())
  {
    std::cerr << "File does not exist: " << path << std::endl;
    return;
  }

  if (std::remove(path.c_str()) == 0)
  {
    std::cout << "File deleted successfully: " << path << std::endl;
  }
  else
  {
    std::cerr << "Failed to delete file: " << path << std::endl;
  }
}

void copy_file(std::string source, std::string destination)
{
}

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
      // int bytesReceived = get_buffer(connection,buffer, BUFFER_SIZE, 0);
      char buffer[BUFFER_SIZE];
      char *ptr = buffer;
      int bytesReceived = get_buffer(connection, ptr, BUFFER_SIZE, 0);
      std::stringstream ss(buffer);
      std::istream_iterator<std::string> begin(ss), end;
      std::vector<std::string> words(begin, end);

      if (words[0] == "create")
      {
        create_file(words[1]);
      }
      else if (words[0] == "delete")
      {
        delete_file(words[1]);
      }
      else if (words[0] == "copy")
      {
        if (words.size() < 3)
        {
          std::cout << "Insufficient Arguments provided" << std::endl;
          continue;
        }
        copy_file(words[1], words[2]);
      }
      else
      {
        std::cout << "Sorry What?" << std::endl;
      }
    }
    else
    {
      std::cout << "No connection from " << connectionType << "\n";
    }
  }
}

void connectToNS(int sock, const std::string &connectionType)
{
  const char *message = "hello";
  if (send(sock, message, strlen(message), 0) < 0)
  {
    std::cerr << "Error sending message to CNS" << std::endl;
  }
  else
  {
    std::cout << "Sent 'hello' to CNS" << std::endl;
  }
}

void handleConnectionClient(int sock, const std::string &connectionType)
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
      char *ptr = buffer;
      int bytesReceived = get_buffer(connection, ptr, BUFFER_SIZE, 0);
      std::stringstream ss(buffer);
      std::istream_iterator<std::string> begin(ss), end;
      std::vector<std::string> words(begin, end);
      int f = checkQuery(words);

      if (words[0] == "read" and f > 0)
      {
        std::string result = read_file(words[1]);
        std::cout << "result: " << result << std::endl;
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
      else if (f > 0)
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
  int cnsSock = socket(AF_INET, SOCK_STREAM, 0);
  if (cnsSock < 0)
  {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT_CNS);
  serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Assuming the server is running on the same machine

  if (connect(cnsSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    std::cerr << "Error connecting to server" << std::endl;
    close(cnsSock);
    return 1;
  }

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

  std::thread cnsThread(connectToNS, cnsSock, "SS");
  std::thread nameServerThread(handleConnectionNameServer, nameServerSock, "name server");
  std::thread clientThread(handleConnectionClient, clientSock, "client");

  nameServerThread.join();
  clientThread.join();
  cnsThread.join();

  close(nameServerSock);
  close(clientSock);

  return 0;
}
