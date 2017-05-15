/*****************************************************************************/
//	Function: control the seeedstudo Grove MP3 player
//      Hardware: Grove - Serial MP3 Player
/*******************************************************************************/

#include <arduino.h>
#include "SoftwareSerial.h"

#ifndef Grovemp3_h
#define Grovemp3_h


class Grovemp3{
  
private:
    SoftwareSerial* mp3;
    
    //SoftwareSerial mp3(int pin_rx, int pin_tx);
    
public:

    Grovemp3();
    Grovemp3(uint8_t pin_rx, uint8_t pin_tx);
    
    //Set the music index to play, the index is decided by the input sequence
    //of the music;
    //hbyte: the high byte of the index;
    //lbyte: the low byte of the index;
    boolean setMusicPlay(uint8_t hbyte,uint8_t lbyte);

    // Pause on/off  the current music
    boolean pauseOnOffCurrentMusic();

    //Set the volume, the range is 0x00 to 0x1F
    boolean setVolume(uint8_t volume);

    boolean setPlayMode(uint8_t playmode);
  
    //Playing is true
    //Stopped is false
    boolean getPlayState();

    boolean playVoice(uint8_t hbyte, uint8_t lbyte);
};
#endif
