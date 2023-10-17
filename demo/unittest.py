import os
import time
import shutil

def run(judge, config) :
    now = int(time.time())
    prefix = 'J' + time.strftime("%H%M%S", time.localtime(now))
    prefix_folder = prefix + '/'
    os.mkdir(prefix_folder)
    shutil.copy(judge, prefix_folder+'main.cpp')
    shutil.copy('interactive.cpp', prefix_folder)
    shutil.copy('interactive.h', prefix_folder)
    os.chdir(prefix_folder)
    os.system('g++ main.cpp interactive.cpp -o main -std=c++11 -Wall')

    cmd = './main ' + prefix
    for p in config :
        for s in p :
            cmd += ' "' + s + '"'
    print(cmd)
    os.system(cmd)
    os.remove('main.cpp')
    os.remove('main')
    os.remove('interactive.cpp')
    os.remove('interactive.h')
    os.chdir('..')

run('judge/tic_tac_toe.cpp', [['../code/tic_tac_toe/A', 'g++ main.cpp -o main', './main'], ['../code/tic_tac_toe/A', 'g++ main.cpp -o main', './main']])