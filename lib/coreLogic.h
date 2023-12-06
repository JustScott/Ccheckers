struct userChosenMove {
    int currentPosition;
    int desiredPosition;
};

int run(int (*promptUser)(char (*)[64], struct userChosenMove *, char (*)[6]));
