#include <nar/narnode/ActiveTask/ActiveTask.h>
#include <nar/narnode/utility.h>
#include <nar/lib/Messaging/MessageTypes/IPCMkdir.h>
#include <nar/lib/Messaging/MessageTypes/Mkdir.h>
using std::cout;
using std::endl;
using std::string;
/*
 * @author : Fatih
 * @todo : try catch  at send messages
*/

void nar::ActiveTask::Mkdir::run(nar::Socket* ipc_socket, nar::MessageTypes::IPCMkdir::Request* req) {
    nar::Socket* server_sck = this->_globals->establish_server_connection();
    string dest_dir = req->get_dest_dir();
    string dir_name = req->get_dir_name();
    if(dest_dir.empty()) {
        dest_dir = _vars->get_current_directory();
    }
    try {
        nar::ActiveTask::user_authenticate(server_sck, this->_vars);
    } catch (nar::Exception::Daemon::AuthenticationError exp) {
        std::cout<<exp.what()<<std::endl;
        return;
    }
    nar::MessageTypes::Mkdir::Request mkdir_req(dir_name,dest_dir);
    nar::MessageTypes::Mkdir::Response mdkir_resp;
    try{
        mkdir_req.send_mess(server_sck,mdkir_resp);
    }
    catch(...){
        return;
    }
    nar::MessageTypes::IPCMkdir::Response resp;
    resp.send_message_end(ipc_socket);

}