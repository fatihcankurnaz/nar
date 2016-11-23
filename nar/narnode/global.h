#ifndef NAR_GLOBAL_H
#define NAR_GLOBAL_H
#include <mutex>
#include <string>

namespace nar {
    class Global {
        private:
            std::mutex read_mtx;
            std::mutex write_mtx;
            int read_count = 0;
            std::string username;
            std::string cur_dir;
			std::string narServerIP;
			int narServerPort;
            void read_start();
            void read_end();
            void write_start();
            void write_end();
        public:
            Global() {set_curdir("~/nar"); set_narServerIp(std::string("127.0.0.1")); set_narServerPort(12345);}
            std::string get_username();
            void set_username(std::string uname);
            std::string get_curdir();
            void set_curdir(std::string cd);
			std::string get_narServerIp();
			int get_narServerPort();
			void set_narServerIp(std::string ip);
			void set_narServerPort(int port);
    };
}

#endif