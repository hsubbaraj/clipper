import json
import time
import threading
import zmq
from collections import deque

class Sender(threading.Thread):
    def __init__(self, que, sock):
        super(Sender, self).__init__()
        self.q = que
        self.sock = sock

    def run(self):
        while True:
            if len(self.q) > 0:
                query = self.q.popleft()
                self.sock.send_json(query)

def query_generator(count):
    a = count % 2 == 0
    return {'user_id': 'rdurrani', 'query_id': count, 'query': [1, 2, 3, 4], 'msg': 'select',
                                'select_flag':a}

class QueryGen(threading.Thread):
    def __init__(self, que, qs):
        super(QueryGen, self).__init__()
        self.q = que
        self.qs = qs
        self.count = 0

    def run(self):
        while True:
            query = query_generator(self.count)
            self.qs[self.count] = json.dumps(query)
            self.count += 1
            self.q.append(query)
            time.sleep(0.05)

class Reciever(threading.Thread):
    def __init__(self, final_q, exec_q, sock):
        super(Reciever, self).__init__()
        self.fq = final_q
        self.eq = exec_q
        self.sock = sock

    def run(self):
        while True:
            query = self.sock.recv_json()
            if query['msg'] == 'exec':
                self.eq.append(query)
            elif query['msg'] == 'return':
                self.fq.append(query)

class TaskExecutor(threading.Thread):
    def __init__(self, task_queue, send_queue, q_dic):
        super(TaskExecutor, self).__init__()
        self.tq = task_queue
        self.sq = send_queue
        self.qd = q_dic

    def run(self):
        while True:
            if len(self.tq) > 0:
                mids = self.tq.popleft()
                query = json.loads(self.qd[mids['id']])
                query['preds'] = [i * 2 for i in mids['mids']]
                query['msg'] = 'combine'
                self.sq.append(query)
                self.qd[mids['id']] = query

class Client(threading.Thread):
    def __init__(self, que, queries):
        super(Client, self).__init__()
        self.q = que
        self.qd = queries

    def run(self):
        while True:
            if len(self.q) > 0:
                q = self.q.popleft()
                query = self.qd[q['id']]
                id = query['query_id']
                correct = (id % 2 == 0 and q['final_pred'] == 4) or (id % 2 == 1 and q['final_pred'] == 6)
                if not correct:
                    print('Query', id, 'FAILED!')
                del self.qd[id]

if __name__ == '__main__':
    ctx = zmq.Context()
    sel_sock = ctx.socket(zmq.PAIR)
    sel_sock.bind('tcp://*:8080')
    send_s = ctx.socket(zmq.PAIR)
    send_s.bind('tcp://*:8083')
    qs = dict()
    queries = deque()
    exec_queue = deque()
    final_queue = deque()
    qg = QueryGen(queries, qs)
    s = Sender(queries, sel_sock)
    r = Reciever(final_queue, exec_queue, send_s)
    t = TaskExecutor(exec_queue, queries, qs)
    c = Client(final_queue, qs)
    qg.start()
    s.start()
    r.start()
    t.start()
    c.start()