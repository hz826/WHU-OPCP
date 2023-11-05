import os
import requests
from judge import judge

config = {
    "cpp_single_gcc_std17" : ['gcc', 'g++ main.cpp -o main --std=c++17', './main'],
    "python3_single" : ['python', 'echo skip_compile', 'python main.py'],
}

url = 'http://127.0.0.1'

while True :
    # get submission
    r = requests.get(url+'/user', auth=('user', 'pass'))
    print(r.json())

    # judge(config, 'judge/tic_tac_toe.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/py', "python3_single"]])
    