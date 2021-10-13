import sys
import os
from socket import socket
import time

F = 20


def gen_message(type, pid):
    msg = '%d|' % type + str(pid) + '|'
    return msg + '0' * (F - len(msg))


def main():
    r = int(sys.argv[1])
    k = int(sys.argv[2])
    s = socket()
    pid = os.getpid()

    s.connect(('127.0.0.1', 8080))

    for i in range(r):
        s.send(gen_message(1, pid).encode())
        response = s.recv(F).decode()

        if (response == ''):
            return

        if (int(response[0]) == 3):
            print('GRANT %d' % pid)
            with open('resultado.txt', 'a') as f:
                f.write(str(time.time()) + ' ' + str(pid) + '\n')

            time.sleep(k)
            s.send(gen_message(2, pid).encode())

    s.close()


main()
