# Directories
SRCDIR  := src
OBJDIR  := obj
INCDIR  := inc
LIBDIR  := libft

# Files
CFILES  := $(wildcard $(SRCDIR)/*.c)
OFILES  := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CFILES))
LIB     := libft.a
LIBPATH := $(LIBDIR)/$(LIB)
NAME    := lem-in

# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -I $(INCDIR) -I $(LIBDIR)

# Commands
RM      := rm -f

# Targets
all: $(NAME)

$(NAME): $(OFILES) $(LIBPATH)
	@$(if $(shell [ -e $(NAME) ] && find $(OFILES) $(LIBPATH) -newer $(NAME) | head -n 1),\
		$(CC) $(CFLAGS) $(OFILES) -o $(NAME) $(LIBPATH),\
		echo "$(NAME) is up to date.")

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIBPATH):
	make -C $(LIBDIR)

clean:
	make -C $(LIBDIR) clean
	$(RM) $(OFILES)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(LIBPATH)
	make -C $(LIBDIR) fclean
	$(RM) $(NAME)

re: fclean all

run: all
	./$(NAME)

test: all
	cat maps/subject.map | ./$(NAME)

leaks: all
	leaks --atExit -- ./$(NAME)

.PHONY: clean fclean all re run test leaks
