#include <thread>
#include <iostream>

#include <concurrentqueue.h>

#include <clipper/datatypes.hpp>

#include "rpc_handle.hpp"

RPCHandle::RPCHandle()
    : select_queue_(std::make_shared<moodycamel::ConcurrentQueue<std::string>>()),
      send_queue_(std::make_shared<moodycamel::ConcurrentQueue<std::vector<std::string>>>()),
      context_(1), send_sock_(context_, ZMQ_PAIR), recv_sock_(context_, ZMQ_PAIR) {}

void RPCHandle::recv_thread() {
  cout << "fello" << std::endl;
  this->recv_sock_.connect("tcp://*:8080");
  cout << "fello" << std::endl;
  zmq::message_t msg;
  while (true) {
    this->recv_sock_.recv(&msg);
    cout << "Helloa" << std::endl;

    this->select_queue_->enqueue(std::string(static_cast<char*>(msg.data()), msg.size()));
  }
}

void RPCHandle::send_thread() {
  cout << "Helloa" << std::endl;
  cout << typeid((send_sock_)).name() << std::endl;
  (this->send_sock_).connect("tcp://*:8083");
  cout << "Helloa" << std::endl;
  std::vector<std::string> list_item;
  while (true) {
    bool not_empty = this->send_queue_->try_dequeue(list_item);
    cout << "Hellof" << std::endl;

    if (not_empty) {
      for (auto item = list_item.begin(); item < list_item.end(); item++) {
        zmq::message_t m(item->length());
        cout << "Hellao" << std::endl;

        memcpy((void *) m.data(), item->c_str(), item->length());
        if (item < list_item.end() - 1) {
          this->send_sock_.send(m, ZMQ_SNDMORE);
          cout << "Hellgo" << std::endl;

        } else {
          this->send_sock_.send(m);
          cout << "Hfello" << std::endl;

        }
      }
    }
  }
}

void RPCHandle::return_selection(std::vector<std::string> models) {
  send_queue_->enqueue(models);
}

void RPCHandle::start() {
  cout << "Hello" << std::endl;
  std::thread recv(&RPCHandle::recv_thread, this);
  cout << "Hello23" << std::endl;

  std::thread send(&RPCHandle::send_thread, this);
  cout << "Hello123" << std::endl;

  send.detach();
  recv.detach();
}

std::string RPCHandle::get_query() {
  std::string item;
  bool not_empty = select_queue_->try_dequeue(item);
  return item;
}