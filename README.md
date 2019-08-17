# Parallelize
C library that a user can use to parallelize their C code.  Supports both the farm and pipeline patterns parallel patterns.
## Running
I have included a make file to compile my library and example code.
• Step 1: To compile the library run make parapat
• Step 2: To compile the example run make example
• Step 3: To run the example use ./example bear in mind that the tests may take a minute or so to complete.
• Step 4: To clean the directory run make clean
If you wish to make changes to the number of workers or tasks go into the para- pat.h file and change the relevant marcos then clean and recompile the example. If you wish to add the library into a different example, include parapat.h and compile using gcc -L./ yourexample.c -lparapat -o desiredoutput
