#ifndef CPU_H
#define CPU_H
    int Cpu__run(int quanta);
    void Cpu__setIP(int index);
    bool Cpu__isBusy();
    void Cpu__cleanup();
    #define DEFAULT_QUANTA 2
#endif