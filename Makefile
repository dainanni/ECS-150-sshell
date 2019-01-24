



all: sshell
	gcc -Wall -Werror -o sshell sshell.c

clean: 
	rm -f sshell 
	echo Clean Complete
