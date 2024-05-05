# BrainFunc

Optimized Brainfuck with functions.

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
```

### Declare Functions

Function names start with $ and end with #.
To call them replace $ with @.

```bf
$foo#
++++++++++.[-]
!

$main#
++[>+++++<-]>
@foo#
!
```

## Bytecode

Brainfuck instructions will be compiled to a byte set that has a range from 0 - 10 so the BrainFunc VM is able to use jump table instead of multiple compares.

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
