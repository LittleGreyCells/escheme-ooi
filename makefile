C++ = g++

APP    = escheme4

CWD = $(shell pwd)
SRCLOC = $(CWD)/src
INCLUDES = -I$(SRCLOC)

SRCS	= \
	$(SRCLOC)/Char.cxx \
	$(SRCLOC)/Fixnum.cxx \
	$(SRCLOC)/Flonum.cxx \
	$(SRCLOC)/List.cxx \
	$(SRCLOC)/Memory.cxx \
	$(SRCLOC)/Nil.cxx \
	$(SRCLOC)/Node.cxx \
	$(SRCLOC)/Str.cxx \
	$(SRCLOC)/Symbol.cxx \
	$(SRCLOC)/Vector.cxx \
	$(SRCLOC)/FilePort.cxx \
	$(SRCLOC)/StringPort.cxx \
	$(SRCLOC)/TerminalPort.cxx \
	$(SRCLOC)/Env.cxx \
	$(SRCLOC)/Prim.cxx \
	$(SRCLOC)/Closure.cxx \
	$(SRCLOC)/ByteVector.cxx \
	$(SRCLOC)/Continuation.cxx \
	$(SRCLOC)/Promise.cxx \
	$(SRCLOC)/SymbolTable.cxx \
	$(SRCLOC)/Reader.cxx \
	$(SRCLOC)/Eval.cxx \
	$(SRCLOC)/PortIO.cxx \
	$(SRCLOC)/Equality.cxx \
	$(SRCLOC)/Transcript.cxx \
	$(SRCLOC)/Primitives.cxx \
	$(SRCLOC)/Primitives_List.cxx \
	$(SRCLOC)/Primitives_Vector.cxx \
	$(SRCLOC)/Primitives_Math.cxx \
	$(SRCLOC)/Primitives_String.cxx \
	$(SRCLOC)/Primitives_Char.cxx \
	$(SRCLOC)/Primitives_Symbol.cxx \
	$(SRCLOC)/Primitives_Env.cxx \
	$(SRCLOC)/Primitives_Closure.cxx \
	$(SRCLOC)/Primitives_Equality.cxx \
	$(SRCLOC)/Primitives_IO.cxx \
	$(SRCLOC)/Primitives_Unix.cxx \
	$(SRCLOC)/Scheme.cxx \
	$(SRCLOC)/intstack.cxx \
	$(SRCLOC)/regstack.cxx \
	$(SRCLOC)/argstack.cxx \

OBJS	= $(SRCS:.cxx=.o)

# optimizations
#   -O
#   -O1 -- Optimize
#   -O2 -- All w/o space/speed trade-off
#   -O3 -- Includes inlining

#DEBUG_FLAGS = -O3
#DEBUG_FLAGS = -O2
#DEBUG_FLAGS = -O1
#DEBUG_FLAGS = -g
DEBUG_FLAGS = -O

CFLAGS = $(DEBUG_FLAGS) -pedantic -std=c++14
LFLAGS = $(DEBUG_FLAGS) -v -lm

$(APP)	: $(OBJS)
	$(C++) -o $@ $(OBJS) $(LFLAGS)
	ln -sf $(APP) escheme

%.o	: %.cxx
	$(C++) $(DEFINES) $(INCLUDES) -c $(CFLAGS) $< -o $@

clean :
	find . -name "*.o" -delete
	find . -name "*~" -delete
	rm -f escheme

install : $(APP)
	mkdir -p /usr/share/$(APP)
	mkdir -p /usr/share/$(APP)/boot
	mkdir -p /usr/share/$(APP)/macros
	cp escheme.scm  /usr/share/$(APP)
	cp boot/*.scm   /usr/share/$(APP)/boot
	cp macros/*.scm /usr/share/$(APP)/macros
	cp $(APP) /usr/local/bin/$(APP)_i
	echo 'ESCHEME=/usr/share/$(APP) $(APP)_i $$@' > /usr/local/bin/$(APP)
	chmod +x /usr/local/bin/$(APP)
	ln -s /usr/local/bin/$(APP) /usr/local/bin/escheme

uninstall :
	if [ -d /usr/share/$(APP) ]; \
	then \
	   rm -rf /usr/share/$(APP); \
	   rm -f /usr/local/bin/$(APP); \
	   rm -f /usr/local/bin/$(APP)_i; \
	   rm -f /usr/local/bin/escheme; \
	fi

