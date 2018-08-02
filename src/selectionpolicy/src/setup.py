from distutils.core import setup, Extension

# the c++ extension module
extension_mod = Extension("rpc", ["rpcmodule.c", "rpc_handle_wrapper.cpp"], include_dirs=['../../libs/concurrent_queue']
                          , extra_compile_args = ["-stdlib=libc++"])

setup(name = "rpc", ext_modules=[extension_mod])