This code is a implementation of lexical analyzer and recursive descent parser for the RPAL language as a part of class project for PLP. 
Written by: Kedar Amrolkar   UFID:18418443	University of Florida

To compile the project, use the included Makefile i.e just run command
$ make         =>     This will create a executable named 'p1' in the same folder.

You can use following command to clear compilation:
$ make cl	=>     This will delete the executable created above.

About executable p1:
Run by: 
	$ ./p1 -ast ~/testFolder/testFile.txt	=> This will print the Abstract Syntax Tree.
or	$ ./p1 -l ~/testFolder/testFile.txt	=> This will print the input.
or	$ ./p1 -ast -l ~/testFolder/testFile.txt	=> This will print the input then the Abstract Syntax Tree.
or	$ ./p1 -l -ast ~/testFolder/testFile.txt	=> This will print the input then the Abstract Syntax Tree.
or	$ ./p1 -ast -debug ~/testFolder/testFile.txt	=> This will print debugging msgs and then the Abstract Syntax Tree.
or 	$ ./p1 -debug -ast ~/testFolder/testFile.txt	=> This will print debugging msgs and then the Abstract Syntax Tree.

Any other switches or parameters or command wont work.

To test automatically the ASTs using 'difftest.pl' file, run the command:
$ perl difftest.pl -1 "./rpal -ast -noout FILE" -2 "./p1 -ast FILE" -t ~/Desktop/rpalFolder/testFolder/
