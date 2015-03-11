//PINS MAPPING
const byte readyPin=9; //H-not ready   L-ready
const byte penPin=10; //H-dole L-nahore
const byte upLeftPin=11; //H-nahoru/doleva L-dolu/doprava
const byte XYPin=12; // H-Y L-X
const byte movePin=13; //pohyb na nabeznou hranu

//MAX SIZE IN STEPS
const int MAX_X=1920;
const int MAX_Y=2650;

//STEP DELAY
const byte DELAY_FAST=5;
const byte DELAY_SLOW=50;

//WRITING VARIABLES
int currentX=0;
int currentY=0;
byte absolute=1;
byte isPenDown=0;
const float SCALE=1;

//READING VARIABLES
char buffer[30]={0};
int readed=0;
byte commInBuff=0;
int readI;

//MOVE X AXE
void moveX(int stepC,int delayMilis){
  int i=0;  
  int inc;
  digitalWrite(XYPin,LOW);
  if(stepC>0){
    digitalWrite(upLeftPin,LOW);
    inc=1;
  }else{
    digitalWrite(upLeftPin,HIGH);
    stepC=stepC*(-1);
    inc=-1;
  }  
  
  digitalWrite(movePin,LOW);
  for(i=0;i<stepC && currentX<MAX_X && currentX>0 ;i++){
    digitalWrite(movePin,HIGH);
    delay(1);
    digitalWrite(movePin,LOW);
    delay(delayMilis);
    currentX+=inc;
  }  
}

//MOVE Y AXE
void moveY(int stepC,int delayMilis){
  int i=0;  
  int inc;  
  digitalWrite(XYPin,HIGH);
  if(stepC>0){
    digitalWrite(upLeftPin,LOW);
    inc=1;
  }else{
    digitalWrite(upLeftPin,HIGH);
    stepC=stepC*(-1);
    inc=-1;
  }  
  
  digitalWrite(movePin,LOW);
  for(i=0;i<stepC && currentY<MAX_Y && currentY>0 ;i++){
    digitalWrite(movePin,HIGH);
    delay(1);
    digitalWrite(movePin,LOW);
    delay(delayMilis);
    currentY+=inc;
  }  
}

//IF PEN IS LEFT
byte isReady(){
  pinMode(readyPin, OUTPUT);
  digitalWrite(readyPin,LOW);
  pinMode(readyPin, INPUT);
  delayMicroseconds(10);
  return !digitalRead(readyPin);
}

//get pen left
void getReady(){
  penUp();
  while(!isReady()){
    moveX(-1,DELAY_FAST);
  }  
}

//set Origin
void setOrigin(){
  currentX=0;
  currentY=0;
}

//get pen home
void goHome(){
  penUp();
  moveX(-currentX,DELAY_FAST);
  moveY(-currentY,DELAY_FAST);
}

void penUp(){
  digitalWrite(penPin,LOW);  
  isPenDown=0;
}

void penDown(){
  digitalWrite(penPin,HIGH);
  isPenDown=1;
}

//draw realtive line
void drawLine(int dx, int dy){
  float dyx;
  int mark;
  int i;
  float my=0;
  int penDelay;
  
  if(isPenDown){
    penDelay=DELAY_SLOW;
  }else{
    penDelay=DELAY_FAST;
  }  
  
  if(dx==0){
    moveY(dy,DELAY_FAST);
  }else
  if(dy==0){
    moveX(dx,DELAY_FAST);
  }else{
    dyx=dy/dx;
    if(dx>0){
      mark=1;
    }else{
      mark=-1;
      dx=-1*dx;      
    }
    for(i=0;i<dx;i++){
      moveX(mark,penDelay);
      my+=mark*dyx;
      moveY((int)my,penDelay);
      my-=(int)my;
    }
  }
}
////////////////////////////////////////////////////////////
/////////////////           SETUP            ///////////////
////////////////////////////////////////////////////////////
void setup() {
  pinMode(penPin,OUTPUT);
  pinMode(upLeftPin,OUTPUT);
  pinMode(XYPin,OUTPUT);
  pinMode(movePin,OUTPUT);
  
  Serial.begin(9600);  
}

void skipSpace(){
  while(buffer[readI]==' '){readI++;}
}

void loop() {
  if(Serial.available()){
      buffer[readed]=Serial.read();
      if(buffer[readed]==';'){
        commInBuff=1;
      }
      readed++;
  }
  if(commInBuff){
    Serial.println("Command Accepted");     
    readI=0;      
    skipSpace();
    //PU - pen up
    if(buffer[readI]=='P' && buffer[readI+1]=='U'){
      readI+=2;
      penUp();
      Serial.println("PU-pen up");
      skipSpace();
      if(buffer[readI]~=';'){
      
      }            
    }
    
    
    
    for(readI=0;readI<readed-1;readI++){
      buffer[readed]=0;    
    }  
    readed=0;
    commInBuff=0;    
  }
}






















