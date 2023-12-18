import os
import requests
from time import sleep
from judge import judge

config = {
    "cpp_single_gcc_std17" : ['gcc', 'g++ main.cpp -o main --std=c++17', './main'],
    "python3_single" : ['python', 'echo skip_compile', 'python main.py'],
}

url = 'http://10.255.54.33:8000/'

while True :
    sleep(1)
    jud = requests.get(url+'/api/gettask/', auth=('user', 'pass'))
    if(jud.status_code != 200):
        continue
    jud = jud.json()
    jud_id = jud['id']
    sub_id = jud['submission']
    sub = requests.get(url+'/api/submissions/{id}'.format(id=sub_id))
    sub = sub.json()
    contest_id = sub['contest']
    user1_id = sub['user']
    file1_id = sub['file']
    user2_id = jud['user2']
    file2_id = jud['file2']
    contest = requests.get(url+'/api/contests/{id}'.format(id=contest_id))
    contest = contest.json()
    judger_id = contest['judger']

    if not os.path.exists('judge/player1'):
        os.mkdir('judge/player1')
    r = requests.get(url+'/api/download/{id}'.format(id=file1_id))
    if(r.status_code != 200):
        data = {'status': 'Error'}
        requests.patch(url+'/api/submissions/{id}'.format(id=sub_id), data=data)
        continue
    content = r.content
    with open('judge/player1/main.cpp', 'wb') as f:
        f.write(content)


    if not os.path.exists('judge/player2'):
        os.mkdir('judge/player2')
    r = requests.get(url+'/api/download/{id}'.format(id=file2_id))
    if(r.status_code != 200):
        data = {'status': 'Error'}
        requests.patch(url+'/api/judges/{id}'.format(id=jud_id), data=data)
        continue
    content = r.content
    with open('judge/player2/main.cpp', 'wb') as f:
        f.write(content)

    r = requests.get(url+'/api/download/{id}'.format(id=judger_id))
    if(r.status_code != 200):
        data = {'status': 'Error'}
        requests.patch(url+'/api/submissions/{id}'.format(id=sub_id), data=data)
        continue
    content = r.content
    with open('judge/judger.cpp', 'wb') as f:
        f.write(content)
    data = {'status': 'Judging'}
    requests.patch(url+'/api/submissions/{id}'.format(id=sub_id), data=data)
    res = judge(config, 'judge/judger.cpp', [['judge/player1', "cpp_single_gcc_std17"], ['judge/player2', "cpp_single_gcc_std17"]])
    print(res)
