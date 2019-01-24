# Program 1


## Overview
This program was an extremely unique assignment that provided real-world <br>
experience with designing a fully functional UNIX shell script from scratch<br>with the help of system calls.

---

## Phase 0 
This phase was mostly preliminary work that involved writing a makefile and<br>understanding the given basic implementation of the system() syscall.

`gcc -Wall -Werror -o sshell sshell.c`  

---

## Phase 1
This phase involved incorporating the code for fork()+exec()+wait() taught<br> in class into the given sample code. The implementation of this part used <br>
3 key lines of code- each for fork(), exec(), and wait():-

`pid = fork();`
`execvp(cmd[0], &cmd[0]);`
`waitpid(-1, &status, 0);`

Since input testing wasn't a part of this phase, hard coded commands (such <br>as date were used).

---

## Phase 2
This phase involved using the built in function fgets() to take command <br>   line input from the user and run it as part of the commands involved in the<br>fork()+exec()+wait() process. Since it was observed that the $PATH variable<br>was a factor that needed to be considered (especially since users are not <br> expected to type command paths into the shell input), the use of execvp() <br> was implemented. Since single commands are the only kinds of input valid in<br>this phase, a regular array was used to store commands intitally while <br>parsing the buffer input. This process was put inside an infinite loop as well <br>to ensure the process does not die unless a syscall fails. Error messages <br> are thrown to `stderr` whenever encountered while parsing. `sshell$` prints<br>itself each time the simple shell is called again because of the infinite loop.


---

## Phase 3
This phase was tricky to tackle at first- a `char*cmd[]` array of strings <br> was used as the main data structure to hold command line input as command <br> and arguments. The parser was appropriately updated and optimized to handle<br>random spaces between commands and arguments too. Command size was set as <br> 512 bytes and the array of strings had a size of 16 as specified to us too.<br>Another feature that was implemented was the return of exit status codes to<br>`stderr` as an information message as part of `+ completed ' ' [exit_code]`<br>Commands and arguments were passed to the execvp() function as part of one <br>single cmd array:-

`execvp(cmd[0], &cmd[0]);`

---

## Phase 4
This phase was interesting to implement as built-in shell commands were to <br>be emulated. `exit`, `cd` and `pwd` were implemented by performing  simple <br>`strcmp()` functions to check the parsed output and catch the command the <br> user enters. `getcwd()` was used to build the built in `pwd` shell command <br>whereas `strcat()` was used along with `chdir()` to append the filename to <br>build the full file path and to finally switch to that directory in order <br> to succesfully implement `cd`.

---

## Phase 5
This phase involved a tedious upgrade to the parser that was being used for<br>all command line input parsing work. Since the parser manually checks for <br> symbols character-by-character rather than using built-in string functions,<br>the `<` character was parsed and set a flag to true to toggle input redir <br> mode on and off. This would then prompt the creation of a file descriptor <br> that opened up a read-only stream that allowed `dup2` to redirect the input<br>- this command was parsed appropriately to neglect extra spaces between `<`<br>characters in the `fgets()` buffer.

`fileDesc = open(file, O_RDONLY);`
`dup2(fileD, 0);`


---

## Phase 6
This phase was similar to phase 5 in its implementation. Output redirection<br>now meant the `open()` function would be modified to write instead of read <br>and would have `dup2` write to `STDOUT_FILENO` instead of `STDIN_FILENO` <br>  like in Phase 5's inout redirection. Inclusion of the `O_CREAT` flag helps <br>with creating a new file if the redirected output has no specified file as <br>an argument.

`fileDesc = open(file, O_WRONLY |  O_CREAT);`
`dup2(fileD, 1);`

---

## Phase 7
This phase gave the impression that pipe implementation needed a split of <br> commands into various different fragments inthe sense that instead of there<br>being a parent and a child process, there would now be the shell as the one<br>parent and the piped commands as the 2 children running separately with the<br>`execvp()` fucntion. 

This version of the simple shell in its current state cannot implement the <br>pipeline of commands.

---

## Phase 8
This phase was not complete or implemented at all within this simple shell.

---


## Testing
This project was tested using ~less~ *(a lot!)* of `gdb`. The given tester <br>was also used to check program validity and experiment with different test <br>cases- `sshell_tester`.

---


### Sources
[Fork+Exec+Wait] (https://canvas.ucdavis.edu/courses/300146/files/folder/lectures?preview=5045952)

[Markdown] (https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)

[Errors] (https://www.gnu.org/software/libc/manual/html_node/Error-Messages.html)

[Memory Allocation and string functions] (https://www.gnu.org/software/libc/manual/html_mono/libc.html#String-and-Array-Utilities)

[GDB debugging] (https://www.thegeekstuff.com/2010/03/debug-c-program-using-gdb/)

[String Input] (https://www.geeksforgeeks.org/fgets-gets-c-language/)

[File redirection] (http://codewiki.wikidot.com/c:system-calls:open)












 
