# distributed_primes

### Koncept

- Master spawnuje kilka workerów
- Master czyta z pliku liste liczb pierwszych i kolejkuje je
- Workery oczekują na dane przekazywane przez shared memory
- Master wysyła nowe liczby dostępnym workerom
- Master komunikuje workerom o nowych zadaniach/danych poprzez pipey
- Po zakonczeniu prace workey przekazuje wynik przez shared memory i komunikuje o stanie przez pipey
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