CFILES	:= main.c
RM		:= rm -f
NAME	:= lem-in
CC		:= gcc
INCDIR	:= -I includes -I libft

LIB		:= libft.a
LIBDIR	:= libft
LIBPATH	:= $(LIBDIR)/$(LIB)
CFLAGS	:= -Wall -Wextra -Werror $(INCDIR) #-fsanitize=address 

all: $(NAME)

$(NAME): $(CFILES) $(LIBPATH)
	$(CC) $(CFLAGS) $(CFILES) -o $(NAME) $(LIBPATH)

$(LIBPATH):
	make -C $(LIBDIR)

clean:
	make -C $(LIBDIR) clean

fclean: clean
	$(RM) $(LIBPATH)
	make -C $(LIBDIR) clean
	$(RM) $(NAME)

re: fclean all

run: all
	./lem-in

leaks: all
	leaks --atExit -- ./lem-in

.PHONY: clean fclean all re run