#ifndef NAR_PULLFILE_H
#define NAR_PULLFILE_H

#include <nar/narnode/Task/ITask.h>
#include <nar/narnode/global.h>
#include <nar/lib/Socket/Socket.h>
#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/lib/nlohJson/json.hpp>
#include <string>


namespace nar {
    namespace task {
        class PullFile : public nar::task::ITask {
            private:
                std::string file_name;
                unsigned long file_size;


            public:
                void initialize();
                void constructRequestJson(nlohmann::json &j, std::string dirname);
                nar::Socket* connectToServer(nar::Global* globals);
                void sendRequestJson(nlohmann::json &j, nar::Socket *server_sck);
                void getResultJson(nlohmann::json &j_resp, nar::Socket *serverSck);
                void comeTogether(nlohmann::json j_resp, nar::Socket *serverSck);
                nar::Socket* connectToPeer(std::string peer_ip, int peer_port);
                void constructJsonforPeer(nlohmann::json &j, std::string dirname);

                PullFile(std::string fn): file_name(fn) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}
#endif