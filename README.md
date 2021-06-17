# Usage
## Build
Build takes a regular expression (standard input)

- concatenation is written with no operator between symbols ab or bc. (NOT a.b)
- union is wirtten with '|'  (ex. a|b)
- star operation is written with '\*' (ex. a\*)
- symbols are lowercase latin letters or digits 'a'-'z' '0'-'9'
- parentheses are allowed
- empty strings do not have symbols but can be written as ()

the code outputs (standard output) the NFA whos language is the given regex 
- number of states is less than or equal to the length of the given string + 1
- start state is always 0
- nodes are numbered from 0

output follows given pattern
number of states, number of accept states, number of edges
list of accept states
after these lines we output the NFA as a graph,
on *i*th line we output the number of edges exiting state numbered *i* (lets call this number *k*). after that come *k* pairs. first element of the pair is the character with which the step is made, the second is the number of the state at the end of the edge.

## Run
Takes a string and a NFA, NFA is formated as described above (standard input)

code prints a string consisting of 'Y' and 'N'. (lets call this string *s*)
s[i] is 'Y' if after reading characters s[0]..s[i] the NFA is in an accept state, and 'N' if it is not.


