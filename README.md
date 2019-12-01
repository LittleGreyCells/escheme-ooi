escheme-oops
================

The escheme interpreter is implemented using object-oriented programming techniques -- classes, virtual functions, and templates.

## Introduction

In this old project I answer the question whether it possible to implement escheme
using a purely object-oriented approach. After some false starts, the escheme interpreter using conventional methods was transformed into
a satisfyingly object-oriented implementation.

## Implementation Constraint(s)

* No discriminated union

## Post Construction

Almost all of the original escheme system is completed with the exception of a handful of custom object access primtives.

## Performance

The first regression test suite passes! Wow! That was quick.

After some initial performance tuning, here are some comparisons:

* speed of the interpreter is 20% slower (than that of the original)
* executable size is 39% larger
* SLOC is 23% smaller




