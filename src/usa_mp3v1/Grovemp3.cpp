/*****************************************************************************/
//	Function: control the seeedstudo Grove MP3 player
//      Hardware: Grove - Serial MP3 Player
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
	mp3->write(0x04);
	mp3->write(0xA0);
	mp3->write(hbyte);
	mp3->write(lbyte);
	mp3->write(0x7E);
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


// Pause on/off  the current music
boolean Grovemp3::pauseOnOffCurrentMusic(void){
  
     mp3->write(0x7E);
     mp3->write(0x02);
     mp3->write(0xA3);
     mp3->write(0x7E);
     delay(10);
     
     while(mp3->available()){
         if (0xA3==mp3->read()){
             return true;
         }
         else{
             return false;
         }
     }
}

//Set the volume, the range is 0x00 to 0x1F
boolean Grovemp3::setVolume(uint8_t volume){
  
     mp3->write(0x7E);
     mp3->write(0x03);
     mp3->write(0xA7);
     mp3->write(volume);
     mp3->write(0x7E);
     
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
//Playing is true
//Stopped is false
boolean Grovemp3::getPlayState(){
  
    uint8_t hbyte;
    uint8_t lbyte;
  
    mp3->write(0x7E);
    mp3->write(0x02);
    mp3->write(0xC2);
    mp3->write(0x7E);
    delay(10);
    
    if(mp3->available()){
        hbyte = mp3->read();
        lbyte = mp3->read();
    }

    if(lbyte == 0x01){ 
        return true;
    }
    else if(lbyte == 0x02){
        return false;
    }
}

//信号を送ろうとしたときに再生中だったら
//送るのをやめる優れものな再生メソッド
boolean Grovemp3::playVoice(uint8_t hbyte, uint8_t lbyte){
  
    if(getPlayState()==false){
        setMusicPlay(hbyte,lbyte);
        delay(10);
        return true;
    }
    else{
        return false;
    }
        
}
