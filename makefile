CC      = gcc
CFLAGS  = -ansi -pedantic -pedantic-errors -Wall -Wextra -Werror \
          -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition

TARGET  = vokrat
SRCS    = vokrat.c filehandler.c colour.c color.c display.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

vokrat.o: vokrat.c filehandler.h display.h
	$(CC) $(CFLAGS) -c $< -o $@

filehandler.o: filehandler.c filehandler.h
	$(CC) $(CFLAGS) -c $< -o $@

colour.o: colour.c colour.h color.h
	$(CC) $(CFLAGS) -c $< -o $@

color.o: color.c color.h
	$(CC) $(CFLAGS) -c $< -o $@

display.o: display.c display.h filehandler.h colour.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)