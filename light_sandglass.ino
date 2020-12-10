#include <MPU6050.h>
#include <Wire.h>
#include <AsyncDelay.h>
#include <Adafruit_NeoPixel.h>


AsyncDelay countTimer;
MPU6050 mpu;
int ax, ay, az, gx, gy, gz, xval, prevVal; //자이드롭센서
unsigned long prevMillis=0 , lightMillis=0, beepMillis=0;
long interval = 1000; 
long liinterval=100;
int count=0; // 뒤집은 반응 +
int fullcount=0; //총타이머 몇분인지 저장
double sec=0; // 초
int first=0;
double bright=0; //빛밝기
int re=0; // 수평으로 있는 시간 계산
boolean b=true, p=false; // 반짝이는 코드에 사용
int wtb=0; //수평으로 들때 타이머에 사용

Adafruit_NeoPixel strip = Adafruit_NeoPixel (16, 3, NEO_GRBW + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize();
  strip.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();

  mpu.getMotion6(&ax, &ay, &az, &gz, &gy, &gz);
  xval=map(ax, -17000, 17000, 0, 180);
//    if(xval != prevVal){
//      prevVal = xval;
//      Serial.print("x축 :");
//      Serial.println(xval);
//      Serial.print("도");
//      Serial.print("\n");
//    }

  if(first==0){//헤헤 이상하지만 꼭있어야함 
    if(175 < xval && xval< 185){
      count=-1;
    }
    first++;
  }

  //================뒤집기 반응 체크 [top]================================================
  if(175 < xval && xval< 185){
    re=0;
    wtb=1;
    if(175 < prevVal && prevVal< 185){//-----안뒤집었을때
      prevVal=xval;
      sec=count*30+sec;
      if(sec>0){
        if(currentMillis - prevMillis >interval){
          prevMillis=currentMillis;
        }
      }else{// ------초기화
        fullcount=0;
        bright=0;
        lightready(bright);
      }
      count=0;
    }else if(350 < prevVal || prevVal < 10){//-----뒤집었을때
      ++count;//1분 추가
      fullcount=fullcount+1;
      prevVal=xval;//자이로센서 초기화
    }
        
  //==============뒤집기 반응 체크 [bottom]=========================================
  } else if(350 < xval || xval < 10){
    wtb=2;
    re=0;
    if(350 < prevVal || prevVal < 10){//안뒤집었을때
      prevVal=xval;
      sec=count*30+sec;
      if(sec>0){
        if(currentMillis - prevMillis >interval){
          prevMillis=currentMillis;
        }
      }else{
        fullcount=0;
        bright=0;
        lightready(bright);
      }
      count=0;
    }else if(175 < prevVal && prevVal< 185){//뒤집었을때
      ++count;
      fullcount=fullcount+1;
      prevVal=xval;
    }
       
  //=======초기화==(수평으로 들때)=================================================  
  } else if(75 < xval && xval < 125){    
    if( re > 500){
      sec=0;
      
      re=0;
      Serial.println("초기화");
      fullcount=0;
      bright=0;
      lightready(bright);
    }
    re++;
    
  }
  
  
  
  //================[top] 빛 작동 ===================
  if(wtb==1){
      if(sec>10){
        if(currentMillis - lightMillis > liinterval ){ //0,1초마다 반복
          sec= sec-0.1; 
          Serial.print("sec : ");
          Serial.println(sec);
          lightMillis=currentMillis;
          bright=255*sec/(30*fullcount);
          lighttop(bright);
        }
      }else if(sec>0){ //=======10초이하 알림        
        if(currentMillis - beepMillis > 500){
          sec=sec-0.5;
          Serial.print("sec : ");
          Serial.println(sec); 
          if(b==true){
            beepbottom(b);
            b=false;
          }else{
            beepbottom(b);
            b=true;
          }
          beepMillis=currentMillis;
        }
      }
    //===========[bottom] 빛 작동=====================
    }else if(wtb==2){
      if(sec>10){
        if(currentMillis - lightMillis > liinterval ){ //0,1초마다 반복
          sec= sec-0.1; 
          Serial.print("sec : ");
          Serial.println(sec);
          lightMillis=currentMillis;
          bright=255*sec/(30*fullcount);
          lighttop(bright);
        }
      }else if(sec>0){ //=======10초이하 알림
        if(currentMillis - beepMillis > 500){
          sec=sec-0.5;
          Serial.print("sec : ");
          Serial.println(sec); 
          if(b==true){
            beeptop(b);
            b=false;
          }else{
            beeptop(b);
            b=true;
          }
          beepMillis=currentMillis;
        }
      }
    }
  //Serial.println(re);
  //Serial.println(fullcount);
  strip.show();
 
}


