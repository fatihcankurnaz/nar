#ifndef NAR_UTILITY_H
#define NAR_UTILITY_H
#include <string>

namespace nar {
    std::string get_string_sckt(int sockfd, int len);
    int get_int_sckt(int sockfd);
    void send_int_sckt(int sockfd, int val);
    void send_string_sckt(int sockfd, std::string str, int len);
}

#endif
