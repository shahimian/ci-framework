@echo off
g++ -c rec.cpp main.cpp
g++ -o main.exe rec.o main.o
del *.o
main.exe
