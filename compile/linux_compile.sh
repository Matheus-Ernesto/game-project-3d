cd ../game
g++ game.cpp -o /test_1.o \
    -I../libs/linux/SFML/include \
    -L../libslibs/linux/SFML/lib \
    -lsfml-graphics -lsfml-window -lsfml-system -lGL