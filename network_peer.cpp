#include <iostream>
#include <openssl/sha.h>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
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


using namespace::std;
using namespace boost::multiprecision;
typedef unsigned char BYTE;

struct Transaction {
    std::string sender;
    std::string receiver;
    double amount;
};

int main() {
    const int PORT = 36707;
    const char* IP = "192.168.0.39"; // 공유기 내에서 대상 컴퓨터의 IP 주소 입력
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &server_addr.sin_addr);

    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    Transaction transaction;
    transaction.sender = "A";
    transaction.receiver = "B";
    transaction.amount = 100.0;

    send(client_socket, &transaction, sizeof(transaction), 0);

    close(client_socket);

    std::cout << "Transaction sent successfully!" << std::endl;

    return 0;
}
