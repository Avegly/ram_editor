windres resources.rc -O coff -o resources.o
g++ main.cpp resources.o -o AveglyRamEditor.exe -mwindows -lcomctl32
pause