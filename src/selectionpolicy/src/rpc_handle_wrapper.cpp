#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include "rpc_handle_wrapper.h"
#include "rpc_handle.hpp"

struct rpc_handle {
    void *object;
};

rpc_handle_t *rpc_handle_create() {
  rpc_handle_t *wrapper;
  RPCHandle *object;

  wrapper = (rpc_handle_t *) malloc(sizeof(*wrapper));
  object = new RPCHandle();

  wrapper->object = object;

  cout << "Blank" << std::endl;

  return wrapper;
}

void rpc_handle_destroy(rpc_handle_t *self) {
  if (self == NULL) {
    return;
  }
  static_cast<RPCHandle *>(self->object)->~RPCHandle();
  free(self);
}

void rpc_handle_start(rpc_handle_t *self) {
  static_cast<RPCHandle *>(self->object)->start();
}

const char *rpc_handle_get_query(rpc_handle_t *self) {
  return static_cast<RPCHandle *>(self->object)->get_query().c_str();
}

void rpc_handle_return_selection(rpc_handle_t *self, char* models[]) {
  std::vector<std::string> mods;
  for (int i = 0; i <  sizeof(models)/sizeof(char*); ++i) {
    mods.emplace_back(models[i]);
  }
  static_cast<RPCHandle *>(self->object)->return_selection(mods);
}