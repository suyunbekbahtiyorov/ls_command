CC = gcc
CFLAGC = -g -Wall -Wextra -Werror
SOURCES = main.c
TARGET = my_ls

$(TARGET):$(SOURCES)
	$(CC) $(CFLAGC) -o $@ $^

clean:
	rm -f *.o

fclean: clean
	rm -f $(TARGET)

re: fclean all