#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

void send_message(int clientSocket, const std::string& message) {
    if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
        std::cerr << "Error sending message to server" << std::endl;
        exit(1);
    }
    std::cout << "Message sent to server: " << message << std::endl;
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    // Send message to the server
    std::string message = "delete ./new_file.txt";
    send_message(clientSocket, message);

    // Close the socket
    close(clientSocket);

    return 0;
}
