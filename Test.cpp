
#define p1m1 2
#define p2m1 3
#define p1m2 4
#define p2m2 5
struct p{
  int x,y,z,m;
  };
void setup() {
  // put your setup code here, to run once:
pinMode(p1m1,OUTPUT);
pinMode(p2m1,OUTPUT);
pinMode(p1m2,OUTPUT);
pinMode(p2m2,OUTPUT);
p ponit;
ponit.x = 10;

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(p1m1,HIGH);
  digitalWrite(p2m1,LOW);
  digitalWrite(p1m2,LOW);
  digitalWrite(p2m2,HIGH);
  digitalWrite(10,255);
  digitalWrite(11,255);
  
  

}
