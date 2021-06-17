# Usage
## Build
Build takes a regular expression (standard input)

- concatenation is written with no operator between symbols ab or bc. (NOT a.b)
- union is wirtten with '|' a|b
- star operation is written with '*'
- symbols are lowercase latin letters or digits 'a'-'z' '0'-'9'
- parentheses are allowed
- empty strings do not have symbols but can be written as ()

the code outputs (standard output) the NFA whos language is given regex 
- number of states is less than or equal to the length of the given string + 1
- start state is always 0
- nodes are numbered from 0

output follows given pattern
number of states, number of accept states, number of edges
list of accept states

for the next *i* lines, on the *i*th line we print k[i] and k[i]
