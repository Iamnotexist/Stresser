g++ stress.cpp -c -o stress.o
g++ lib\provod2.cpp -c -o lib\provod2.o
g++ -o stress.exe stress.o lib\provod2.o
del /Q lib\provod2.o
del /Q stress.o
g++ compile\standart_checker.cpp -o compile\standart_checker.exe