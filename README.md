escheme-oops
================

The escheme interpreter is implemented using object-oriented programming techniques -- classes, virtual functions, and templates.

## Introduction

In this old project, whose inception reaches back into the early 1990s, I answer the question whether it is possible to implement escheme
using a purely object-oriented approach. Among the OO-cognoscenti this means NO CASE ANALYSIS.

## Implementation Constraint(s)

* No discriminated union(s)

## Post Construction

Almost all of the original escheme system is completed with the exception of a handful of custom object access primtives and no linenoise integration.

## Performance

After some initial performance tuning, here are some comparisons:

* speed of the interpreter is approximately 20% slower than that of the original discriminated union implementation.
* executable size is 40% larger
* SLOC is 23% smaller




