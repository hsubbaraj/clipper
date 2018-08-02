//
// Created by Rehan S. Durrani on 7/27/18.
//

#ifndef LIBCLIPPER_RPC_HANDLE_WRAPPER_H
#define LIBCLIPPER_RPC_HANDLE_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

struct rpc_handle;
typedef struct rpc_handle rpc_handle_t;

rpc_handle_t *rpc_handle_create();
void rpc_handle_destroy(rpc_handle_t *self);

void rpc_handle_start(rpc_handle_t *self);
void rpc_handle_return_selection(rpc_handle_t *self, char* models[]);

const char *rpc_handle_get_query(rpc_handle_t *self);

#ifdef __cplusplus
}
#endif

#endif //LIBCLIPPER_RPC_HANDLE_WRAPPER_H