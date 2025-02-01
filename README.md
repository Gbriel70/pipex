<div align="center"><h1>Pipex [125 / 100]</h1></div>

<div align="center">
  <a href="https://github.com/Gbriel70/pipex" target="_blank"><img height=300 src="https://github.com/byaliego/42-project-badges/raw/main/badges/pipexm.png"></a>
</div>

# Pipex 🚀

*A project that recreates the behavior of the operator |  (pipe) of the shell, connecting two commands via fork(), pipe() and execve(). Ideal for learning about processes, I/O redirection and pipe management.*


# What is Pipex ?

Pipex is a simulation of the | from the shell. It allows you to execute two commands in sequence, where the output of the first command is passed as input to the second command, using pipes and processes.

# Exemple

**Comand pipe |**
```bash
>$ ls | grep exemple.txt
```
**Pipex program**
```bash
>$ ./pipex input_file "ls" "grep exemple.txt" output_file    
```

# ⚙️ How does it work?

**Execution Flow**

    Open Input/Output Files:

        input.txt → Reading.

        output.txt → Writing (created if does not exist).

    Create a Pipe:

        Two file descriptors: pipefd[0] (reading) and pipefd[1] (writing).

    Child Process 1 (cmd1):

        Redirects standard output (STDOUT) to pipefd[1].

        Run cmd1 (ex: ls).

    Child Process 2 (cmd2):

        Redirects standard input (STDIN) to pipefd[0].

        Redirects standard output (STDOUT) to output.txt.

        Run cmd2 (ex: grep .txt).

    Close Pipes and Wait for Processes:

        Ensure all processes terminate with waitpid().

# ASCII diagram

```bash
input.txt → [cmd1] → (pipe) → [cmd2] → output.txt
```

# 🛠️ Installation and Use

1. Clone the repository
    ```bash
    git clone https://github.com/Gbriel70/pipex.git
    ```
2. Navigate to the project directory
    ```bash
    cd pipex
    ```
3. Init submodule
   ```bash
   git submodule update --init --recursive
   ```
4. Compile the project using Make:
    ```bash
    make
    ```

## Usage

Execute the pipex program with the following syntax:
```bash
./pipex <input_file> "<command1>" "<command2>" <output_file>
```
Example:
```bash
./pipex input.txt "ls -l" "wc -l" output.txt
```

# 🧩 Key Concepts

#### 1. Processes (fork())

    Creates a child process by duplicating the parent process.

    Tip: Use fork() to run cmd1 and cmd2 in separate processes.

#### 2. Pipes (pipe())

    Creates two file descriptors: one for reading (pipefd[0]) and one for writing (pipefd[1]).

    Attention: Close unused descriptors to avoid leaks!

#### 3. Redirection (dup2())

    Redirects STDIN/STDOUT to the pipe's file descriptors or files.

    Example: dup2(pipefd[1], STDOUT_FILENO) sends the output of cmd1 to the pipe.

### 4. Command Execution (execve())

    Replaces the current process with the executed command.

    Use access() to check if the command exists and is executable.

### 5. Error Management

    Always check returns from functions like fork(), pipe(), open().

    Use perror() for clear error messages (e.g. "Error: fork failed").


# 🐛 Common Problems and Solutions

| Problem               | Probable Cause                | Solution                                      |
|-----------------------|-------------------------------|-----------------------------------------------|
| Command not found     | Incorrect PATH or invalid command | Use absolute paths (/bin/ls) or check PATH.   |
| Pipe Leak             | Unclosed descriptors          | Close all descriptors with `close()`.         |
| Output file not created | Invalid permissions or path  | Use `open()` with `O_CREAT | O_WRONLY | O_TRUNC`. |
| Commands stuck        | Pipe not closed properly      | Ensure that the writing side of the pipe is closed in the reading process. |

# 🧪 Testing the Project

### Test Cases

1. **Basic Commands:**
    ```bash
    ./pipex input.txt "ls" "wc -l" output.txt
    # Equivalent to: ls < input.txt | wc -l > output.txt
    ```
2. **Commands with Arguments:**
    ```bash
    ./pipex input.txt "grep 'hello'" "awk '{print \$1}'" output.txt
    ```
3. **Intentional Errors:**
    ```bash
    ./pipex no_exist.txt "ls" "wc" output.txt # Invalid input file
    ```
4. **PATH Command:**
    ```bash
    ./pipex input_file "/usr/bin/ls" "/usr/bin/wc" output_file
    ```

# 🏆 BONUS PART

## 🔥 Bonus Features

# 1 -> **Multiple Pipes Support**

    Allows you to chain more than two commands (e.g. cmd1 | cmd2 | cmd3 | ...).

    How to implement:

    Use a loop to create pipes and processes dynamically.

    Manage file descriptors carefully to avoid leaks.

**Usage example:**

```bash
./pipex input.txt "cat" "grep test" "wc -l" output.txt
```

# 2 -> **Here Documents (<<)**

**🔍 How does it work?**

    The here_doc reads the user input until it finds a specific delimiter, indicated by the user. All the content typed up to the delimiter is stored and then redirected to the command that will be executed.

**🚀 Example of Usage in the Shell**

```bash
$ ./pipex here_doc LIMITER cmd1 cmd2 outfile
```
    This simulates the following behavior in the shell:

    cmd1 << LIMITER | cmd2 >> outfile

    Here, the LIMITER defines where the here_doc input ends.

## ⚙️ Implementation Logic

1. Checking the here_doc:

    In main(), check if the first argument is here_doc. This changes the logic of redirection and opening the output file (append mode instead of truncate).

    Reading the Input:
    Use a loop with get_next_line() to read the user input until the delimiter is found.

    ```C
    while (1) {
    write(1, "heredoc> ", 9);
    line = get_next_line(STDIN_FILENO);
    if (!line || ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
    break;
    write(pipe_fd[1], line, ft_strlen(line));
    free(line);
    }
    ```

2. Creating a Pipe:

    Store the content read in a pipe, redirecting its output to the cmd1 command.

3. Executing the Commands:

    cmd1 reads from the pipe (instead of a file), and cmd2 processes the result normally, saving it in outfile.

## 📝 Considerations

    The output file in here_doc mode must be opened with the O_APPEND mode, to add it to the end of the existing file.
    Proper control of resources (pipes, processes and memory) is essential to avoid memory leaks and open file descriptors.

# 3 -> **Environment Variables (PATH)**

    Searches for commands in the directories listed in the $PATH environment variable.

    How to implement:

    Use split(":", getenv("PATH")) to get the list of directories.

    Check in each directory if the command exists (access()).

**Example:**
```bash
./pipex input.txt "ls" "grep .c" output.txt # Works without using /bin/ls
```

# 4 -> **Advanced Error Handling**

    Detailed error messages (e.g. "Command not found: lss").

    Handling signals like SIGINT (Ctrl+C) to close descriptors correctly.

# 5 -> **Error Input/Output Redirection (STDERR)**

    Allows you to redirect STDERR to a file (e.g. 2> error.log).

    How to implement:

    Use dup2() to redirect file descriptor 2 (STDERR_FILENO).
