#ifndef __RPC_HANDLE_H__
#define __RPC_HANDLE_H__

#include <string>
#include <vector>

#include <concurrentqueue.h>

#include <clipper/datatypes.hpp>

#include "zmq.hpp"

using namespace std;

class RPCHandle {
public:
    ~RPCHandle() = default;

    RPCHandle();

    void start();
    void recv_thread();
    void send_thread();
    void return_selection(std::vector<std::string> models);

    std::string get_query();

private:
    shared_ptr<moodycamel::ConcurrentQueue<std::string>> select_queue_;
//    shared_ptr<moodycamel::ConcurrentQueue<clipper::Query>> select_feedback_queue;
//    shared_ptr<moodycamel::ConcurrentQueue<clipper::Query>> combine_queue;
//    shared_ptr<moodycamel::ConcurrentQueue<clipper::Query>> update_state_queue;
    shared_ptr<moodycamel::ConcurrentQueue<std::vector<std::string>>> send_queue_;
    zmq::context_t context_;
    zmq::socket_t send_sock_;
    zmq::socket_t recv_sock_;
};

#endif // __RPC_HANDLE_H__