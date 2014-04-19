CC := gcc
CC_FLAGS :=
LD_FLAGS :=
src := capture.c
obj := $(patsubst %.c, %.o, $(src))

capture: $(obj)
	$(CC) $(CC_FLAGS) -o $@ $^

$(obj): $(src)
	$(CC) $(LD_FLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm capture
