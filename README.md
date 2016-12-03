# seashell

 README


Shell by Oliver Chen, Jongyoul Lee, Dewan Sunnah


Features:
	Forks and executes commands
	Parses multiple commands on one line
	Redirects using <,>
	Implements pipes
Attempted:
	We attempted to also allow commands to have more than one space.
Bugs:
Extra spaces: 
    trimming works occasionally but may crash when there’s a newline or too many extra spaces or a tab
    (if necessary, comment out the two lines in parse_run() that call noMoreSpaces())

Files & Function Headers
shell.c


/*======== void run() ==========

	Inputs:
		char *command[], a null terminated array of pointers to strings
	Returns:
		nothing
 
forks a child process and executes the command, parent waits for child to exit
====================*/

/*======== void promptUser() ==========

	Inputs:
		none
	Returns:
		none
		
what it does: gets the user and current directory and prints them to prompt user for command
====================*/

/*======== int indexInArray() ==========

	Inputs:
		char *command[], an array of pointers to strings
		char *target, a string
	Returns:
		The index of command where target is found 
Searches in command for target and returns the index

====================*/

/*======== void redirOut() ==========

	Inputs:
		char *command[], an array of pointers to strings, where the pointers to the left of > represent the command whose stdin is being redirected and the pointers to the right of > represent the file we’re redirecting to
int outLoc, location of > symbol
	Returns:
		Nothing

Redirects stdin of first command to stdout of other between two commands and executes the second command
====================*/


/*======== void redirIn() ==========

	Inputs:
		char *command[], an array of pointers to strings, where the pointers to the right of < represent the command whose stdin is being redirected and the pointers to the left of < represent the file we’re redirecting into
int inLoc, location of < symbol
	Returns:
		Nothing


Redirects stdin of first command to stdout of other between two commands and executes the second command
====================*/




/*======== void thepipe() ==========

	Inputs:
		char *pipecommand[], an array of pointers to strings, where the pointers to the left of the pipe character(|) represent the command whose stdin is being redirected and the pointers to the right of | represent the command that will be executed
int pipelocation, index where pipe character(|) is found
	Returns:
		Nothing


Redirects stdin of first command to stdout of other between two commands and executes the second command
====================*/

/*======== void noMoreSpaces() ==========

	Input:
		char *command[]
	Returns:
		nothing

Removes all white spaces in the given array of pointers
====================*/


/*========void cd()============

	Input:
		char *whereto, a pointer to the path
	Returns:
		Nothing
Changes directories


===========================*/


/*======== void parse_run() ==========

What it does: prompts user for input, separates the input by semicolons, loops through the array of pointers created,
              separates the strings pointed to using white spaces then runs the command by
              determining if cd, exit, <, > or pipe are called and executes those separately, otherwise calls the run function on the commands
Arguments: None
Return Value: None


Example User Inputs:
  Multiple commands: ls -a -l;pwd;echo hello;exit
  Commands with extra spaces:    ls  -a    -l;   pwd
  Piping: ls -a -l | wc
  
====================*/

