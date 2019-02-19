@echo off
g++ -c list.cpp rec.cpp main.cpp
g++ -o list.exe list.o rec.o main.o
del *.o
