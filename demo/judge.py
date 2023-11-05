import os
import time
import shutil

def judge(config, judge, code) :
    now = int(time.time())
    prefix = 'J' + time.strftime("%Y%m%d_%H%M%S", time.localtime(now))
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
        for s in config[p[1]] :
            cmd += ' "' + s + '"'

    print()
    # print(cmd)
    print('judge =', judge)
    print('code  =', [p[0] for p in code])
    os.system(cmd)

    result = {}
    with open('score.txt', 'r') as f :
        lines = f.readlines()
        result['status'] = lines[0]
        result['score'] = []
        if len(lines)-1 != len(code) :
            raise Exception('cannot read score')
        
        for i in range(1, len(lines)) :
            if ord(lines[i][0]) != ord('A')+i-1 or lines[i][1:3] != ': ' :
                raise Exception('cannot read score')
            result['score'].append(int(lines[i][3:]))
    
    with open('details.txt', 'r') as f :
        result['details'] = ''
        for l in f.readlines() :
            if not l[3:].startswith('[Debug]') :
                result['details'] += l

    os.remove('main.cpp')
    os.remove('main')
    os.remove('interactive.cpp')
    os.remove('interactive.h')
    os.system('tail -n 1 details.txt')
    os.chdir('../..')
    print('')
    return result