# Передача информации между процессами

## System V IPC
Интерфейс межпроцессного взаимодействия System V IPC позволяет манипулировать классическими набором объектов, таких как: очередь сообщений, семафор и разделяемая память (память для совместного использования процессами).

## ftok()
```
#include <sys/types.h>
#include <sys/ipc.h>

key_t frok (char* pathname, char proj)
```
**Parametrs**: 

    [pathname] - указатель на имя файла, доступного для процесса; 
    [proj] - число, характеризующее экземпляр средства связи

## Дескриптор средства связи 

Ядро хранит информацию обо всех средствах System V IPC, исопльзуемых в системе, вне контекстов пользовательских процессов.

Дескприптор средства связи (неотрицательное число) - характеризует средство связи. Ядро оперирует со средством связи через дескриптор.

## Shared memory

ОС предоставляет процессам сегмент памяти для совместного пользования. 

## shmget() - возвращает дескприптор запрашиваемого сегмента памяти по параметрам 

```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.>

int shmget(key_t key, int size, int shmflg);
```
**Parametrs:** 

    [key] = (ftok() or IPC_PRIVATE - new key that has not used before), 
    [size] - size of memory in bytes (if size is not correct for segment by key, then failure occur),
    [shmflg] - flags of shm: rotes of using, IPC_CREAT (create a segment), IPC_EXCL - failure in case existing of segment; IGNORED IN CASE WITH IPC_PRIVATE

## shmat() - прикрепление средства связи к процессу. Включает сегмент в аресное пространство
```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

char* shmat(int shmid, char* shmaddr, int shmflg);
```
**Parametries:**

    [shmid] - descriptor of segment of memory;
    [shmaddr] - address to attempted memory 
    [shmflg] - 0 (read and write); SHM_RDONLY - only reading

## shmdt() - исключение сегмента из адресного пространства
```
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int shmdt(char* shmaddr);
```  

![Thomas Shelby SHMEM](./Peaky_Tommy_Shelby.webp)
