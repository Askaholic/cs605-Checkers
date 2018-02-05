from setuptools import setup, Extension

extension_mod = Extension('board_funcs',
                          sources=[
                            'src/board_funcs/board_funcs.cpp',
                            'src/board_funcs/helpers.cpp',
                            'src/board_funcs/board_state.cpp',
                            'src/board_funcs/board_funcs_module.cpp'],
                          include_dirs=['src/board_funcs'],
                          language='c++',
                          extra_compile_args=['-std=c++14'])

setup(
    name='board_funcs',
    ext_modules=[extension_mod],
    packages=[]
)
