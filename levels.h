#ifndef LEVELS_H_INCLUDED
#define LEVELS_H_INCLUDED

struct Level {
    int sizeBlocks;
    int level;
};

struct Blocks {
    int x;
    int y;
    bool stay;
};

struct Level level;

struct Blocks* Level_1(int n, int m) {
    if(level.level == 1) {
        int probel = 5;
        level.sizeBlocks = n-(probel*2);
        struct Blocks* blocks = (struct Blocks*)malloc(sizeof(struct Blocks)*level.sizeBlocks);
        for(int i = 0; i < level.sizeBlocks; i++) {
            blocks[i].x = probel+i;
            blocks[i].y = m/2-2;
            blocks[i].stay = true;
        }
        return blocks;
    }
    if(level.level == 2) {
        int probel = 4;
        int h = 5;
        level.sizeBlocks = (int)((n-2)/probel)*h; //выделение памяти
        struct Blocks* blocks = (struct Blocks*)malloc(sizeof(struct Blocks)*level.sizeBlocks);
        for(int i = 0; i < level.sizeBlocks/h; i++) {
            for(int g = 0; g < h; g++) {
                blocks[i*h+g].x = probel*i+2;
                blocks[i*h+g].y = 1+g;
                blocks[i*h+g].stay = true;
            }
        }
        return blocks;
    }

    if(level.level == 3) {
        level.level = -1;
    }



}

#endif // LEVELS_H_INCLUDED
