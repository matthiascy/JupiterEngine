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
 *
 *      @file:      jupiter_music.cpp
 *      @author:    ChenYang :) Guoyan WeiFuJiang
 *      @date:      2014/11/27 17:13
 *      @brief:     Implementation of JupiterMusic, nothing special, easy to
 *                  understand. Take your time and have fun! :-)
 */

#include "jupiter_music.h"
#pragma comment(lib,"winmm.lib")

int JupiterMusic::jupiterMusicID = 1;

JupiterMusic::JupiterMusic()
{
    jupiMscID = jupiterMusicID;
    jupiterMusicID++;	
    jupiMscVolume = 300;

}

JupiterMusic::JupiterMusic(LPTSTR szMusicPath)
{
    jupiMscID = jupiterMusicID;
    jupiterMusicID++;	
    jupiMscVolume = 300;
    if(strlen(szMusicPath) > 0) {
        open(szMusicPath);
    }
}

JupiterMusic::~JupiterMusic()
{
    close();
}		

bool JupiterMusic::getPlayState()
{	
    char szStatus[20] = {'\0'};
    char szCommand[50];
    
    // Create MCI command 
    sprintf(szCommand, "status MUSIC%d mode", jupiMscID);
    // Get state
    mciSendString(szCommand, szStatus, 20, 0);
    // Status is 'playing'
    if (strcmp(szStatus, "playing") == 0)
        return true;
    else
        return false;
}

bool JupiterMusic::getOpenState()
{
    char szStatus[20] = {'\0'};
    char szCommand[50];

    sprintf(szCommand, "status MUSIC%d mode", jupiMscID);	
    mciSendString(szCommand, szStatus, 20, 0);
    // No matter the music is been playing or stopped, it's already open.
    if (strcmp(szStatus,"stopped") == 0 || strcmp(szStatus,"playing") == 0)
        return true;
    else
        return false;
}

bool JupiterMusic::getStopState()
{
    char szStatus[20] = {'\0'};
    char szCommand[50];

    sprintf(szCommand, "status MUSIC%d mode", jupiMscID);	
    mciSendString(szCommand, szStatus, 20, 0);
    if (strcmp(szStatus,"stopped") == 0)
        return true;
    else
        return false;
}

int JupiterMusic::getVolume()
{
    return jupiMscVolume;	
}

void JupiterMusic::setVolume(int nVolume)
{
    if(nVolume < 0)
        nVolume = 0;
    if(nVolume > 1000)
        nVolume = 1000;

    char szCommand[50];
    // Create MCI command.
    sprintf(szCommand, "set audio MUSIC%d volume to %d", jupiMscID, nVolume);
    mciSendString(szCommand, NULL, 0, 0); // Set volume.

    jupiMscVolume = nVolume;
}

void JupiterMusic::volumeDown()	
{
    setVolume(jupiMscVolume - 10);
}                              //  \
                               //    Default stride is 10
void JupiterMusic::volumeUp() //  /
{
    setVolume(jupiMscVolume + 10);
}

bool JupiterMusic::open(LPTSTR szMusicPath)
{

    if (strlen(szMusicPath) == 0)
        return false;

    if (getPlayState() || getStopState()) // If there is a music file open,
        close();                         // close it.

    char szCommand[50];
    sprintf(szCommand, "open %s ALIAS MUSIC%d", szMusicPath, jupiMscID);
    if (!mciSendString(szCommand, NULL, 0, 0)) { // Open music file.
        return true;
    }
    return false;
}

bool JupiterMusic::close()
{
    char szCommand[50];

    if (getOpenState()) {
        sprintf(szCommand,"close MUSIC%d", jupiMscID);
        if (!mciSendString(szCommand, NULL, 0, 0)) {
            return true;				
        }
    }
    return false;
}

bool JupiterMusic::play(int nVolume/* = 300*/, bool bRepeat/* = false*/,
                        bool bReStart/* = false*/)
{
    char szCommand[50];	
    // Only when music is stop or request restart
    if (getStopState() || bReStart) {
        if (bRepeat)
            sprintf(szCommand, "play MUSIC%d FROM 0  repeat", jupiMscID);
        else
            sprintf(szCommand, "play MUSIC%d FROM 0", jupiMscID);

        if (!mciSendString(szCommand, NULL, 0, 0)) {
            setVolume(nVolume);				
            return true;
        }
    }

    return false;
}

bool JupiterMusic::stop()
{
    char szCommand[50];
    if (getPlayState()) {
        sprintf(szCommand, "stop MUSIC%d", jupiMscID);
        if (!mciSendString(szCommand, NULL, 0, 0)) {
            return true;
        }
    }		
    return false;
}	