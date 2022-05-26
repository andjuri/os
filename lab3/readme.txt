PRVI TERMINAL:
andrea@DEA:/mnt/c/Users/Dea/Desktop/lab3$ echo 10 > /tmp/fifo
andrea@DEA:/mnt/c/Users/Dea/Desktop/lab3$ echo 25 > /tmp/fifo

DRUGI TERMINAL:

andrea@DEA:/mnt/c/Users/Dea/Desktop/lab3$ ./lab3 status.txt obrada.txt /tmp/fifo
Program s PID=400 krenuo s radom
upisujem broj 4, dretva 1
upisujem broj 9, dretva 6
upisujem broj 16, dretva 24
upisujem broj 121, dretva 50
upisujem broj 144, dretva 73
upisujem broj 169, dretva 67
upisujem broj 196, dretva 52
upisujem broj 225, dretva 13
upisujem broj 676, dretva 23
upisujem broj 729, dretva 82
upisujem broj 784, dretva 78
2
upisujem broj 9, dretva 56
upisujem broj 16, dretva 46
^Z
[4]+  Stopped                 ./lab3 status.txt obrada.txt /tmp/fifo
