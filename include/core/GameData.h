#ifndef GAMEDATA_H
#define GAMEDATA_H

class GameData
{
public:
    static bool playerVictory;
    static bool zombieFarAway;

    enum RuneState { RUNA_ALAGADA, RUNA_VAZIA, RUNA_DESENHADA, RUNA_COM_UMA_VELA, RUNA_FINALIZADA, RUNA_LIGADA };
    static RuneState runeState;
};

#endif // GAMEDATA_H
