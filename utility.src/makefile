%.o: %.c
	$(CC) -c $< -o $(@)

execs = addtime dircount dirtime fatfile space_rename

#OBJS = $(foreach alpha,$(execs),$(alpha).o )
#ex2 = $(wildcard *.c)

all : $(execs)

$(execs): %: %.o
	gcc -Wall -O2 $< -o $(@)