void lighttop(double bright){
  Serial.println("lighttop");
      strip.setPixelColor(0, 0, 0, 0, bright);
      strip.setPixelColor(1, 0, 0, 0, bright);
      strip.setPixelColor(2, 0, 0, 0, bright);
      strip.setPixelColor(3, 0, 0, 0, bright);
      strip.setPixelColor(4, 0, 0, 0, bright);
      strip.setPixelColor(5, 0, 0, 0, bright);
      strip.setPixelColor(6, 0, 0, 0, bright);
      strip.setPixelColor(7, 0, 0, 0, bright);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, 255-bright);
      strip.setPixelColor(9, 0, 0, 0, 255-bright);
      strip.setPixelColor(10, 0, 0, 0, 255-bright);
      strip.setPixelColor(11, 0, 0, 0, 255-bright);
      strip.setPixelColor(12, 0, 0, 0, 255-bright);
      strip.setPixelColor(13, 0, 0, 0, 255-bright);
      strip.setPixelColor(14, 0, 0, 0, 255-bright);
      strip.setPixelColor(15, 0, 0, 0, 255-bright);
}
void lightbottom(double bright){
  Serial.println("lightbottom");
      strip.setPixelColor(0, 0, 0, 0, 255-bright);
      strip.setPixelColor(1, 0, 0, 0, 255-bright);
      strip.setPixelColor(2, 0, 0, 0, 255-bright);
      strip.setPixelColor(3, 0, 0, 0, 255-bright);
      strip.setPixelColor(4, 0, 0, 0, 255-bright);
      strip.setPixelColor(5, 0, 0, 0, 255-bright);
      strip.setPixelColor(6, 0, 0, 0, 255-bright);
      strip.setPixelColor(7, 0, 0, 0, 255-bright);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, bright);
      strip.setPixelColor(9, 0, 0, 0, bright);
      strip.setPixelColor(10, 0, 0, 0, bright);
      strip.setPixelColor(11, 0, 0, 0, bright);
      strip.setPixelColor(12, 0, 0, 0, bright);
      strip.setPixelColor(13, 0, 0, 0, bright);
      strip.setPixelColor(14, 0, 0, 0, bright);
      strip.setPixelColor(15, 0, 0, 0, bright);
}
void lightready(double bright){
  Serial.println("lightready");
      strip.setPixelColor(0, 0, 0, 0, bright);
      strip.setPixelColor(1, 0, 0, 0, bright);
      strip.setPixelColor(2, 0, 0, 0, bright);
      strip.setPixelColor(3, 0, 0, 0, bright);
      strip.setPixelColor(4, 0, 0, 0, bright);
      strip.setPixelColor(5, 0, 0, 0, bright);
      strip.setPixelColor(6, 0, 0, 0, bright);
      strip.setPixelColor(7, 0, 0, 0, bright);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, bright);
      strip.setPixelColor(9, 0, 0, 0, bright);
      strip.setPixelColor(10, 0, 0, 0, bright);
      strip.setPixelColor(11, 0, 0, 0, bright);
      strip.setPixelColor(12, 0, 0, 0, bright);
      strip.setPixelColor(13, 0, 0, 0, bright);
      strip.setPixelColor(14, 0, 0, 0, bright);
      strip.setPixelColor(15, 0, 0, 0, bright);
}
void beepbottom (boolean b) {
  Serial.println("beepbottom");
  if( b == true){
    //Serial.println("beep bottom true");
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.setPixelColor(1, 0, 0, 0, 0);
      strip.setPixelColor(2, 0, 0, 0, 0);
      strip.setPixelColor(3, 0, 0, 0, 0);
      strip.setPixelColor(4, 0, 0, 0, 0);
      strip.setPixelColor(5, 0, 0, 0, 0);
      strip.setPixelColor(6, 0, 0, 0, 0);
      strip.setPixelColor(7, 0, 0, 0, 0);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, 255);
      strip.setPixelColor(9, 0, 0, 0, 255);
      strip.setPixelColor(10, 0, 0, 0, 255);
      strip.setPixelColor(11, 0, 0, 0, 255);
      strip.setPixelColor(12, 0, 0, 0, 255);
      strip.setPixelColor(13, 0, 0, 0, 255);
      strip.setPixelColor(14, 0, 0, 0, 255);
      strip.setPixelColor(15, 0, 0, 0, 255);
  }else if( b == false){
    //Serial.println("beep bottom false");
      strip.setPixelColor(0, 0, 0, 0, 255);
      strip.setPixelColor(1, 0, 0, 0, 255);
      strip.setPixelColor(2, 0, 0, 0, 255);
      strip.setPixelColor(3, 0, 0, 0, 255);
      strip.setPixelColor(4, 0, 0, 0, 255);
      strip.setPixelColor(5, 0, 0, 0, 255);
      strip.setPixelColor(6, 0, 0, 0, 255);
      strip.setPixelColor(7, 0, 0, 0, 255);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, 0);
      strip.setPixelColor(9, 0, 0, 0, 0);
      strip.setPixelColor(10, 0, 0, 0, 0);
      strip.setPixelColor(11, 0, 0, 0, 0);
      strip.setPixelColor(12, 0, 0, 0, 0);
      strip.setPixelColor(13, 0, 0, 0, 0);
      strip.setPixelColor(14, 0, 0, 0, 0);
      strip.setPixelColor(15, 0, 0, 0, 0); 
  }
}
void beeptop (boolean b) {
  Serial.println("beeptop");
  if( b == true){
    //Serial.println("beep top true");
      strip.setPixelColor(0, 0, 0, 0, 255);
      strip.setPixelColor(1, 0, 0, 0, 255);
      strip.setPixelColor(2, 0, 0, 0, 255);
      strip.setPixelColor(3, 0, 0, 0, 255);
      strip.setPixelColor(4, 0, 0, 0, 255);
      strip.setPixelColor(5, 0, 0, 0, 255);
      strip.setPixelColor(6, 0, 0, 0, 255);
      strip.setPixelColor(7, 0, 0, 0, 255);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, 0);
      strip.setPixelColor(9, 0, 0, 0, 0);
      strip.setPixelColor(10, 0, 0, 0, 0);
      strip.setPixelColor(11, 0, 0, 0, 0);
      strip.setPixelColor(12, 0, 0, 0, 0);
      strip.setPixelColor(13, 0, 0, 0, 0);
      strip.setPixelColor(14, 0, 0, 0, 0);
      strip.setPixelColor(15, 0, 0, 0, 0); 
  }else if( b == false){
    //Serial.println("beep top false");
      strip.setPixelColor(0, 0, 0, 0, 0);
      strip.setPixelColor(1, 0, 0, 0, 0);
      strip.setPixelColor(2, 0, 0, 0, 0);
      strip.setPixelColor(3, 0, 0, 0, 0);
      strip.setPixelColor(4, 0, 0, 0, 0);
      strip.setPixelColor(5, 0, 0, 0, 0);
      strip.setPixelColor(6, 0, 0, 0, 0);
      strip.setPixelColor(7, 0, 0, 0, 0);
      //==[↑bottom  ↓top]=================================
      strip.setPixelColor(8, 0, 0, 0, 255);
      strip.setPixelColor(9, 0, 0, 0, 255);
      strip.setPixelColor(10, 0, 0, 0, 255);
      strip.setPixelColor(11, 0, 0, 0, 255);
      strip.setPixelColor(12, 0, 0, 0, 255);
      strip.setPixelColor(13, 0, 0, 0, 255);
      strip.setPixelColor(14, 0, 0, 0, 255);
      strip.setPixelColor(15, 0, 0, 0, 255);
  }
}
