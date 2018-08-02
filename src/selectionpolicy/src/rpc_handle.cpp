#include <thread>

#include <concurrentqueue.h>

#include <clipper/datatypes.hpp>

#include "rpc_handle.hpp"

RPCHandle::RPCHandle()
    : select_queue_(std::make_shared<moodycamel::ConcurrentQueue<std::string>>()),
      send_queue_(std::make_shared<moodycamel::ConcurrentQueue<std::vector<std::string>>>()),
      context_(1), send_sock_(context_, ZMQ_PAIR), recv_sock_(context_, ZMQ_PAIR) {}

void RPCHandle::recv_thread() {
  recv_sock_.bind("tcp://*:8080");
  zmq::message_t msg;
  while (true) {
    recv_sock_.recv(&msg);
    select_queue_->enqueue(std::string(static_cast<char*>(msg.data()), msg.size()));
  }
}

void RPCHandle::send_thread() {
  send_sock_.bind("tcp://*:8083");
  std::vector<std::string> list_item;
  while (true) {
    bool not_empty = send_queue_->try_dequeue(list_item);
    if (not_empty) {
      for (auto item = list_item.begin(); item < list_item.end(); item++) {
        zmq::message_t m(item->length());
        memcpy((void *) m.data(), item->c_str(), item->length());
        if (item < list_item.end() - 1) {
          send_sock_.send(m, ZMQ_SNDMORE);
        } else {
          send_sock_.send(m);
        }
      }
    }
  }
}

void RPCHandle::return_selection(std::vector<std::string> models) {
  send_queue_->enqueue(models);
}

void RPCHandle::start() {
  std::thread recv(&RPCHandle::recv_thread, this);
  std::thread send(&RPCHandle::send_thread, this);

  recv.join();
  send.join();
}

std::string RPCHandle::get_query() {
  std::string item;
  bool not_empty = select_queue_->try_dequeue(item);
  return item;
}