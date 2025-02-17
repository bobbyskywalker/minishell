# **minishell** 🐚

## 📖 Overview

minishell is a lightweight, custom shell implementation that mimics the basic functionality of Bash. It is designed to handle user input, execute commands, manage processes, and support fundamental shell features like redirections and piping. Built as a school project at 42 Warsaw.

## ✨ Features

* Command & executable execution (built-in and external commands)

* Redirections (>, <, >>, <<)

* Pipes (|)

* Environment variable handling

* Working history

* Double and single quotes handling

* Signal handling (Ctrl+C, Ctrl+D, Ctrl+\\)

## 🛠️ Getting started

Ensure you have these prerequisites:

- A Linux or macOS operating system
- `gcc` (GNU Compiler Collection)
- `make` utility.

## ⚙️ Command execution

**📜 Built-in commands:**  

***cd*** - change directory

***pwd*** - print current working directory

***export*** - create or replace an environment variable

***unset*** - unset an environment variable

***env*** - show the environment variables on stdout

***exit*** - exit the shell

Other commands are executed by searching for the specified executable or command within the directories listed in the PATH environment variable. Alternatively, commands can be executed by providing their relative or absolute path.

## 🚀 Usage

To compile:

```bash
make
```
To run the shell:

```bash
./minishell
```

The program replicates Bash behaviour on most of the prompts. You can try some basic commands like:

```bash
ls -l
cat file.txt > output.txt
echo "Hello, World!" | grep Hello
export VAR=value
```
Check **Known Issues** section for more info.

## Known Issues
* Ctrl+C on empty prompt gets exit status = 85, not 130
* ***export*** command with no args works like env, does not mimic exact bash behaviour
* ***export*** with args works a bit different than OG one
* Ctrl+C doesn't escape from heredoc
* Input redirection without a preceding command doesn't work properly

## Notes 📌
👀 If you notice any bugs and/or have a solution to one of the **known issues**, feel free to fork the repository & submit a pull request!

📢 If you're a 42 student, use this as a guide at most. Don't cheat, learn! <3

📅 Created in January/February 2025 as part of 42's Common Core at 42 Warsaw

## 📄 License

This project is licensed under the MIT License. See LICENSE for details.

👨‍💻 Authors

https://github.com/Jzackiewicz

https://github.com/bobbyskywalker