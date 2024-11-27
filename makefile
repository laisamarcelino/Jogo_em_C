# Nome do executável
TARGET = jogar

# Fontes e cabeçalhos
SRC = main.c jogador.c auxiliares.c joystick.c inimigos.c projeteis.c
OBJ = $(SRC:.c=.o)

# Flags do compilador
CFLAGS = -Wall -Wextra -O2 $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags) -lm
LDFLAGS = $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags) -lm

# Regra principal
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Regra para compilar arquivos .c em .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Limpeza dos arquivos gerados
clean:
	rm -f $(OBJ) $(TARGET)

# Regra para recompilar tudo
rebuild: clean all
