This code is a implementation of Interpreter and CSE machine program execution kit for the RPAL language as a part of class project for PLP. 
Written by: Kedar Amrolkar   UFID:18418443	University of Florida

To compile the project, use the included Makefile i.e just run command
$ make         =>     This will create a executable named 'p2' in the same folder.

You can use following command to clear compilation:
$ make cl	=>     This will delete the executable created above.

About executable p2:
Run by: 
	$ ./p2 -ast testFolder/testFile.txt	=> This will print the Abstract Syntax Tree and final output.
or	$ ./p2 -l testFolder/testFile.txt	=> This will print the input.
or	$ ./p2 -ast -l testFolder/testFile.txt	=> This will print the input, Abstract Syntax Tree and then final output.
or	$ ./p2 -l -ast testFolder/testFile.txt	=> This will print the input, Abstract Syntax Tree and then final output.
or	$ ./p2 -ast -debug testFolder/testFile.txt	=> This will print debugging msgs, the Abstract Syntax Tree and output.
or 	$ ./p2 -debug -ast testFolder/testFile.txt	=> This will print debugging msgs, the Abstract Syntax Tree and output.

Any other switches or parameters or command wont work.

To test automatically the ASTs using 'difftest.pl' file, run the command:
$ perl difftest.pl -1 "./rpal -ast -noout FILE" -2 "./p2 -ast FILE" -t testFolder/

NOTE:
The 54 different Test files are present in the 'testFolder' directory. These are valid RPAL Programs.
