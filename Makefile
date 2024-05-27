
CLINK = -lncurses

GAMES_DIR = games
COMPILE_DIR = compiled

MENU = $(GAMES_DIR)/Menu/menu.c
GAMES = $(GAMES_DIR)/ahorcado/ahorcado.c \
		$(GAMES_DIR)/CCP/ccp.c \
		$(GAMES_DIR)/Flip/flip.c \
		$(GAMES_DIR)/Maze/maze.c \
		$(GAMES_DIR)/NIM/nim.c \
		$(GAMES_DIR)/permuter/permuter.c \
		$(GAMES_DIR)/Press_the_key/press_the_key.c \
		$(GAMES_DIR)/Pong/pong.c

.PHONY:
install:
	@for game in $(GAMES) ; do \
		filename=$$(basename -- "$$game"); \
		extension="$${filename##*.}"; \
		filename="$${filename%.*}"; \
		gcc $$game -o $(COMPILE_DIR)/$$filename $(CLINK);\
	done \

	@gcc $(MENU) -o tgs -lncursesw

clean:
	@rm -f compiled/*
