#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>

//ref from https://git.uwaterloo.ca/ece650-1221/cpp/-/blob/master/using_exec.cpp
//Processing random generator
int procRgen(int argc, char** argv) {

	argv[0] = (char *)"rgen";
	execv("rgen", argv); 

	return 0;
}

//process a1 ece650-a1.py
int procA1(int argc, char** argv) {

	argv[0] = (char *)"python";
	argv[1] = (char *)"ece650-a1.py";
	argv[2] = nullptr;

	execvp("python", argv);

	return 0;

}
//process a2 ece650-a2.cpp
int procA2(int argc, char** argv) {

	argv[0] = (char*)"ece650-a2";
	argv[1] = nullptr;

	execv("ece650-a2", argv);

	return 0;

}
//process input until EOD
int procInput(void) {

	while (!std::cin.eof()) {

		std::string line;
		std::getline(std::cin, line);

		if (line.size() > 0) {

			std::cout << line << std::endl;
		}
	}
	return 0;
}
// ref from https://git.uwaterloo.ca/ece650-1221/cpp/-/blob/master/using_pipe.cpp
int main (int argc, char** argv) {

	std::vector<pid_t> kids;

	// Pipe created
    // std::cout<<"debug A3 - pipe inititaed - rgen to a1" << std::endl;
    // rgen to a1 process - pipe1
	int pipe1[2];
	pipe(pipe1);

    // std::cout<<"debug A3 - pipe inititaed - a1 to a2" << std::endl;
    //a1 to a2 process - pipe2 
	int pipe2[2];
	pipe(pipe2);

	pid_t child_pid;

	child_pid = fork();
	// rgen to a1
	if (child_pid == 0)

	{
		// standard output to pipe
		dup2(pipe1[1], STDOUT_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);  
		
		return procRgen(argc,argv);
	}
    else if (child_pid<0){
        std::cerr<<"Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);
	
	child_pid = fork();
	if (child_pid == 0) {
		dup2(pipe1[0], STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);

		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe2[0]);
		close(pipe2[1]);

		return procA1(argc,argv);
	}
    else if (child_pid<0){
        std::cerr<<"Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);

	child_pid = fork();
	if (child_pid == 0)
	{
        //redirect stdin from pipe
		dup2(pipe2[0], STDIN_FILENO);
		close(pipe2[1]);
		close(pipe2[0]);

		return procA2(argc, argv);

	}
    else if(child_pid<0)
    {
        std::cerr<<"Error: could not fork\n";
        return 1;
    }

	kids.push_back(child_pid);
	child_pid = 0;
    child_pid = fork();
    if(child_pid==0){
        dup2(pipe2[1], STDOUT_FILENO);
	close(pipe2[0]);
	close(pipe2[1]);

    
	int res = procInput();
    return 0;    
    }
    kids.push_back(child_pid);

    // //redirect stdout to the pipe
	// dup2(pipe2[1], STDOUT_FILENO);
	// close(pipe2[0]);
	// close(pipe2[1]);

    // //
	// int res = procInput();
    int waitforChild;
    wait(&waitforChild);
    //kill all child process
    for (pid_t k : kids) {

        int status;

        kill (k, SIGTERM);

        waitpid(k, &status, 0);

    }
	return 0;
}