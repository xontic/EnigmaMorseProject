#include <SoftwareSerial.h>
#include <Bounce2.h>
#define buttonPin 5

Bounce button = Bounce();
SoftwareSerial SWSerial(2,4);

unsigned long buttonPressStartTimeStamp; //Variabel for når knapp trykkes
unsigned long buttonPressDuration; //Variabel for hvor lenge knapp er holdt nede
unsigned long buttonPause;
unsigned long buttonDivider;
int pausedot = 250; //En delay for dot morse
int pausedash = 750; //Delay for dash morse
char ch; //Variabel for hvilke bokstaver som blir skrevet
int lengde = 0; //Variabel for lengde av ordet som skrives
String code = ""; //String for hele ordet som blir skrevet i seriell output
int enigma1[] = {0, 200}; //Array for potmeter posisjon
int enigma2[] = {201, 400};
int enigma3[] = {401, 600};
int enigma4[] = {601, 800};
int enigma5[] = {801, 1000};
int* enigmaPos = enigma1; // * betyr at det er en peker, og pekeren peker på en array

void setup() { // flyttet settup fra bunnen til toppen
  Serial.begin(9600);
  SWSerial.begin(9600);
  // Setup the button with an internal pull-up
  pinMode(buttonPin, INPUT_PULLUP);
  button.attach(buttonPin);
  button.interval(20);//set debounce interval
}
void loop() {

int pot_value = analogRead(A0);


if(pot_value >= enigmaPos[0] && pot_value <= enigmaPos[1]){ //Sjekker om potmeter er i riktig posisjon for å skrive bokstav i morse kode 
  // Update the Bounce instance, does digitalRead of button
  button.update();

  // Button press transition from HIGH to LOW)
  if (button.fell())
  {
    buttonPressStartTimeStamp = millis();
    tone(3, 3000);
  }

  // Button release transition from LOW to HIGH) :
  if (button.rose())
  {
    buttonPressDuration = (millis() - buttonPressStartTimeStamp);
    buttonPause = millis();
    noTone(3);
  }

  if ((buttonPressDuration <= 200) && (buttonPressDuration > 10)){
    buttonPressDuration = 0;
    Serial.print(".");
  }

  else if (buttonPressDuration > 200){
    buttonPressDuration = 0;
    Serial.print("_");
  }

    if (button.fell())
  {
    buttonDivider = (millis() - buttonPause);
  }

  if(((buttonDivider) > 750) && ((buttonDivider) < 1750)){
    buttonPause = 0;
    buttonDivider = 0;
    Serial.print(" ");
  }

  else if((buttonDivider) >= 1750){
    buttonPause = 0;
    buttonDivider = 0;
    Serial.print("/ ");
  }


  if(Serial.available() > 0){
    code = Serial.readString();
    Serial.print(code);
    Serial.print("= ");
    Serial.println("");
    SWSerial.print(code);
    delay(1000);
    Ord2Morse();
  }

  if(SWSerial.available() > 0){
    code = SWSerial.readString();
    Serial.println("");
    Ord2Morse();
    Serial.println("");
  }
 } 

else{
  button.update();

  // Button press transition from HIGH to LOW)
  if (button.fell())
  {
    buttonPressStartTimeStamp = millis();
    tone(3, 3000);
  }

  // Button release transition from LOW to HIGH) :
  if (button.rose())
  {
    buttonPressDuration = (millis() - buttonPressStartTimeStamp);
    buttonPause = millis();
    noTone(3);
  }

  if ((buttonPressDuration <= 200) && (buttonPressDuration > 10)){
    buttonPressDuration = 0;
    Serial.print("-");
  }

  else if (buttonPressDuration > 200){
    buttonPressDuration = 0;
    Serial.print(".");
  }

    if (button.fell())
  {
    buttonDivider = (millis() - buttonPause);
  }

  if(((buttonDivider) > 750) && ((buttonDivider) < 1750)){
    buttonPause = 0;
    buttonDivider = 0;
    Serial.print("/");
  }

  else if((buttonDivider) >= 1750){
    buttonPause = 0;
    buttonDivider = 0;
    Serial.print("æ");
  }


  if(Serial.available() > 0){ // VAR KANSJE HER GARBO OG RANDOM SKULLE VÆRE   // endret til garbledMessage og randomMessage
    code = Serial.readString();
    Serial.print(code);
    Serial.print("= ");
    Serial.println("");
    SWSerial.print(code);
    delay(1000);
    garbledMessage(); // endret til garbledMessage
    Serial.println("");
    randomMessage(); // endret til randomMessage
  }

  if(SWSerial.available() > 0){
    code = SWSerial.readString();
    Serial.println("");
    garbledMessage(); // endret til garbledMessage
    Serial.println("");
    randomMessage(); // endret til randomMessage
    Serial.println("");
  }
}
}
//Funksjon for en dot i morsekode
void dot(){ 
  Serial.print(".");
  tone(3, 3000);
  delay(pausedot);
  noTone(3);
  delay(pausedot);
}

// function to generate a random morse character
char randomMorseChar() {
  char morseChars[36] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  char randomChar = morseChars[random(36)];
  return randomChar;
}

// function to generate a random morse message
void randomMessage() {
  for (int i = 0; i < lengde; i++) {
    lengde = code.length(); //
    Serial.print(randomMorseChar());
    Serial.print(" ");
    delay(50); 
  }
}

