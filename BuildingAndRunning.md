Building and Running
====================

## Building

In the root directory type 'make' to build the executable:
```
   $ make
```

## Running

To run the interpreter in the build directory invoke escheme and the
*terminal* prompt is displayed:
```
   $ ./escheme

   ece>
```

Files can by loaded before the prompt if listed on the command line::
```
   $ ./escheme <file1> <file2> ... <filen>
```   

To exit escheme, type ^D, ^C or exit function at the prompt:
```
   ece> (exit)
```

## Installation and Removal

The scheme interpeter and its supporting files can be installed on Unix/Linux systems.
```
   $ sudo make install
```

The interpreter can now be invoked anywhere, not just off of the build directory, using the
pathless executable name "escheme-oops". The executable is installed in /usr/local/bin which
should be included in the path definition, if not already.

Lastly the interpreter can be uninstalled.
```
   $ sudo make uninstall
```

