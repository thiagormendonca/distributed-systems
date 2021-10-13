import pandas as pd
from queue import Queue

tests = [
    {
        "n": [2],
        "r": 10,
        "k": 2,
    },
    {
        "n": [2, 4, 8, 16, 32],
        "r": 10,
        "k": 2,
    },
    {
        "n": [2, 4, 8, 16, 32, 64],
        "r": 5,
        "k": 1,
    },
    {
        "n": [2, 4, 8, 16, 32, 64, 128],
        "r": 3,
        "k": 0,
    },
]


def line_number(df, n, r):
    return len(df) == n * r


def time_evolution(df):
    for i in range(len(df)):
        if (i == 0):
            continue
        # compara tempo de cada linha com linha anterior
        if (df.iloc[i]['time'] < df.iloc[i - 1]['time']):
            return False
    return True


def write_number(df, n, r):
    # conta quantas escritas foram feitas por processo
    process_number = df.groupby('pid').count()
    if (len(process_number) != n or len(process_number[process_number['time'] != r]) > 0):
        return False
    return True


def coordinator_validation(df):
    queue = Queue()
    granted = 0
    for i in range(len(df)):
        message = df.iloc[i]['message']
        pid = df.iloc[i]['pid']

        if (message == 'REQUEST'):
            queue.put(pid)  # insere na fila de REQUEST
        elif (message == 'GRANT'):
            granted = pid  # salva como último GRANT
        else:  # RELEASE
            # se mensagem não foi enviada pelo último processo que recebeu GRANT
            # ou pelo processo na cabeça da fila
            if (pid != granted or pid != queue.get()):
                return False
    return True


def main():
    for i in range(len(tests)):
        test = tests[i]

        for n in test['n']:
            result = pd.read_csv('./test_{}/resultado_{}.txt'.format(i, n),
                                 sep=' ', header=None, index_col=None, names=['time', 'pid'])

            if (not line_number(result, n, test['r'])):
                print('[FALHA] Número de linhas: teste {}; n = {};'.format(i, n))
                return

            if (not time_evolution(result)):
                print('[FALHA] Evolução do tempo: teste {}; n = {};'.format(i, n))
                return

            if (not write_number(result, n, test['r'])):
                print('[FALHA] Número de escritas: teste {}; n = {};'.format(i, n))
                return

        coordinator = pd.read_csv('./test_%d/coordinator.txt' % i,
                                  sep=' ', header=None, index_col=None, names=['time', 'pid', 'message'])

        if (not coordinator_validation(coordinator)):
            print('[FALHA] Corretude do coordenador: teste {};'.format(i, n))
            return

    print('Testes validados!')


main()
