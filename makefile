CC      = gcc
CFLAGS  = -ansi -pedantic -pedantic-errors -Wall -Wextra -Werror \
          -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition

TARGET  = labyrinth
SRCS    = labyrinth.c filehandler.c colour.c color.c display.c entities.c random.c game.c history.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

labyrinth.o: labyrinth.c filehandler.h display.h entities.h random.h game.h history.h
	$(CC) $(CFLAGS) -c $< -o $@

filehandler.o: filehandler.c filehandler.h
	$(CC) $(CFLAGS) -c $< -o $@

colour.o: colour.c colour.h color.h
	$(CC) $(CFLAGS) -c $< -o $@

color.o: color.c color.h
	$(CC) $(CFLAGS) -c $< -o $@

display.o: display.c display.h filehandler.h colour.h
	$(CC) $(CFLAGS) -c $< -o $@

entities.o: entities.c entities.h
	$(CC) $(CFLAGS) -c $< -o $@

random.o: random.c random.h
	$(CC) $(CFLAGS) -c $< -o $@

game.o: game.c game.h filehandler.h display.h entities.h random.h history.h
	$(CC) $(CFLAGS) -c $< -o $@
	
history.o: history.c history.h
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)