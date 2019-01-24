#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>




int redirection(char*cmd[16], char*file, int redir)
{
	int fileDesc;

        if(redir == 1)
        {
		fileDesc = open(file, O_RDONLY);
                //if((fileDesc = open(file, O_RDONLY)) <0) //Opens file and file descriptor for read
                //{
                //        fprintf(stderr, "Error: cannot open input file\n"); //Input file command error message
                //        exit(1);
               // }

        }

	else if(redir == 0)
	{
                if((fileDesc = open(file, O_WRONLY |  O_CREAT)) <0) //Opens file and file descriptor for write
                {
                        fprintf(stderr, "Error: cannot open output file\n"); //Output file command error message
                        exit(1);
                }

	}

	return fileDesc;
}




int forkExecWait(char*cmd[16],char*file, int redir)
{
	pid_t procID;
	int status;
	int fileD;//file redirection file descriptor


	fileD = redirection(cmd, file, redir);

	procID = fork();
	
	

        if(procID == 0)
	{
		//CHILD PROCESS
	
		if(redir == 1)
		{
		//	int fileDesc;
	       	//	fileDesc = redirection(cmd, file, redir); //Creates file desc and checks for errors
			dup2(fileD, 0); //Duplicates file desc and redirects input from stdin to fileDesc
		}	

		else if(redir == 0)
		{
		//	int fileDesc;
                //       fileDesc = redirection(cmd, file, redir); //Creates file desc and checks for errors
                        dup2(fileD, 1); //Duplicates file desc and redirects input from stdin to fileDesc
		}

                execvp(cmd[0], &cmd[0]);
		
		//Executes the error statement only if execvp fails
                fprintf(stderr, "Error: command not found\n"); //Exec command error message
                exit(1);
        }
        else if(procID > 0)
	{
        //PARENT PROCESS
                waitpid(-1, &status, 0);
             // printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
        else
	{
                perror("fork");
                exit(1);
        }

	return WEXITSTATUS(status);

}




int  builtInCmd(char*input[16], int flag, int *status)
{
	char curDir[PATH_MAX];

	if(!strcmp(input[0], "exit"))//Built in shell exit command
	{
		fprintf(stderr, "Bye...\n");//Exit message
		exit(0);
	}

	else if(!strcmp(input[0], "cd"))//Built in shell cd command
	{
		getcwd(curDir, sizeof(curDir));//Calculates current working directory
		strcat(curDir,"/");//Concatenates / to the current working directory
		strcat(curDir, input[1]);//Adds desired directory name to working directory path

		if((chdir(curDir)) < 0)// Changes current working directory		
		{
			fprintf(stderr, "Error: no such directory\n");
			*status = 1;
		}

		else
			*status = 0;
		flag = 1;
	}

	else if(!strcmp(input[0], "pwd"))//Built in shell pwd command
	{
		getcwd(curDir, sizeof(curDir));//Calculates current working directory
		printf("%s\n", curDir);		
		flag = 1;
		*status = WEXITSTATUS(*status);

	}

	return flag;
}






int main(int argc, char *argv[])
{
	int flag = 0;
	int redir = -1; //Redirection flag
	
	while(1)
	{

	fprintf(stdout, "sshell$ ");
	char buffer[512];
	fgets(buffer, 512, stdin);

	/*
     	* Echoes command line to stdout if fgets read from a file and not
     	* the terminal (which is the case with the test script)
     	*/
	if (!isatty(STDIN_FILENO))
	{
		printf("%s", buffer);
		fflush(stdout);
	}


	char *cmd[16];

	for(int i = 0; i<16; i++)
		cmd[i] = (char*)malloc(sizeof(char[512])); //Allocating memory iteratively to array of strings

	char *temp = (char*)malloc(sizeof(char[512])); //Allocating memory to temporary string
	char *file = (char*)malloc(sizeof(char[512])); //Allocating memory to hold filename
	int tempIndex = 0;
	int cmdIndex = 0;
	int fileIndex = 0;
	size_t bufLen = strlen(buffer);

	if(bufLen > 0 && buffer[bufLen - 1] == '\n') //Gets rid of trailing newline character from fgets() input
		buffer[bufLen - 1] = '\0';

	for(int n=0; n<=bufLen; n++)
	{
		
		if(buffer[n] == ' ')
		{
			if(buffer[n-1] == ' ')
				continue;	
			
			else if(buffer[n-1] == '<')
				continue;
			
			else if(redir == 1 || redir == 0)
				redir = -1; //resets redirection mode

			else
			{
			temp[tempIndex] = '\0';
			strcpy(cmd[cmdIndex++], temp);// Command/Arg  copied
			memset(temp, 0, sizeof(char[512])); //Clears mallocated temp string
			tempIndex = 0;
			}
		}
		
		else if(buffer[n] == '<')
		{
			redir = 1; //toggles input redirection mode on

			if(buffer[n-1] == ' ')
				continue;

			else
			{
				temp[tempIndex] = '\0';
                        	strcpy(cmd[cmdIndex++], temp);// Command/Arg  copied
                        	memset(temp, 0, sizeof(char[512])); //Clears mallocated temp string
                        	tempIndex = 0;
                        }

		}

		else if(buffer[n] == '>')
                {
                        redir = 0; //toggles output redirection mode on

                        if(buffer[n-1] == ' ')
                                continue;

                        else
                        {
                                temp[tempIndex] = '\0';
                                strcpy(cmd[cmdIndex++], temp);// Command/Arg  copied
                                memset(temp, 0, sizeof(char[512])); //Clears mallocated temp string
                                tempIndex = 0;
                        }

                }

		else if(buffer[n] == '&')
			fprintf(stderr, "Error: invalid command line\n"); //& reserved char error message


		else if(buffer[n] == '\0') //End of args reached
		{
			temp[tempIndex] = '\0';
			strcpy(cmd[cmdIndex++], temp);
			memset(temp, 0, sizeof(char[512]));
			tempIndex = 0;
			cmd[cmdIndex] = NULL; //Adds NULL to end of cmdArray
			break;
		}

		else if(redir == 1 || redir == 0)
		{
			if(buffer[n] == '\0')
			{
				file[fileIndex] = '\0';
				fileIndex = 0;
				break;
			}

			else
				file[fileIndex++] = buffer[n];//Saves file name as string

		}
		
		else
			temp[tempIndex++] = buffer[n]; //Parses command/arg into temp variable

	}


//	builtInCmd(cmd, flag); //Checks for phase four shell built in commands

//	int retval = forkExecWait(cmd);// Retrieving return value for exit status 
	int retval, status;

	if(!builtInCmd(cmd, flag, &status)) //Check if built in shell command was invoked
	{
       		retval = forkExecWait(cmd, file, redir);//passes Command and arguments into function to fork+exec+wait
		fprintf(stderr, "+ completed '%s' [%d]\n", buffer, retval);
	}

	else
		fprintf(stderr, "+ completed '%s' [%d]\n", buffer, status);

	flag = 0;

	}//Loops indefinitely unless system failure


}



