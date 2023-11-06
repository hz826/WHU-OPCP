import os
import requests
from judge import judge

config = {
    "cpp_single_gcc_std17" : ['gcc', 'g++ main.cpp -o main --std=c++17', './main'],
    "python3_single" : ['python', 'echo skip_compile', 'python main.py'],
}

url = 'http://127.0.0.1:8000'

while True :
    # get submission
    # sub = requests.get(url+'/api/gettask/', auth=('user', 'pass'))
    # if(sub.status_code != 200):
    #     continue
    # sub = sub.json()
    # subid = sub['id']
    # fileid = sub['file']
    # r = requests.get(url+'/api/download/{id}'.format(id=fileid))
    # if(r.status_code != 200):
    #     data = {'status': 'Error'}
    #     requests.post(url+'/api/submissions/{id}'.format(id=subid), data=data)
    #     continue
    # content = r.content
    # with open('judge/now.cpp', 'wb') as f:
    #     f.write(content)
    # data = {'status': 'Judging'}
    # requests.post(url+'/api/submissions/{id}'.format(id=subid))
    res = judge(config, 'judge/now.cpp', [['code/tic_tac_toe/random', "cpp_single_gcc_std17"], ['code/tic_tac_toe/py', "python3_single"]])
    print(res)
    while True:
        1