#ifndef NAR_PUSHFILE_H
#define NAR_PUSHFILE_H

#include <nar/narnode/Task/ITask.h>
#include <nar/lib/Socket/Socket.h>
#include <nar/narnode/global.h>
#include <string>

#include <nar/lib/rapidjson/document.h>
#include <nar/lib/rapidjson/allocators.h>
#include <nar/lib/rapidjson/stringbuffer.h>
#include <nar/lib/rapidjson/writer.h>
#include <nar/lib/nlohJson/json.hpp>

namespace nar {
    namespace task {
        class PushFile : public nar::task::ITask {
            private:
                std::string file_path;
				nar::Socket *establishServerConnection(nar::Global* globals);
				void getJsonHeader(rapidjson::Document &header );
				//nar::Socket *establishPeerConnection( std::string peerIp, int peerPort);
				void getJsonPayload(rapidjson::Document &payload, std::string fileName, unsigned long fileSize,std::string dir);
				void sendJson(rapidjson::Document &msg,nar::Socket *serverSck );
				void recvJson(nlohmann::json &msg, nar::Socket *serverSck );
				void distributeFile(nlohmann::json &msg, nar::Socket *serverSck);
            public:
                PushFile(std::string fp): file_path(fp) { }
                void run(int unx_sockfd, nar::Global* globals);
        };
    }
}
#endif