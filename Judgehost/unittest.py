import os
from judge import judge

config = {
    "cpp_single_gcc_std17" : ['gcc', 'g++ main.cpp -o main --std=c++17', './main'],
    "python3_single" : ['python', 'echo skip_compile', 'python main.py'],
}

os.system('rm -rf run/')
print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/py', "python3_single"]]))
print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/mle', "cpp_single_gcc_std17"]]))
print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/tle', "cpp_single_gcc_std17"]]))
# print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/best', "cpp_single_gcc_std17"]]))
# print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"]]))
# print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/re1', "cpp_single_gcc_std17"]]))
# print(judge(config, 'judge/tic_tac_toe.cpp', [['code/unittest/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/unittest/tic_tac_toe/hack1', "cpp_single_gcc_std17"]]))
