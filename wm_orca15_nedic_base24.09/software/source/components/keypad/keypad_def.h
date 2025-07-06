#ifndef KEYPAD_DEF_H
#define KEYPAD_DEF_H 1

typedef enum
{
    // COL0
    KEY00 = 0,
    KEY10 = 10,
    KEY20 = 20,
    KEY30 = 30,
    KEY40 = 40,
    KEY50 = 50,
    // COL1
    KEY01 = 1,
    KEY11 = 11,
    KEY21 = 21,
    KEY31 = 31,
    KEY41 = 41,
    KEY51 = 51,
    // COL2
    KEY02 = 2,
    KEY12 = 12,
    KEY22 = 22,
    KEY32 = 32,
    KEY42 = 42,
    KEY52 = 52,
    // COL3
    KEY03 = 3,
    KEY13 = 13,
    KEY23 = 23,
    KEY33 = 33,
    KEY43 = 43,
    KEY53 = 53,
    // COL4
    KEY04 = 4,
    KEY14 = 14,
    KEY24 = 24,
    KEY34 = 34,
    KEY44 = 44,
    KEY54 = 54,
    // COL5
    KEY05 = 5,
    KEY15 = 15,
    KEY25 = 25,
    KEY35 = 35,
    KEY45 = 45,
    KEY55 = 55,

    KEYALL = 0xff
} keypad_keyEnum_t;

#endif
