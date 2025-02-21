1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: Calling execvp() from sheel would terminate the shell after a single command. Fork creates a nested process that allows the shell to keep running and the nested process runs the command. The value of the process would be to execute multiple commands without terminating our shell.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**: If fork fails we return -1. This usually occurs when the system has low memory or only a certain number of processes are allowed. In my code, I handle this by checking if fork() returns -1. If it does, my shell prints an error message using perror("fork failed") and continues running gracefully without crashing. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp() searches for the executable command by going through directories listed in the system environment variable PATH. For example, if you run ls, execvp() looks for an executable file named ls in each directory specified in PATH. If it finds the command, it executes it; if not, it returns an error.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  Calling wait() in the parent process ensures that the parent waits for the child to finish the process and collects its status. If we didn’t call wait(), the child process would become a process that has finished running but still exists in the process table upon termination. Over time, failing to use wait() can lead to resource exhaustion and result in low memory or unintended errors. 


5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() gets the exit status of a child process from the integer value returned by wait(). The exit status is helpful because it tells the shell whether the child command executed successfully or not. This status is crucial in debugging, and providing feedback to users about command execution.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: My implementation of build_cmd_buff() handles quoted arguments as single, unbreakable arguments, passing through internal spaces just as entered. When it encounters a quote (\"), the parser interprets all spaces within quotes as being part of that single argument until the closing quote. This is necessary because commands usually require arguments that include spaces (such as file names or strings) to be processed correctly, and incorrect parsing would otherwise invalidate the use of the intended command.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  In this assignment, I simplified parsing logic by removing pipe (|) handling since now we deal with a single command for input. Instead of using a command_list_t data structure, I implemented a lesser data structure cmd_buff_t, and used an array for argv. Another tough point was the handling of quoted strings, i.e., maintaining the space within a quoted string untouched. Refactoring involved approaches to tokenization logic ensuring the stability of parsing behavior.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Linux signals are events or notifications sent to processes, typically to inform them of some condition or to request that they take some action. Signals are not like other IPC methods like pipes or message queues since they carry minimal information, often just a signal number. They are also asynchronous, meaning they can be received at any time. Contrary to other mechanisms of IPC capable of transporting compound data, signals are designed to serve primarily as simple notifications or control commands.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGINT: Typically requests a process to quit gracefully. Used often for stopping running commands in terminals. 
    SIGTERM: Killing signal utilized by commands such as kill to politely ask processes to shut down and allow cleanup.
    SIGKILL: Instantly kills a process and cannot be caught, blocked, or ignored. Used as a last resort for processes not responding to SIGTERM.


- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  If a process is SIGSTOP, it is stopped right away and will not resume execution. SIGSTOP cannot be caught, caught, or ignored by the process. This is done to give the operating system a way of stopping any process at any moment, regardless of the process's own signal processing code.
