from setuptools import setup, Extension

extension_mod = Extension('board_funcs',
                          sources=[
                            'src/board_funcs/board_funcs.cpp',
                            'src/board_funcs/helpers.cpp',
                            'src/board/board_state.cpp',
                            'src/board/piece_count.cpp',
                            'src/board/moves/jump_table.cpp',
                            'src/board/moves/jump_generator.cpp',
                            'src/pymodule/checkers605_module.cpp',
                            'src/game/game.cpp',
                            'src/game/player.cpp',
                            'src/search/search.cpp',
                            'src/asknet/asknet.cpp',
                            'src/asknet/asknet4.cpp'],
                          include_dirs=[
                            'src/board_funcs',
                            'src/asknet',
                            'src/board',
                            'src/board/moves',
                            'src/pymodule',
                            'src/game',
                            'src/search'
                          ],
                          language='c++',
                          extra_compile_args=['-std=c++14', '-mavx'])

setup(
    name='board_funcs',
    ext_modules=[extension_mod],
    packages=[]
)
