#include "UserRegister.h"
std::string& nar::MessageTypes::UserRegister::Request::get_username(){
    return this->_username;
}
std::string& nar::MessageTypes::UserRegister::Request::get_aes_crypted(){
    return this->_aes_crypted;
}
std::string& nar::MessageTypes::UserRegister::Request::get_rsa_pub(){
    return this->_rsa_pub;
}
std::string& nar::MessageTypes::UserRegister::Request::get_rsa_pri_crypted(){
    return this->_rsa_pri_crypted;
}
void nar::MessageTypes::UserRegister::Request::send_mess(nar::Socket* skt, nar::MessageTypes::UserRegister::Response & resp) {
    nlohmann::json usrreg_req_send;
    usrreg_req_send["header"] = send_head();
    usrreg_req_send["payload"]["username"] = this->_username;
    usrreg_req_send["payload"]["aes_crypted"] = this->_aes_crypted;
    usrreg_req_send["payload"]["rsa_pub"] = this->_rsa_pub;
    usrreg_req_send["payload"]["rsa_pri_crypted"] = this->_rsa_pri_crypted;
    send_message(skt,usrreg_req_send.dump());
    std::string temp = get_message(skt);
    nlohmann::json usrreg_req_recv = nlohmann::json::parse(temp);
    resp.receive_message(usrreg_req_recv);
    return;

}
nlohmann::json nar::MessageTypes::UserRegister::Request::test_json() {
    nlohmann::json usrreg_req_test;
    return usrreg_req_test;
}


void nar::MessageTypes::UserRegister::Request::receive_message(std::string& recv_jsn){
    auto usrreg_req_recv = nlohmann::json::parse(recv_jsn);
    nlohmann::json head = usrreg_req_recv["header"];
    recv_fill(head);
    this->_username = usrreg_req_recv["payload"]["username"];
    this->_aes_crypted = usrreg_req_recv["payload"]["aes_crypted"];
    this->_rsa_pub = usrreg_req_recv["payload"]["rsa_pub"];
    this->_rsa_pri_crypted = usrreg_req_recv["payload"]["rsa_pri_crypted"];
    if((this->_username=="")||(this->_aes_crypted=="")||(this->_rsa_pub=="")||(this->_rsa_pri_crypted=="")){
        throw nar::Exception::MessageTypes::BadMessageReceive("user register message receive can not have empty items");
    }
    return;
}
void nar::MessageTypes::UserRegister::Response::send_mess(nar::Socket* skt) {
    nlohmann::json usrreg_resp_send;
    usrreg_resp_send["header"] = send_head();
    send_message(skt,usrreg_resp_send.dump());
    return;
}

void nar::MessageTypes::UserRegister::Response::receive_message(nlohmann::json usrreg_resp_recv){
    nlohmann::json head = usrreg_resp_recv["header"];
    recv_fill(head);
    if(_status_code == 300) {
        throw nar::Exception::MessageTypes::UserNameAlreadyExist("User name already exists", _status_code);
    } else  if(_status_code == 400) {
        throw nar::Exception::MessageTypes::UserNameAlreadyExist("User name already exists", _status_code);
    }
    return;
}
nlohmann::json nar::MessageTypes::UserRegister::Response::test_json() {
    nlohmann::json usrreg_resp_test;
    usrreg_resp_test["header"] = send_head();
    return usrreg_resp_test;
}
