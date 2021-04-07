escheme-ooi
================

The _escheme_ interpreter is implemented using object-oriented programming techniques -- classes, virtual functions, and templates.

## Introduction

The inception of this project reaches back into the early 1990s. I answer the question whether it is possible to implement _escheme_
using a purely object-oriented approach -- no case analysis, only dynamic dispatch.

## Implementation Constraint(s)

* No discriminated union(s)

## Performance

After some initial performance tuning, here are some comparisons:

* speed of the interpreter is approximately 20% slower than that of the original discriminated union implementation.
* executable size is 40% larger
* SLOC is 23% smaller




