from distutils.core import setup, Extension

# the c++ extension module
extension_mod = Extension("rpc", ["rpcmodule.c", "rpc_handle_wrapper.cpp"])

setup(name = "rpc", ext_modules=[extension_mod])