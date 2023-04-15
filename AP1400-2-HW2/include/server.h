#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <random>



extern std::vector<std::string> pending_trxs;
class Client;
class Server {
public:
    Server();
    std::shared_ptr<Client> add_client(std::string id);
    std::shared_ptr<Client> get_client(std::string id) const;//this function you can get a pointer to a Client using its id.
    double get_wallet(std::string id);// Using this function will return the wallet value of the client with username id.
    static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);//"sender-receiver-value"
    bool add_pending_trx(std::string trx, std::string signature);//
    size_t mine();

    std::map<std::shared_ptr<Client>, double> get_clients() const;
private:
    std::map<std::shared_ptr<Client>, double> clients;//map each client to its wallet
};

void show_wallets(const Server& server);

#endif //SERVER_H

