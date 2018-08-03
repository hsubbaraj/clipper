from ctypes import *

class RPCHandle(Structure) :
    _fields_ = [("object", c_void_p)]

cdll.LoadLibrary("../debug/src/selectionpolicy/libselectionpolicy.dylib")

libcpp = CDLL("../debug/src/selectionpolicy/libselectionpolicy.dylib")

# print(libcpp.rpc_handle_start())

# rpchandle = cast(libcpp.rpc_handle_start(), POINTER(RPCHandle))

# print(rpchandle)

libcpp.rpc_handle_start()

print('Running lol!"!"!"!!"!')