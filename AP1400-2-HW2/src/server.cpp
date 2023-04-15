#include "server.h"

#include <stdexcept>
#include <iostream>

#include "crypto.h"

std::map<std::shared_ptr<Client>, double> Server::get_clients() const {
    return this->clients;
}

std::vector<std::string> pending_trxs = {};

void show_wallets(const Server& server) {
    for (const auto& client : server.get_clients()) {
        std::cout << client.first->get_id() << " : " << client.second << std::endl;
    }
}

Server::Server() {
    this->clients = {};
}

std::string four_digit_random() {
    std::string rd_str{};
    for (int i = 0; i < 4; ++i) {
        auto num = e() % 10;
        rd_str += std::to_string(num);
    }
    return rd_str;
}



std::shared_ptr<Client> Server::add_client(std::string id) {
    bool isExist = false;
    for (const auto& client : this->get_clients()) {
        if (client.first->get_id() == id) {
            isExist = true;
            break;
        }
    }


    if (isExist) {
        id += four_digit_random();
    }
    std::shared_ptr<Client> new_ptr_clnt = std::make_shared<Client>(id, *this);

    this->clients[new_ptr_clnt] = 5;

    return new_ptr_clnt;

}   

std::shared_ptr<Client> Server::get_client(std::string id) const{
    for (auto& client : clients) {
        if (client.first->get_id() == id) {
            return client.first;
        }
    }
    return nullptr;
}

double Server::get_wallet(std::string id) {
    auto sp { get_client(id) };
    if (sp == nullptr) {
        return 0;
    }
    return clients[sp];
}

bool Server::parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value) {
    int p = 0;
    int idx_a[] {0, 0};

    for (int i = 0; i < trx.size(); ++i) {
        if (trx.at(i) == '_' || p >= 2 && trx.at(i) == '-') {
            throw std::runtime_error("something bad\n");
        }
        if (trx.at(i) == '-') {
            idx_a[p++] = i;
        }
    }

    sender = trx.substr(0, idx_a[0]);//从0开始，长度为idx_a[0]
    receiver = trx.substr(idx_a[0] + 1, idx_a[1] - idx_a[0] + 1);//从idx_a[0] + 1开始到idx_a[1]
    value = std::stod(trx.substr(idx_a[1] + 1));

    return true;
    
}

bool Server::add_pending_trx(std::string trx, std::string signature) {
    std::string sender {};
    std::string receiver {};
    double value = 0;

    try {
        parse_trx(trx, sender, receiver, value);
    } catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    pending_trxs.emplace_back(trx);

    if (get_wallet(sender) < value) {
        return false;
    }
    
    clients[get_client(sender)] -= value;
    clients[get_client(receiver)] += value;
    return true;


}


size_t Server::mine() {

    if (pending_trxs.size() == 0) {
        return e() % 10;
    }

    std::string mempool = "";
    for (int i = 0; i < pending_trxs.size(); i++) {
        mempool += pending_trxs[i];
    }

    std::cout << mempool << std::endl;

    std::string id = "";
    size_t n = 0;
    for (auto& [k, v] : clients) {
        auto nonce = k->generate_nonce();
        std::string str = mempool + std::to_string(nonce);
        std::string hash{crypto::sha256(str)};
        if (hash.substr(0, 10).find("000") != std::string::npos) {
            std::cout << k->get_id() << std::endl;
            v += 6.5;
            pending_trxs.clear();
            return nonce;
        }
    }
    for (int i = 0; i < 10; i++) {
        std::string str = mempool + std::to_string(i);
        std::string hash{crypto::sha256(str)};
        if (hash.substr(0, 10).find("000") != std::string::npos) {
            return i;
        }
    }
    return e() % 10;
}
