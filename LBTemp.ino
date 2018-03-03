#include <MsTimer2.h>

class LedControl{
  private:
  byte  *pins;
  byte *comPins;
  byte numbers[10] = { B11111100, B11000000, B10011111, B11001110, B11100010,
  B01101110, B01111110, B11000100, B11111110, B11101110}; 
  bool isAnnode;
  int8_t forPINS;
  int8_t forCOMs;
  byte number[5]={0,0,0,0,0};
 
  void parseToLED(int num){
    
    for(int i=4;i>=0;i--){
      number[i] = num%10;
      num = num/10;
    } 
    
  }
 
  void showNumber(int num){
    for(int i=0;i<7;i++){ //Аннод - лоу ,катод - хай
      if(bitRead(numbers[num],7-i)==forPINS){
      digitalWrite(pins[i],HIGH);
      }
      else{
        digitalWrite(pins[i],LOW);
      }
    } 
  }
  void changeComPin(){
    static byte i=0;
    static bool OFF=(isAnnode)?LOW:HIGH;
    for(int j=0;j<5;j++){
      if(j!=i){
          digitalWrite(comPins[j],OFF);
      }
      else{
        digitalWrite(comPins[j],forCOMs);
      }
    }
    if(++i==5){
      i=0;
    }
  }
  public:
  LedControl(byte *pins ,byte *comPins,bool isAnnodCom ){
     this->pins  = new byte[7];
     this->comPins = new byte[5];
     for(byte i=0;i<7;i++){
        this->pins[i] = pins[i]; 
     }
     for(byte i=0;i<5;i++){
        this->comPins[i] =comPins[i]; 
     }
 
    isAnnode=isAnnodCom;
    if(isAnnode){
      forPINS=LOW;
      forCOMs=HIGH;
    }
    else {
      forPINS=HIGH;
      forCOMs=LOW;
    }
    
  }
  
  void showValue(int num)
  {   
      parseToLED(num);
  
    for(int i=0;i<5;i++){

        showNumber(number[i]);
        
        changeComPin();
        delay(5);
        
    }
    
    
  }
  
};


class TempSensor{
  private:
  byte readPin;
  public:
  TempSensor(byte readPin){
    this->readPin=readPin;
 
  }  
  float getValue(){
    int reading = analogRead(readPin);
    float voltage = reading * 5.0; 
    voltage /= 1024.0;  
    float temperatureC = (voltage - 0.5) * 100 ;
    
    return temperatureC;
  }
};


// переменная для хранения значения текущей цифры

 int value = 1234;
  // byte  pins[7]={2,3,4,5,6,7,8};
  //byte comPins[5]={9,10,11,12,13};

   byte  pins[7]={2,3,4,5,6,7,8};
  byte comPins[5]={10,11,12,13,9};//9-посл 10 е
 // byte comPins[5]={10,11,12,13,9};//9-посл 10 е

LedControl *lc;
TempSensor *temp;
void updateValue(){
    value= temp->getValue();
    
}

void setup()
{   
    
    for(int i=0;i<7;i++){  
        pinMode(pins[i],OUTPUT);
      
    }
    
   
    
    for(int i=0;i<5;i++){
       
    
      pinMode(comPins[i],OUTPUT);
      
    }
    
     
    
 
    lc = new LedControl(pins,comPins,false);
    temp = new TempSensor(A1);
    MsTimer2::set(500, updateValue); // задаем период прерывания по таймеру 500 мс
    MsTimer2::start();   
// Сконфигурировать контакты как выходы

}
void loop()
{
  lc->showValue(value);
}
