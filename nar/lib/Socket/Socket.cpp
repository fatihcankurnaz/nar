#include "Socket.h"
#include <string>
#include <iostream>
//#include <nar/lib/Exception/Exception.h>
#include "../Exception/Exception.h"
using std::cout;
using std::endl;

nar::Socket::Socket(boost::asio::io_service& io_serv, char type) {
    _type = type;
    if(type == 'c') {
        _sock = new boost::asio::ip::tcp::socket(io_serv);
    } else if(type == 's') {
        _acceptor = new boost::asio::ip::tcp::acceptor(io_serv);
    } else {
        throw nar::Exception::Socket::UnknownType("Unknown type in Socket constructor", type);
    }
}

nar::Socket::~Socket() {
    if(_type == 'c') {
        delete _sock;
    } else if(_type == 's') {
        delete _acceptor;
    } else {
        throw nar::Exception::Socket::UnknownType("Unknown type in Socket destructor.", _type);
    }
}

void nar::Socket::bind(const unsigned short port, const char* interface) {             // NOT DONE
    if(_type == 'c') {
        _sock->open(boost::asio::ip::tcp::v4());
        boost::system::error_code ec;
        _sock->bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(interface), port), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Client Socket::bind", port, _type);
        }
    } else if(_type == 's') {
        boost::system::error_code ec;
        _acceptor->open(boost::asio::ip::tcp::v4(), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", port, _type);
        }
        _acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(interface), port);
        _acceptor->bind(endpoint, ec);    
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", port, _type);
        }
        _acceptor->listen();
    } else {
        throw nar::Exception::Socket::UnknownType("Unknown type in Socket::bind.", _type);
    }
}

void nar::Socket::bind(const unsigned short port) {
    if(_type == 'c') {
        _sock->open(boost::asio::ip::tcp::v4());
        boost::system::error_code ec;
        _sock->bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Client Socket::bind", port, _type);
        }
    } else if(_type == 's') {
        boost::system::error_code ec;
        _acceptor->open(boost::asio::ip::tcp::v4(), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", port, _type);
        }
        _acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        _acceptor->bind(endpoint, ec);    
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", port, _type);
        }
        _acceptor->listen();
    } else {
        throw nar::Exception::Socket::UnknownType("Unknown type in Socket::bind.", _type);
    }
}

void nar::Socket::bind() {
     if(_type == 'c') {
        _sock->open(boost::asio::ip::tcp::v4());
        boost::system::error_code ec;
        _sock->bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Client Socket::bind", 0, _type);
        }
    } else if(_type == 's') {
        boost::system::error_code ec;
        _acceptor->open(boost::asio::ip::tcp::v4(), ec);
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", 0, _type);
        }
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 0);
        _acceptor->bind(endpoint, ec);        
        if(ec) {
            throw nar::Exception::Socket::BindError("Error on Server Socket::bind", 0, _type);
        }
         
        _acceptor->listen();
    } else {
        throw nar::Exception::Socket::UnknownType("Unknown type in Socket::bind.", _type);
    }
}

void nar::Socket::accept(nar::Socket& acc) const {
    if(_type == 'c') {
        throw nar::Exception::Socket::WrongSocketType("Accept call on a client Socket",_type); 
    }
    if(acc._type == 's') {
        throw nar::Exception::Socket::WrongSocketType("Server socket is given as an argument to a accept call",_type); 
    }
    boost::asio::ip::tcp::endpoint endpoint  = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345);
    _acceptor->accept(*acc._sock,endpoint);
}

void nar::Socket::connect(const std::string& host, const unsigned short port) {
    if(_type == 's') {
        throw nar::Exception::Socket::WrongSocketType("Connect call on a server Socket",_type); 
    }

    boost::system::error_code ec = boost::asio::error::host_not_found;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    _sock->connect(endpoint,ec);
    if(ec) {
        throw nar::Exception::Socket::ConnectionError("Error on a connect call", host, port, _type);
    }
}

void nar::Socket::send(const char* offset, int length) const {
    if(_type == 's') {
        throw nar::Exception::Socket::WrongSocketType("Send call on a server Socket",_type); 
    }
    boost::system::error_code ec;
    boost::asio::write(*_sock, boost::asio::buffer((void *)offset, length), ec);
    if(ec) {
        throw nar::Exception::Socket::SystemError("System error from socket->send");    
    }
}

int nar::Socket::recv(char* offset, int length) const {
    if(_type == 's') {
        throw nar::Exception::Socket::WrongSocketType("Recv call on a server Socket",_type); 
    }
    boost::system::error_code ec;
    size_t len = _sock->read_some(boost::asio::buffer(offset,length), ec);
    if(ec) {
        throw nar::Exception::Socket::SystemError("System error from socket->recv");    
    }
    return len;
}

void nar::Socket::close() const {
    boost::system::error_code ec;
    _sock->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _sock->close(ec);
    // An error may occur from 2 oprations above.Doesnt matter closes the socket anyway.
}

std::string nar::Socket::get_dest_ip() const {
    return _sock->remote_endpoint().address().to_string();
}

unsigned short nar::Socket::get_local_port() const {
    return _acceptor->local_endpoint().port();
}
