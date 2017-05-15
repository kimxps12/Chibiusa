/*****************************************************************************/
//	Function: Control the ChibiUsa
//      Hardware: ATmega328p-pu(arduino bootloader)
//                Original Substrate
/*******************************************************************************/

#include "Grovemp3.h"

#define PIN_RX 2
#define PIN_TX 3

//Pin Number of Switches
#define PIN_VOLUP   5
#define PIN_VOLDOWN 6
#define PIN_SW1 7
#define PIN_SW2 8
 

//各センサの入力信号ピン
#define PIN_ILM  0
#define PIN_VOL  1
#define PIN_LED  16

//Voice File Index
#define START_ATTIMUITE 1
#define END_ATTIMUITE   3
#define START_JANKEN    4
#define END_JANKEN      7
#define START_NOMAL     8
#define END_NOMAL      22


Grovemp3* grovemp3;

int key_dist = 300;
int val_ret, temp_random;
static int val_past, val_now;
///////////////////////////////////////////////////////

int myRandom(int val_min, int val_max){
  
    while(1){
        temp_random = val_ret;
        val_ret = random(val_min, val_max);
        
        if(temp_random != val_ret){
            return val_ret;
        }
    }
}    


void set_keydist(){
    
    int val_sens = analogRead(PIN_ILM);
    key_dist = analogRead(PIN_VOL);

     Serial.print("Sens : ");
     Serial.print(val_sens);
     Serial.print("  VR : ");
     Serial.println(key_dist);
  
    if(val_sens < key_dist){
      digitalWrite(PIN_LED, HIGH);
    }
    else{
      digitalWrite(PIN_LED, LOW);
    }
}


void play_voice(uint8_t num){ 
  
  static int sens_past;
  static int sens_now;
    
  sens_past = sens_now;
  sens_now = analogRead(PIN_ILM);

  //明るい→暗いに変化＝しゃべる
  if(sens_past > key_dist && sens_now < key_dist){
    grovemp3->playVoice(00,num);

    grovemp3->increaseVolume();
  }
}

void play_voice_hand(int mode){
     
        //再生するmp3ファイルの番号格納用
        int num;

        //あっちむいてほいモード
        if(mode==3){
          num = random(START_ATTIMUITE, END_ATTIMUITE+1);
        }
        //じゃんけんモード
        else if(mode==2){
          num = random(START_JANKEN, END_JANKEN+1);
        }
        //通常のおしゃべりモード
        else if(mode==1){
          num = myRandom(START_NOMAL, END_NOMAL+1);
        }
        play_voice(uint8_t(num));
}

///////////////////////////////////////////////////////////////

    

void setup() {
  
    randomSeed(analogRead(0)+micros()+millis());
    
    //LED点灯用に設定
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);
    pinMode(PIN_VOLUP, INPUT_PULLUP);    
    pinMode(PIN_VOLDOWN, INPUT_PULLUP);
        
    //mp3モジュールが立ち上がるまで待機
    delay(5000);
    
    //mp3モジュール制御用インスタンス生成
    grovemp3 = new Grovemp3(PIN_RX, PIN_TX);
    
    //ボリュームは最大にセット
    grovemp3->setVolume(0x15);
    delay(100);

    //Select SD card as the player device.
    grovemp3->selectPlayerDevice(0x02); 
    delay(100);

    //grovemp3->playVoice(00,01);
    //grovemp3->setMusicPlay(00,01);
    Serial.begin(9600);
}

void loop() {

  /*
  if(digitalRead(PIN_SW1)==LOW){
      grovemp3->playVoice(00,01);
  }
  if(digitalRead(PIN_SW2)==LOW){
    grovemp3->playVoice(00,02);
  }
  //Serial.println(grovemp3->queryPlayStatus());
  delay(100);
  */
    int mode = 1;
  
    int sw1 = digitalRead(PIN_SW1);
    int sw2 = digitalRead(PIN_SW2);
  
    //スイッチで３つのモード切替
    if(sw1==LOW && sw2==HIGH){
      //あっちむいてほいモード  
      mode = 3;
    }
    else if(sw1==HIGH && sw2==LOW){
      //じゃんけんモード
      mode = 2;
    }
    else{
      //受付モード
      mode = 1;
    }

    //Control Volume
    int volup = digitalRead(PIN_VOLUP);
    int voldown = digitalRead(PIN_VOLDOWN);
    
    if(volup==LOW && voldown==HIGH){
      grovemp3->increaseVolume();
    }
    else if(volup==HIGH && voldown==LOW){
      grovemp3->decreaseVolume();
    }
      
    //手をかざす方のセンサの閾値設定
    set_keydist();

    play_voice_hand(mode);
    
    delay(200);

}
