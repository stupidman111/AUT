#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include <string>
#include <random>

static std::default_random_engine e;

class Server;
class Client {
public:
    Client(std::string id, const Server& server);
    std::string get_id();// Returns the Client's id.
    std::string get_publickey() const;//Returns the Client's public key.
    double get_wallet();//Returns the amount of money the client has.
    std::string sign(std::string txt) const;//signs the input with the private key and returns the signature.
    bool transfer_money(std::string receiver, double value);
    size_t generate_nonce();//Returns a random number as a nonce so the server uses it for mining.
private:
    Server const* const server;//T const* const p等价于const T* const p，这里很重要：指向常量的指针只能访问它的const成员函数
    const std::string id;
    std::string public_key;
    std::string private_key;
};

#endif //CLIENT_H


