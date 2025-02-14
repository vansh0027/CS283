1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:
Citation: Taken from Geeks for Geeks. Fgets() is a better choice as it reads a line of input, and prevents buffer overflow. As it limits the number of characters it is reading However, scanf()  doesn’t allow spaces within the input being read and cannot handle empty lines. Moreover, fgets() can also detect the end of the file.


2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: We need malloc as cmd_buff needs to be dynamically allocated based on SH_CMD_MAX. If we use a fixed array, we might waste memory if the buffer is too large/small. Our shell is more flexible with this approach.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: Trimming leading and trailing spaces makes sure that commands are properly parsed and executed. If spaces were not trimmed we could have empty commands causing errors as they might be misinterpreted and cause multiple unwanted warnings. Extra spaces in pipeline-separated commands could cause failures in execution.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: 1. ls > output.txt 2. sort < input.txt 3. echo "New line" >> output.txt Challenges:  1.File handling, and overwrite protection. 2. We need to open the file in read mode and redirect STDIN before executing the command. 3. We must open the file in append mode and handle file permissions correctly to prevent data loss.


- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection connects a command's input/output to a file. Piping connects the output of one command directly to the input of another. Redirection is about file I/O; piping is about inter-process communication.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: Separation of STDERR and STDOUT allows for better scripting/automation, debugging, a cleaner user experience, and separate error logging.
Keeping STDERR separate from STDOUT allows error messages to be distinct from normal output. If mixed together, errors could corrupt structured data. For example, if the output is redirected to a file, allowing stderr to mix into the file would jeopardize the parsing at a later point in time. 


- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Shell should capture STDOUT/STDERR separately, report exit status, display STDERR (default), optionally display STDOUT, and provide a way to merge them (e.g., 2>&1). This aids debugging and scripting.
