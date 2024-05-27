
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
        "          |---",
        "          0",
        "         -|-",
        "",
        "",
        "palabra: _h_r_ad_",
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
        "Xx      →      xX",
        "      ↑  ↓",
        " per  U  M  ter",
        "      ↑  ↓",
        "       ←",
        "Xx             xX",
    },

    // CCP
    {
        "  0--1--2--1--2--1",
        "",
        " /////  /////  ////",
        " //     //     /  /",
        " //     //     ////",
        " /////  /////  /",
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
        "//// /  / //// ////",
        "/     //   //   //",
        "//    //   //   //",
        "/     //   //   //",
        "//// /  / ////  //",
    },

};
