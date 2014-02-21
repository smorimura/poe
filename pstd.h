/* pstd.h
 * standard libraries
 */

/* LIST OF ALL FUNCTIONS IN STANDARD LIBRARIES FOLLOWS:

GLOBAL FUNCTIONS:
compile: compiles a string or source code file to a code object. (Unlike load,
does not execute the code.)
copy
eq?
export(src, (dest)): if dest is given, copy all the elements from table src
to the table dest. If not, copy to the global table.
fold(f,init,iter): collapses a list according to a given function
I: identity function
load: executes zero or more string or file objects on the currently
running Poe universe. To determine whether the file is bytecode or source code,
consults the file extension (.poe vs. .pbc).
map
map!: applies a function to all elements of an iterable
print: prints all of the objects, with a space between each element and a
newline following.
range
require: executes zero or more bytecode or source code files on the current Poe
universe. The difference between require and load is that require consults the
required table before loading anything.
_REQUIRED: a table containing the names of all files required.
*run: executes zero or more bytecode, string, or file objects on a NEW Poe
universe.
size: get size of iterable
time
tochar: converts an integer or floating-point number to a character.
toint: converts a character or floating-point number to an integer.
tofloat: converts an integer or character to a floating-point number.
tostring: converts any object into a string.
type: retrieves the type of any object.
STRING LIBRARY:
string.toarr: returns a new array whose elements are the integer values of all
the string's characters. (Takes an optional array buffer.)
string.fromarr: returns a new string composed of the character encodings of the
integer arguments. (Takes an optional string buffer.)
string.cmp
*string.code: returns a new string from the binary representation of the
instructions in a code or function object. Is not directly executable; write to
a file to execute the code. (Takes an optional string buffer.)
string.concat(s1, s2, (buf))
string.copy!(dest, src, (n), (max)): copy bytes from src to dest starting at 
index n. if max is declared, copy only max bytes at most.
string.find(str, sub): find occurrences of sub in str. return starting index
of match and last index of match, storing this data in string.found for later
calls. 
*string.format: Returns a string formatted from the given format string (first
argument) and the function's arguments.
string.found.str
string.found.sub
string.found.start
string.ncmp
string.slice
*string.tok
MATH LIBRARY:
math.abs
math.arccos
math.arcsin
math.arctan
math.ceiling
math.cos
math.cosh
math.deg
math.ex
math.floor
math.fmod
math.log
math.log10
math.pi
math.pow
math.rad
math.rand
math.srand
math.sin
math.sinh
math.sqrt
math.tan
math.tanh
I/O LIBRARY:
io.close
io.flush
io.open
io.read(n, (file), (str)): read at most n characters from file, using optional
buffer str
io.readc: read character from file
io.readf: read float from file (using scanf)
io.readi: read integer from file (using scanf)
io.readl((file), (str)): read a line from file, using optional buffer str
io.stderr
io.stdin
io.stdout
io.tostr: read the entire contents of a file, storing everything in a single
Poe string
io.toarr: read the entire contents of a file, storing everything in an array
of Poe strings, one string per line
io.write
GC LIBRARY*:
gc.cycle
gc.majcyc
gc.majoffset
gc.start
gc.step
gc.stepfactor
gc.stop
gc.timer
*/

#ifndef PSTD_H_
#define PSTD_H_

#include "pdata.h"
#include "puniverse.h"

int poe_loadglobs(poe_universe *);
int pstd_compile(poe_universe *);
int pstd_I(poe_universe *);
int pstd_print(poe_universe *);
int pstd_size(poe_universe *);
int pstd_time(poe_universe *);
int pstd_tochar(poe_universe *);
int pstd_tofloat(poe_universe *);
int pstd_toint(poe_universe *);
int pstd_tostring(poe_universe *);
int pstd_type(poe_universe *);

#endif
