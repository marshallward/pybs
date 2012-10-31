EXEC = pbs.so

PY_PATH = /apps/python/2.7.3
PY_INC = $(PY_PATH)/include/python2.7
PY_LIB = $(PY_PATH)/lib

PBS_PATH = /opt/pbs
PBS_INC = $(PBS_PATH)/include
PBS_LIB = $(PBS_PATH)/lib

CC = icc
CC_FLAGS = -fPIC -I$(PY_INC) -I$(PBS_INC) -g
LD_FLAGS = -shared -L$(PBS_LIB) -lpbs -lmunge

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $< $(LD_FLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CC_FLAGS)

clean:
	rm -f $(EXEC) $(OBJECTS)

.PHONY: clean
