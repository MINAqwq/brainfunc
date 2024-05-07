# BrainFunc

Optimized and compiled Brainfuck superset with functions and syscall interface.


## Build

```sh
make all
```

yeah, thats it


## Syntax

```
+ | Add
- | Sub
< | Left
> | Right
[ | Loop Start
] | Loop End
. | Print
, | Read
$ | Declare
@ | Call
# | Terminate String
! | Return
; | Comment
& | Copy
* | Syscall
```


### Declare Functions

Function names start with $ and end with #.
To call them replace $ with @.

```bf
; sample programm

$echo#
,.[-]
!

$printlf#
++++++++++.[-]
!
$main#
++++++[>+++++++++++<-]>+++. ; E:  70
+<++++[>++++++++++<-]>.     ; n: 110
++++++.                     ; t: 116
---------------.            ; e: 101
; I wanted to write "Enter a symbol:" but im too lazy so it's just "Ente"

; copy e to next location
&>.
>
@printlf#
@echo#
@printlf#
!
```


## Bytecode

Brainfuck instructions will be compiled to a byte set that has a range from 0 - 12 so the BrainFunc VM is able to use jump table instead of multiple compares.

Calls are working with offsets. The start of compiled BrainFunc code is offset 0, where the VM will start executing. 


## BFE (BrainFunc Executable)

The BFE Format gives the vm infos about code length about entry point.
Some of the sizes are architecture depended. Yes, compiled Brainfunc is not intended to be cross platform.

| Name | Description | Type/Size |
|------|-------------|---------------|
| magic | 4 ascii chars ("BFUN") | char[4] |
| ver_major | Major Version | uint8 |
| ver_minor | Minor Version | uint8 |
| size | size of code without header | int |
| entry | offset of entry point | int |


## BFC (BrainFunc Compiler)

```sh
# Usage:
bfc [file1...file2]
```

The compiler writes the compiled output to stdout. To save the compiled code you can use your shell.

```sh
bfc file1.bf file2.bf >| file.bfe
```


## BFVM (BrainFunc Virtual Machine)

```sh
# Load and execute file
bfvm file.bfe

# Read code from stdin (won't support read operations atm)
cat file.bfe | bfvm -
```

### Syscalls

The BFVM exposes the OS API through own syscalls a Brainfunc program can use. Arguments are either given as biggest addressable number or c string from your bf memory array. To execute a syscall set the memory index to the syscall number followed by its arguments.

Availabe Syscalls:
| Number | Name | Arguments | Description |
| ------ | ---- | --------- | ----------- |
| 0x00 | Open | str_path, str_mode | Open a file |
| 0x01 | Close | | (not implemented) |
| 0x02 | Read |  | (not implemented) |
| 0x03 | Write |  | (not implemented) |  

## TODO
- [ ] Stack Trace for runtime error
- [ ] Debug Symbols?
- [ ] bfstd library
- [ ] MORE Syscalls
