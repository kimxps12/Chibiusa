/*****************************************************************************/
//	Function: control the seeedstudo Grove MP3 player ver.2
//      Hardware: Grove - Serial MP3 Player ver.2
//      自作メソッドあり
/*******************************************************************************/
#include "Grovemp3.h"

//コンストラクタ
Grovemp3::Grovemp3(){};
Grovemp3::Grovemp3(uint8_t pin_rx, uint8_t pin_tx){
  
    mp3 = new SoftwareSerial(pin_rx, pin_tx);
    mp3->begin(9600);
    delay(50);
    
};

//Set the music index to play, the index is decided by the input sequence
//of the music;
//hbyte: the high byte of the index;
//lbyte: the low byte of the index;
boolean Grovemp3::setMusicPlay(uint8_t hbyte,uint8_t lbyte){
  
	mp3->write(0x7E);
	mp3->write(0xFF);
	mp3->write(0x06);
  mp3->write(0x03);
  mp3->write(uint8_t(0x00));
	mp3->write(hbyte);
	mp3->write(lbyte);
	mp3->write(0xEF);
  delay(10);
        
	while(mp3->available()){
	    if (0xA0==mp3->read()){
	        return true;
            }
	    else{
	        return false;
            }
        }
}



//Set the volume, the range is 0x00 to 0x1E
boolean Grovemp3::setVolume(uint8_t volume){
  
    mp3->write(0x7E);
    mp3->write(0xFF);
    mp3->write(0x06);
    mp3->write(0x06);
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(volume);
    mp3->write(0xEF);
     
     delay(10);
     
     while(mp3->available()){
         if (0xA7==mp3->read()){
             return true;
         }
         else{ 
             return false;
         }
     }
}

boolean Grovemp3::setPlayMode(uint8_t playmode){
    if (((playmode==0x00)|(playmode==0x01)|(playmode==0x02)|(playmode==0x03))==false){
        Serial.println("PlayMode Parameter Error! ");
        return false;
    }
    
    mp3->write(0x7E);
    mp3->write(0x03);
    mp3->write(0xA9);
    mp3->write(playmode);
    mp3->write(0x7E);

    delay(50);
    
    if(mp3->available()){
        delay(100);
        if (mp3->read() ==0xA9){
            return true;
        }
        else{ 
            return false;
        }
    }
}

//現在音声を再生中かどうかを取得します
//true : 再生終了の瞬間（エッジ）
//false : その他(再生中, 待機中など)
boolean Grovemp3::getPlayState(){
  
    unsigned char c[10] = {0};
    uint8_t i = 0;
    //check if there's any data sent from the Grove_Serial_MP3_Player
    while(mp3->available()){
        c[i] = mp3->read();
        i++;
        delay(1);
        if (i == 10) break;
    }
    
    if(c[3] == 0x3C || c[3] == 0x3D || c[3] == 0x3E){
        return true;
    }
    else{
        return false;
    }
}

//信号を送ろうとしたときに再生中だったら
//送るのをやめる優れものな再生メソッド
boolean Grovemp3::playVoice(uint8_t hbyte, uint8_t lbyte){
  
    if(flg_playing==false){
        flg_playing = true;
        setMusicPlay(hbyte,lbyte);
        delay(10);
        while(getPlayState()==false){}
        flg_playing = false;
        return true;
    }
    else{
        return false;
    }  
}

///////////////////////////////////////////////////
//ver.2から実装されたメソッド
///////////////////////////////////////////////////

/**************************************************************** 
 * Function Name: SelectPlayerDevice
 * Description: Select the player device, U DISK or SD card.
 * Parameters: 0x01:U DISK;  0x02:SD card
 * Return: none
****************************************************************/ 
void Grovemp3::selectPlayerDevice(uint8_t device)
{
    mp3->write(0x7E);
    mp3->write(0xFF);
    mp3->write(0x06);
    mp3->write(0x09);
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(device);
    mp3->write(0xEF);
    delay(200);
}

/**************************************************************** 
 * Function Name: PlayPause
 * Description: Pause the MP3 player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void Grovemp3::playPause(void)
{
    mp3->write(0x7E);
    mp3->write(0xFF);
    mp3->write(0x06);
    mp3->write(0x0E);
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
//  mp3->write(0xFE);
//  mp3->write(0xED);
    mp3->write(0xEF);
    delay(20);
//  return true;
}

/**************************************************************** 
 * Function Name: IncreaseVolume
 * Description: Increase the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void Grovemp3::increaseVolume(void)
{
    mp3->write(0x7E);
    mp3->write(0xFF);
    mp3->write(0x06);
    mp3->write(0x04);
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(0xEF);
    delay(10);
//  return true;
}

/**************************************************************** 
 * Function Name: DecreaseVolume
 * Description: Decrease the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void Grovemp3::decreaseVolume(void)
{
    mp3->write(0x7E);
    mp3->write(0xFF);
    mp3->write(0x06);
    mp3->write(0x05);
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(uint8_t(0x00));
    mp3->write(0xEF);
    delay(10);
//  return true;
}



