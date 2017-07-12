CC = gcc

NAME = gamebox

FLAGS = -g -Wall -Werror -export-dynamic `pkg-config --cflags gtk+-3.0`

LIBS = -export-dynamic `pkg-config --libs gtk+-3.0`

SRCS = src/tictactoe/ttt-view.c    \
       src/tictactoe/ttt-field.c   \
       src/tictactoe/ttt-check.c   \
       src/connect-four/cf-view.c  \
       src/connect-four/cf-field.c \
       src/connect-four/cf-check.c \
       src/mill/mill-view.c        \
       src/mill/mill-field.c       \
       src/mill/mill-check.c       \
       src/gbox-window.c           \
       src/main.c

all: $(SRCS)
	$(CC) $(FLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean: $(NAME)
	rm $(NAME)

install: $(NAME)
	install -Dm 0755 $(NAME) /usr/bin/$(NAME)
	install -Dm 0644 gamebox.desktop /usr/share/applications/gamebox.desktop
	install -Dm 0644 LICENSE /usr/share/licenses/gamebox/LICENSE
	install -Dm 0644 images/mill.png /usr/share/gamebox/images/mill.png

uninstall:
	rm /usr/bin/$(NAME)
	rm /usr/share/applications/gamebox.desktop
	rm /usr/share/licenses/$(NAME) -r
	rm /usr/share/gamebox -r
