CC = gcc

NAME = gamebox

FLAGS = -g -Werror -Wall -export-dynamic `pkg-config --cflags gtk+-3.0`

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
       src/checkers/chks-view.c    \
       src/checkers/chks-field.c   \
       src/checkers/chks-check.c   \
       src/gbox-window.c           \
       src/gbox-preview.c          \
       src/main.c

all: $(SRCS)
	$(CC) $(FLAGS) $(SRCS) $(LIBS) -o $(NAME)

debug:
	$(CC) -DDEBUG $(FLAGS) $(SRCS) $(LIBS) -o $(NAME)

clean: $(NAME)
	rm $(NAME)

install: $(NAME)
	install -Dm 0755 $(NAME) /usr/bin/$(NAME)
	install -Dm 0644 gamebox.desktop /usr/share/applications/gamebox.desktop
	install -Dm 0644 LICENSE /usr/share/licenses/gamebox/LICENSE
	install -Dm 0644 images/mill.png /usr/share/gamebox/images/mill.png
	install -Dm 0644 images/tictactoe-preview.png /usr/share/gamebox/images/tictactoe-preview.png
	install -Dm 0644 images/connect-four-preview.png /usr/share/gamebox/images/connect-four-preview.png
	install -Dm 0644 images/mill-preview.png /usr/share/gamebox/images/mill-preview.png
	install -Dm 0644 images/checkers-preview.png /usr/share/gamebox/images/checkers-preview.png
	install -Dm 0644 images/checkers.png /usr/share/gamebox/images/checkers.png

uninstall:
	rm /usr/bin/$(NAME)
	rm /usr/share/applications/gamebox.desktop
	rm /usr/share/licenses/$(NAME) -r
	rm /usr/share/gamebox -r
