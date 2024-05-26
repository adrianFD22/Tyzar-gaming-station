
enum game {
    N4S,
    AHORCADO,
    NIM,
    PERMUTER,
    CCP,
    MAZE,
    FLIPIT,
    SALIR,
    NUM_GAMES
};

// Iconos para cada juego. Una portada consistiendo
// en 6 strings de tamaño 20 carácteres para mostrar
// a la izquierda del menú cuando se pase por encima
// de cada juego.
const char icons[NUM_GAMES][6][64] = {
    // N4S
    {
        "",
        "N   N     4    SSSS",
        "NN  N    44   S    ",
        "N N N   4 4    SSS ",
        "N  NN  44444      S",
        "N   N     4   SSSS ",
    },

    // AHORCADO
    {
        "",
        "",
        "",
        "",
        "",
        "",
    },

    // NIM
    {
        "     ___          ",
        "     ___       ___",
        "___  ___       ___",
        "___  ___       ___",
        "___  ___  ___  ___",
        "___  ___  ___  ___",
    },

    // PERMUTER
    {
        "",
        "",
        "",
        "",
        "",
        "",
    },

    // CCP
    {
        "",
        "",
        "",
        "",
        "",
        "",
    },

    // MAZE
    {
        " __________________ ",
        " █   ____    __    |",
        "|  | |___ |  |  |  |",
        "| _| ___| |__| _|  |",
        "|_____  |____   | _|",
        "|__________________ ",
    },

    // FLIPIT
    {
        "     _______________ ",
        "    |██  |████      |",
        "    |    |██        |",
        "    |____|    ██████|",
        "    |██   ██      ██|",
        "    |_______________|",
    },

    // SALIR
    {
        "",
        "",
        "",
        "",
        "",
        "",
    },

};
