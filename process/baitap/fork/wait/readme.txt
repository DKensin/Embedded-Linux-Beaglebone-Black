The following shell session demonstrates the use of program:

$ ./a.out &
Child PID is 32360
[1] 32359
$ kill -STOP 32360
stopped by signal 19
$ kill -CONT 32360
continued
$ kill -TERM 32360
killed by signal 15
[1]+  Done                    ./a.out
$

How to build
	1. type make
	2. run by type in terminal ./wait
	3. clear: make clean