// funksjon for å generere en tilfeldig morse melding
void garbledMessage() {
  for (int i = 0; i < lengde; i++) {
    lengde = code.length();
    Serial.print(randomMorseChar());
    Serial.print(" ");
    delay(random(50, 500)); // til å legge til tilfeldig forsinkelse mellom hver karakter i morsekoden
  }
}

//Variabel for en dash i morsekode
void dash(){
Serial.print("_");
tone(3, 3000);
delay(pausedash);
noTone(3);
delay(pausedot);
}

//Nedover er bibliotek av alfabetet for transformering av tekst i seriell output til morse
void A(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void B(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void C(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void D(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void E(){
  dot();
  delay(pausedot*3);
}

void f(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}


void G(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void H(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void I(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void J(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void K(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void L(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void M(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void N(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void O(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void P(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void Q(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void R(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void S(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void T(){
  dash();
  delay(pausedot*3);
}

void U(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void V(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void W(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void X(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void Y(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void Z(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void one(){
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void two(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void three(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void four(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

void five(){
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void six(){
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void seven(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void eight(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void nine(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dot();
  delay(pausedot*3);
}

void zero(){
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot);
  dash();
  delay(pausedot*3);
}

//Funksjon for å lese hva som er skrevet inn i seriell output og sende ut morsekode for hver bokstav
void morse()
{
  if (ch == 'A' || ch == 'a')
  {
    A();
    Serial.print(" ");
  }
  else if (ch == 'B' || ch == 'b')
  {
    B();
    Serial.print(" ");
  }
  else if (ch == 'C' || ch == 'c')
  {
    C();
    Serial.print(" ");
  }
  else if (ch == 'D' || ch == 'd')
  {
    D();
    Serial.print(" ");
  }
  else if (ch == 'E' || ch == 'e')
  {
    E();
    Serial.print(" ");
  }
  else if (ch == 'F' || ch == 'f')
  {
    f();
    Serial.print(" ");
  }
  else if (ch == 'G' || ch == 'g')
  {
    G();
    Serial.print(" ");
  }
  else if (ch == 'H' || ch == 'h')
  {
    H();
    Serial.print(" ");
  }
  else if (ch == 'I' || ch == 'i')
  {
    I();
    Serial.print(" ");
  }
  else if (ch == 'J' || ch == 'j')
  {
    J();
    Serial.print(" ");
  }
  else if (ch == 'K' || ch == 'k')
  {
    K();
    Serial.print(" ");
  }
  else if (ch == 'L' || ch == 'l')
  {
    L();
    Serial.print(" ");
  }
  else if (ch == 'M' || ch == 'm')
  {
    M();
    Serial.print(" ");
  }
  else if (ch == 'N' || ch == 'n')
  {
    N();
    Serial.print(" ");
  }
  else if (ch == 'O' || ch == 'o')
  {
    O();
    Serial.print(" ");
  }
  else if (ch == 'P' || ch == 'p')
  {
    P();
    Serial.print(" ");
  }
  else if (ch == 'Q' || ch == 'q')
  {
    Q();
    Serial.print(" ");
  }
  else if (ch == 'R' || ch == 'r')
  {
    R();
    Serial.print(" ");
  }
  else if (ch == 'S' || ch == 's')
  {
    S();
    Serial.print(" ");
  }
  else if (ch == 'T' || ch == 't')
  {
    T();
    Serial.print(" ");
  }
  else if (ch == 'U' || ch == 'u')
  {
    U();
    Serial.print(" ");
  }
  else if (ch == 'V' || ch == 'v')
  {
    V();
    Serial.print(" ");
  }
  else if (ch == 'W' || ch == 'w')
  {
    W();
    Serial.print(" ");
  }
  else if (ch == 'X' || ch == 'x')
  {
    X();
    Serial.print(" ");
  }
  else if (ch == 'Y' || ch == 'y')
  {
    Y();
    Serial.print(" ");
  }
  else if (ch == 'Z' || ch == 'z')
  {
    Z();
    Serial.print(" ");
  }
  else if (ch == '0')
  {
    zero();
    Serial.print(" ");
  }
  else if (ch == '1')
  {
    one();
    Serial.print(" ");
  }
  else if (ch == '2')
  {
    two();
    Serial.print(" ");
  }
  else if (ch == '3')
  {
    three();
    Serial.print(" ");
  }
  else if (ch == '4')
  {
    four();
    Serial.print(" ");
  }
  else if (ch == '5')
  {
    five();
    Serial.print(" ");
  }
  else if (ch == '6')
  {
    six();
    Serial.print(" ");
  }
  else if (ch == '7')
  {
    seven();
    Serial.print(" ");
  }
  else if (ch == '8')
  {
    eight();
    Serial.print(" ");
  }
  else if (ch == '9')
  {
    nine();
    Serial.print(" ");
  }
  else if(ch == ' ')
  {
    delay(pausedot*7);
    Serial.print("/ ");
  }


}

//Funksjon for å transformere ord til morsekode, tar en og en bokstav og gjør om til morse ved hjelp av morse funksjonen
void Ord2Morse(){
  lengde = code.length();
  for (int i = 0; i < lengde; i++){
    ch = code.charAt(i);
    morse();
  }

}
