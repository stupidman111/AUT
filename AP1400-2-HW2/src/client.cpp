#include "client.h"
#include "crypto.h"
#include <random>

Client::Client(std::string id, const Server& server) : id(id),  server(&server) {
    crypto::generate_key(this->public_key, this->private_key);
}

std::string Client::get_id() {
    return this->id;
}

std::string Client::get_publickey() const{
    return this->public_key;
}

double Client::get_wallet() {
    /* 
        因为Server类中的get_wallet是非const函数，而Client中的server指针是指向常量的指针，
        而指向常量的类指针只能使用其const成员函数，因此下面注释掉的写法是不对的。
    */
    //return server->get_wallet(this->get_id());

    return (const_cast<Server*>(server))->get_wallet(id);

}



std::string Client::sign(std::string txt) const{
    return crypto::signMessage(this->private_key, txt);
}




// bool Client::transfer_money(std::string receiver, double value) {
//     //接受者需要存在？

//     std::string trx { this->id + "-" + receiver + "-" + std::to_string(value) };
//     std::string signature { sign(trx) };
//     if (!(const_cast<Server*>(server)->add_pending_trx(trx, signature))) {
//         return false;
//     }

//     return true;
// }

std::string clean(double value) {
    std::string fakeString = std::to_string(value);
    if (fakeString.find(".") == std::string::npos) {
        return fakeString;
    }

    int i = fakeString.length() - 1;
    while (fakeString.at(i) == '0' && i >= fakeString.find(".")) {
        i--;
    }
    if (fakeString.at(i) == '.') {
        return fakeString.substr(0, i);
    }
    return fakeString.substr(0, i + 1);
}

bool Client::transfer_money(std::string receiver, double value) {
    // delete useless 0

    std::string transaction = this->get_id() + "-" + receiver + "-" + clean(value);
    std::string signature = sign(this->private_key);
    if (!((Server*)(this->server))->add_pending_trx(transaction, signature))
        return false;
    return true;
}

size_t Client::generate_nonce() {
    return e() % 10;
}   



