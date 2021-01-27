# distributed_primes

### Koncept

- Master spawnuje kilka workerów
- Master czyta z pliku liste liczb pierwszych i kolejkuje je
- Workery oczekują na dane przekazywane przez shared memory
- Master wysyła nowe liczby dostępnym workerom
- Master komunikuje workerom o nowych zadaniach/danych poprzez sockety unixowe
- Po zakonczeniu prace workey przekazuje wynik przez shared memory i komunikuje o stanie przez sockety unixowe
- Master agreguje wyniki i wyświetla rezultat

### TODO:

#### Worker:
- [ ] Funkcja zarzadzajaca komunikacja z masterem  
- [ ] Funkcja sprawdzająca czy liczba jest pierwszą
#### Master:
- [ ] Funkcja spawnująca workery 
- [ ] Funkcja zarzadzajaca komunikacja z workerami 



### Ciekawe linki:

- [Proste wyjaśnienie działania pamięci współdzielonej](https://www.youtube.com/watch?v=rPV6b8BUwxM)
- [Mapowanie plików z dysku do pamięci virtualnej](https://www.youtube.com/watch?v=m7E9piHcfr4)
- [Rozwiązanie problemu pipeów](https://stackoverflow.com/questions/53032459/use-two-pipes-in-c-or-one-pipe-for-more-than-2-read-writes-and-how)
- [Wyjaśnienie poll()](https://man7.org/linux/man-pages/man2/poll.2.html)
- [poll() vs. select()](https://stackoverflow.com/questions/970979/what-are-the-differences-between-poll-and-select)