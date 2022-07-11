#include "OLED_I2C.h"           // подключаем библиотеку для экрана
OLED myOLED(SDA, SCL, 8);  // создаем объект myOLED

// подключаем шрифты для текста и цифр
extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];



class game_model{
public:
  int LeastStepsPlatform = 0;
  int nowPosPlatform     = 0;
  
  bool gameStatus        = true;     

  int LeastSteps_onX_Ball  = 100;
  int nowPosX_Ball         = 0;

  int LeastSteps_onY_Ball = 45;
  int nowPosY_Ball     = 0;
  int points = 0;
  
  void process_game(){
    if(gameStatus){ 
          myOLED.clrScr();
          buttonChecker();
          processMath();
          step_ball();
          
          step_platform();
      }else{
                     myOLED.clrScr();
                     myOLED.print("GAME OVER ", CENTER, 30);
                     myOLED.print(String(points) + " points!", CENTER, 48);

        
          }
        
          myOLED.update();

    }
  void step_platform(){
          if(LeastStepsPlatform > 0){
                      myOLED.print("____", ++nowPosPlatform, 50);
            }
          if(LeastStepsPlatform < 0){
                      myOLED.print("____", --nowPosPlatform, 50);
            } 
          if(LeastStepsPlatform == 0){
                  myOLED.print("____", nowPosPlatform, 50);
          }
    }
   void step_ball(){
          if(LeastSteps_onY_Ball > 0){
                      LeastSteps_onY_Ball--;
                      ++nowPosY_Ball;
          }
          if(LeastSteps_onY_Ball < 0){
                      LeastSteps_onY_Ball++;
                      --nowPosY_Ball;
          }
             

           if(LeastSteps_onX_Ball > 0){
                      LeastSteps_onX_Ball--;
                      ++nowPosX_Ball;
           }
           if(LeastSteps_onX_Ball < 0){
                      LeastSteps_onX_Ball++;
                      --nowPosX_Ball;
           }
           
          myOLED.print("*", nowPosX_Ball, nowPosY_Ball);
           
    }

    void processMath(){
      if((nowPosY_Ball == 45) && (nowPosPlatform-7 <= nowPosX_Ball) &&(nowPosX_Ball <= nowPosPlatform+20)){
              LeastSteps_onY_Ball = -45;
              points++;
      }else if(nowPosY_Ball == 45){
              gameStatus = false;
      }

      if(nowPosY_Ball == 0){
        LeastSteps_onY_Ball = 45;
      }

      if(nowPosX_Ball == 120){
        LeastSteps_onX_Ball = -120;
      }

      if(nowPosX_Ball == 0){
        LeastSteps_onX_Ball = 128;
      }
      
      
    }

    void buttonChecker(){
      Serial.println("A0:" + String(analogRead(A0)));
      Serial.println("A1:" + String(analogRead(A1)));
      if(analogRead(A0) < 512){
        LeastStepsPlatform+=1;
      }
      if(analogRead(A1) < 512){
        LeastStepsPlatform-=1;
      }
      if(analogRead(A0) > 1000 && analogRead(A1) > 1000){
        LeastStepsPlatform = 0;
      }
    }
  
};



game_model platform;
   
void setup(){
     // инициализируем экран

  Serial.begin(9600);
  myOLED.begin();
  myOLED.setFont(SmallFont);
  myOLED.print("GAME PLATFORM", 32, 48);
  myOLED.update();
  delay(1000);
  myOLED.clrScr();

  //кнопки
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

}

void loop(){
    
  platform.process_game();
  
  //myOLED.update();

  //delay(10);
  //pratform.moveAll(100);
  //pratform.moveAll(-100);
      
  
  
}
