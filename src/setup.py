from distutils.core import setup, Extension

extension_mod = Extension('board_funcs',
                          sources=[
                            'board_funcs_module.cpp',
                            'board_funcs.cpp'],
                          extra_compile_args=['-std=c++14'])

setup(name='board_funcs', ext_modules=[extension_mod])
