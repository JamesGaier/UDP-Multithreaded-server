#include "sock.hpp"
#include <vector>

int main()
{

    serv::UDPSocket client("127.0.0.1", 8080);
    std::string str = "Hello!";
    std::vector<char> bytes(str.begin(), str.end());
    client.Send(bytes, "127.0.0.1", 8080);

    client.Close();
}