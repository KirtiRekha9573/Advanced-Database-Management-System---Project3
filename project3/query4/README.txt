Summary:
This Makefile will create executable for building the secondary index file index.gdb and clustered index binary file clustered.bin. 
Additionally main program will be compiled to create query4 executable that uses the new binary clustered.bin for execution.


Commands to execute:
1. Compile build_index.cpp
make index

2. Create secondary index file index.gdb:
./buildindex

3. Compile main program:
make tool

4. Execute main program using clustered.bin:
./query4
