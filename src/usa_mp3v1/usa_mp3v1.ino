/*****************************************************************************/
//	Function: Control the ChibiUsa
//      Hardware: ATmega328p-pu(arduino bootloader)
//                Original Substrate
/*******************************************************************************/

#include "Grovemp3.h"

#define PIN_RX 2
#define PIN_TX 3

#define PIN_SW1 7
#define PIN_SW2 8

//各センサの入力信号ピン
#define PIN_DIST A5
#define PIN_VOL  A1
#define PIN_LED  16

//#define PIN_VOL_PEN A1
//#define PIN_ILM_PEN A0
//#define PIN_LED_PEN 16

Grovemp3* grovemp3;

int key_dist = 300;
int key_ilm_pen = 100;
int val_ret, temp_random;
static int val_past, val_now;
///////////////////////////////////////////////////////

//boolean get_flg_trg(){
//    //[0] is now,[1] is past
//    static int flg_trg[2];
//    flg_trg[1] = flg_trg[0];
//    flg_trg[0] = digitalRead(SW_TRG);
//    
//    //trueなら発光可、falseなら発光不可
//    boolean flg = false;
//
//    if(flg_trg[1] == HIGH && flg_trg[0] == LOW){
//        flg = true;
//    }
//    return flg;
//}
int myrandom(int val_min, int val_max){
  
    while(1){
        temp_random = val_ret;
        val_ret = random(val_min, val_max);
        
        if(temp_random != val_ret){
            return val_ret;
        }
    }
}    


void set_keydist(){
    
    int val_sens = analogRead(A5);
    key_dist = analogRead(PIN_VOL);
  
    if(val_sens < key_dist){
      digitalWrite(PIN_LED, HIGH);
    }
    else{
      digitalWrite(PIN_LED, LOW);
    }
}


void play_voice(int num){ 
  
  static int sens_past;
  static int sens_now;
    
  sens_past = sens_now;
  sens_now = analogRead(PIN_DIST);

  //明るい→暗いに変化＝しゃべる
  if(sens_past > key_dist && sens_now < key_dist){
    grovemp3->playVoice(00,num);
  }
}

void play_voice_hand(int mode){
     
        //再生するmp3ファイルの番号格納用
        int num;

        //じゃんけんモード
        if(mode==3){
          num = random(1, 5);
        }
        //あっちむいてほいモード
        else if(mode==2){
          num = random(5, 8);
        }
        //通常のおしゃべりモード
        else if(mode==1){
          num = myrandom(10, 28);
        }
        //grovemp3->playVoice(00,num);
        play_voice(num);
}

///////////////////////////////////////////////////////////////

    

void setup() {
  
    randomSeed(analogRead(A0)+micros()+millis());
    
    //LED点灯用に設定
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);
    
    pinMode(14, OUTPUT);
    
    //mp3モジュールが立ち上がるまで待機
    for(int i=13; i<19; i++){
        delay(600);
    }
    
    //mp3モジュール制御用インスタンス生成
    grovemp3 = new Grovemp3(PIN_RX, PIN_TX);
    
    //ボリュームは最大にセット
    grovemp3->setVolume(0x1F);
    delay(100);
    //プレイモード0（繰り返し無しのモード）
    grovemp3->setPlayMode(0x00);
    delay(100);
    //grovemp3->playVoice(00,01);
    //grovemp3->playVoice(00,00);
}

void loop() {
    
    int mode = 1;
  
    int sw1 = digitalRead(PIN_SW1);
    int sw2 = digitalRead(PIN_SW2);
  
    //スイッチで３つのモード切替
    if(sw1==LOW && sw2==HIGH){
      //じゃんけんモード  
      mode = 3;
    }
    else if(sw1==HIGH && sw2==LOW){
      //あっちむいてほいモード
      mode = 2;
    }
    else{
      //受付モード
      mode = 1;
    }  
  
      
    //手をかざす方のセンサの閾値設定
    set_keydist();

    play_voice_hand(mode);
   
    
    delay(500);
 
}
