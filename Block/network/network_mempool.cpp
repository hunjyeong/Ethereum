#include <iostream>
#include <openssl/sha.h>
#include <random>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <cassert>
#include <random>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

using namespace::std;
typedef unsigned char BYTE;


struct Transaction {
    char sender[50];
    char receiver[50];
    double amount;
};

int main() {
    const int PORT = 36707;
    const char* IP = "0.0.0.0";
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);

    std::vector<Transaction> transactions(44);

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);

        Transaction transaction;
        memset(&transaction, 0, sizeof(transaction));

        int bytes_received = recv(client_socket, &transaction, sizeof(transaction), 0);

        if (bytes_received > 0) {
            transactions.push_back(transaction);

            std::cout << "Transaction received:" << std::endl;
            std::cout << "Sender: " << transaction.sender << std::endl;
            std::cout << "Receiver: " << transaction.receiver << std::endl;
            std::cout << "Amount: " << transaction.amount << std::endl;
        }

        close(client_socket);
    }

    close(server_socket);

    return 0;
}
