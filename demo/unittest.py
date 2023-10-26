import os
import time
import shutil

config = {
    "cpp_single_gcc_std17" : ['g++ main.cpp -o main --std=c++17', './main']
}

def run(judge, code) :
    now = int(time.time())
    prefix = 'J' + time.strftime("%H%M%S", time.localtime(now))
    prefix_folder = 'run/' + prefix + '/'

    if not os.path.exists('run') :
        os.mkdir('run')
    
    os.mkdir(prefix_folder)
    shutil.copy(judge, prefix_folder+'main.cpp')
    shutil.copy('interactive.cpp', prefix_folder)
    shutil.copy('interactive.h', prefix_folder)
    os.chdir(prefix_folder)
    os.system('g++ main.cpp interactive.cpp -o main -std=c++11 -Wall')

    cmd = './main ' + prefix
    for p in code :
        cmd += ' "' + '../../' + p[0] + '"'
        cmd += ' "' + config[p[1]][0] + '"'
        cmd += ' "' + config[p[1]][1] + '"'

    # print(cmd)
    print('judge =', judge)
    print('code  =', [p[0] for p in code])
    os.system(cmd)
    os.remove('main.cpp')
    os.remove('main')
    os.remove('interactive.cpp')
    os.remove('interactive.h')
    os.system('tail -n 1 details.txt')
    os.chdir('../..')
    print('')

os.system('rm -rf run/')
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/best', "cpp_single_gcc_std17"]])
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/random', "cpp_single_gcc_std17"]])
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/mle', "cpp_single_gcc_std17"]])
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/tle', "cpp_single_gcc_std17"]])
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/re1', "cpp_single_gcc_std17"]])
run('judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/hack1', "cpp_single_gcc_std17"]])