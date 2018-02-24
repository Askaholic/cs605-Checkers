from setuptools import setup, Extension

extension_mod = Extension('board_funcs',
                          sources=[
                            'src/board_funcs/board_funcs.cpp',
                            'src/board_funcs/helpers.cpp',
                            'src/board/board_state.cpp',
                            'src/board/jump_table.cpp',
                            'src/board/jump_generator.cpp',
                            'src/pymodule/board_funcs_module.cpp',
                            'src/search/search.cpp',
                            'src/search/little_search.cpp',
                            'src/asknet/asknet.cpp'],
                          include_dirs=[
                            'src/board_funcs',
                            'src/asknet',
                            'src/board',
                            'src/pymodule',
                            'src/search'
                          ],
                          language='c++',
                          extra_compile_args=['-std=c++14', '-mavx'])

setup(
    name='board_funcs',
    ext_modules=[extension_mod],
    packages=[]
)
