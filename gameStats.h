/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   gameStats.h
 * Author: ajporter
 *
 * Created on May 28, 2024, 6:37 PM
 */

#ifndef GAMESTATS_H
#define GAMESTATS_H

using namespace std;

class GameStats {
public:
    float gamesPlayed;
    float gamesWon;
    float gamesLost;
    
    GameStats() : gamesPlayed(0), gamesWon(0), gamesLost(0) {}
    
    void recordWin();
    void recordLoss();
    void displayStats() const;
};


#endif /* GAMESTATS_H */
