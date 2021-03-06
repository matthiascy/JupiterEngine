/*
 *      Copyright(C) 2014, Jupiter-org.
 *      License Boilerplate: MIT
 *
 *        _______
 *       /\      \                 __     __
 *       \ /-\ \-/                /\ \   /\ \
 *          \ \ \  __   __  _____ \/_/  _\_\ \__  ____  _____
 *           \ \ \/\ \  \ \/\  __/\/\`\/\___  __\/`__'\/\  __\
 *            \ \ \ \ \  \ \ \ \_\ \ \ \/__/\ \_/\  __/\ \ \_/
 *         /\` ` \ \ \ \--\ \ \ \_'/\ \_\  \ \_\\ \____\\ \_\
 *         \/______/\/____/_/\ \ \   \/_/   \/_/ \/____/ \/_/
 *                            \ \_\
 *                             \/_/

 *      @file:      jupiter_music.h
 *      @author:    ChenYang :) GuoYan WeiFuJiang
 *      @date:      2014/11/27 17:13
 *      @brief:     What a exciting moment, you can play music now!
 *                  Enjoy yourself! :-)
 */

#ifndef __JUPITER_MUSIC_H__
#define __JUPITER_MUSIC_H__

#include "windows.h"
#include "stdio.h"
#include "mmsystem.h"

class JupiterMusic {
public:
    JupiterMusic();
    // Construct JupiterMusic from szMusicPath.
    JupiterMusic(LPTSTR szMusicPath);
    ~JupiterMusic();

    // Music playing state, true for playing, false for pause.
    bool getPlayState();
    // Music open state, true for open, false for close.
    bool getOpenState();
    // Music stop state, true for stop, false for playing. 
    bool getStopState();
    // Return music volume.
    int getVolume();
    // Set music volume.
    void setVolume(int nVolume);
    // Decrease volume.
    void volumeDown();
    // Increase volume.
    void volumeUp();
    // Open music file.
    bool open(LPTSTR szMusicPath);
    // Close music file.
    bool close();
    // Stop playing music.
    bool stop();
    // Pause
    bool pause();

    // Play a music.
    // @params: nVolume -- music volume, 300 by default.
    //          bRepeat -- repeat music or not. False by default.
    //          bReStart -- restart or not. False by default
    bool play(int nVolume = 300, bool bRepeat = false, bool bReStart = false);

protected:
    // Statistics every music you added, signs each one a unique id.
    static int jupiterMusicID;
    // Music id.
    int jupiMscID;
    // Music volume.
    int jupiMscVolume;
};

#endif // __JUPITER_MUSIC_H__