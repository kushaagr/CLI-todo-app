# A Simple Command-line Todo-list application (stodo)
[![Windows](https://img.shields.io/static/v1?label=os&message=Windows&logo=microsoft&color=blue)]()
[![Linux](https://img.shields.io/static/v1?label=os&message=Linux&logo=linux&color=green)]()

Command line todo app created for [GDC Software Engineering Fellowship](https://fullstack.gdc.network/?utm_source=sendinblue&utm_campaign=GDC_-_Not_Selected&utm_medium=email) program.

## Usage

```
Usage :-
$ .\task add 2 hello world    # Add a new item with priority 2 and text "hello world" to the list
$ .\task ls                   # Show incomplete priority list items sorted by priority in ascending order
$ .\task del INDEX            # Delete the incomplete item with the given index
$ .\task done INDEX           # Mark the incomplete item with the given index as complete
$ .\task help                 # Show usage
$ .\task report               # Statistics
```

## Demo 
![task-cli-demo](https://github.com/kushaagr/cli-todo/assets/68564934/c30b1454-55c0-416d-9055-ca6f69227501)

## Build instructions
+ Make sure you have g++ and it compiles to **C++11**
+ On Linux, make the build.bat executable by running `chmod +x build.bat`
+ Execute `./build.bat` (Works on Windows and Linux)
