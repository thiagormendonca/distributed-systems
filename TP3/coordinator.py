from threading import Lock, Thread
import time
from queue import Queue
import socket

mutex = Lock()
queue = Queue()
F = 20
granted = {}


def grant(pid, client):
    client.send(('3|' + '0' * (F - 2)).encode())
    with open('coordinator.txt', 'a') as out:
        out.write(str(time.time()) + ' ' + str(pid) + ' GRANT\n')

    if pid in granted:
        granted[pid] += 1
    else:
        granted[pid] = 1


def request(pid, client):
    mutex.acquire()
    with open('coordinator.txt', 'a') as out:
        out.write(str(time.time()) + ' ' + str(pid) + ' REQUEST\n')

    if (queue.empty()):
        grant(pid, client)

    queue.put((pid, client))

    mutex.release()


def release(pid):
    mutex.acquire()
    with open('coordinator.txt', 'a') as out:
        out.write(str(time.time()) + ' ' + str(pid) + ' RELEASE\n')

    queue.get()

    if (not queue.empty()):
        (pid, client) = queue.queue[0]
        grant(pid, client)

    mutex.release()


def listener():
    s = socket.socket()
    s.bind(('', 8080))
    s.listen(5)

    while True:
        c, addr = s.accept()
        Thread(target=connection, args=(c,), daemon=True).start()


def connection(client):
    while True:
        buffer = client.recv(F).decode()
        if (buffer == ''):
            break
        msg, pid, zeros = buffer.split('|')

        if (int(msg) == 1):  # REQUEST
            request(pid, client)
        elif (int(msg) == 2):  # RELEASE
            release(pid)


def terminal():
    while True:
        command = int(input())

        if (command == 1):
            mutex.acquire()
            print(queue.queue)
            mutex.release()
        elif (command == 2):
            print(granted)
        elif (command == 3):
            break
        else:
            print('Comando inválido')


def main():
    Thread(target=listener, daemon=True).start()
    terminal()


main()
