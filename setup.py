from setuptools import setup, Extension

extension_mod = Extension('board_funcs',
                          sources=[
                            'src/board_funcs/board_funcs.cpp',
                            'src/board_funcs/helpers.cpp',
                            'src/board_funcs/board_state.cpp',
                            'src/board_funcs/board_funcs_module.cpp',
                            'src/board_funcs/search.cpp',
                            'src/board_funcs/jump_table.cpp',
                            'src/board_funcs/jump_generator.cpp',
                            'src/board_funcs/asknet/asknet.cpp',
                            'src/board_funcs/little_search.cpp'],
                          include_dirs=['src/board_funcs', 'src/board_funcs/asknet'],
                          language='c++',
                          extra_compile_args=['-std=c++14', '-mavx'])

setup(
    name='board_funcs',
    ext_modules=[extension_mod],
    packages=[]
)
