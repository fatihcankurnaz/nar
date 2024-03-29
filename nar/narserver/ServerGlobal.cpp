#include <nar/narserver/ServerGlobal.h>
void nar::ServerGlobal::read_start() {
    read_mtx.lock();
    read_count++;
    if(read_count == 1)
        write_mtx.lock();
    read_mtx.unlock();
}

void nar::ServerGlobal::read_end() {
    read_mtx.lock();
    read_count--;
    if(read_count == 0)
        write_mtx.unlock();
    read_mtx.unlock();
}

void nar::ServerGlobal::write_start() {
    write_mtx.lock();
}
void nar::ServerGlobal::write_end() {
    write_mtx.unlock();
}


nar::ServerGlobal::ServerGlobal(std::string db_name, std::string db_user, std::string db_pass): _db_name(db_name), _db_user(db_user), _db_pass(db_pass), ctx(boost::asio::ssl::context::sslv23) {

    ctx.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);
    
    ctx.use_certificate_chain_file("server.crt"); 
    ctx.use_private_key_file("server.key", boost::asio::ssl::context::pem);
    ctx.use_tmp_dh_file("dh2048.pem");

    _db = new nar::Database();
    _db->setUser(_db_user);
    _db->setPass(_db_pass);
    _db->setDbname(_db_name);
    _db->connect();
    peers = new nar::Peers(_db);
}


string nar::ServerGlobal::get_db_user() {
    read_start();
    string db_user =  _db_user;
    read_end();
    return db_user;
}

string nar::ServerGlobal::get_db_pass() {
    read_start();
    string db_pass =  _db_pass;
    read_end();
    return db_pass;
}

string nar::ServerGlobal::get_db_name() {
    read_start();
    string db_name =  _db_name;
    read_end();
    return db_name;
}

void nar::ServerGlobal::set_db_user(string db_user) {
    write_start();
    _db_user = db_user;
    write_end();
    return;
}

void nar::ServerGlobal::set_db_pass(string db_pass) {
    write_start();
    _db_pass = db_pass;
    write_end();
    return;
}

void nar::ServerGlobal::set_db_name(string db_name) {
    write_start();
    _db_name = db_name;
    write_end();
    return;
}
nar::Database* nar::ServerGlobal::get_db() {
    read_start();
    nar::Database* db = _db;
    read_end();
    return db;
}

boost::asio::io_service& nar::ServerGlobal::get_ioserv() {
    read_start();
    boost::asio::io_service& ioserv = this->io_service;
    read_end();
    return ioserv;
}

boost::asio::ssl::context& nar::ServerGlobal::get_ctx() {
    return ctx;
}

void nar::ServerGlobal::set_randezvous_port(unsigned short port) {
    write_start();
    this->_randezvous_port = port;
    write_end();
}

unsigned short nar::ServerGlobal::get_randezvous_port() {
    read_start();
    unsigned short port = this->_randezvous_port;
    read_end();
    return port;
}

void nar::ServerGlobal::set_next_stream_id(unsigned int str_id) {
    write_start();
    this->_next_stream_id = str_id;
    write_end();
}

unsigned int nar::ServerGlobal::get_next_stream_id() {
    write_start();
    unsigned int next = this->_next_stream_id++;
    write_end();
    return next;
}
