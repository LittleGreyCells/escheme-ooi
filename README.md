escheme-ooi
================

The _escheme_ interpreter is implemented using purely object-oriented programming techniques -- classes, virtual functions, and templates.

## Introduction

The inception of this project reaches back into the early 1990s. I answer the question whether it is possible to implement _escheme_
using a predominantly object-oriented approach for escheme object representations and services -- no case analysis, only dynamic dispatch.

## Implementation Constraint(s)

* No discriminated union(s)

## Performance

After some initial performance tuning, here are some comparisons:

* speed of the interpreter is approximately 18% to 30% slower (depending on test) than that of the original discriminated union implementation.
* executable size is 35% larger
* SLOC is 6% smaller




