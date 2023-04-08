#include <Key.h>
#include <Keypad.h>

#include <LiquidCrystal.h>

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

//hardware connections
byte pin_rows[ROW_NUM] = {22, 23, 24, 25}; 
byte pin_column[COLUMN_NUM] = {26, 27, 28, 29}; 

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

int buzzer = 3;

#define trigger 5
#define echo 6

const int button = 2; //number of button pin
bool pressed = true; //for button
char *loc[] = {"      ", "HUB   ", "SSC   ", "Rivera", "Orbach"};
int locIndex = 0;

const int totalSeatsHUB = 60;
char *seatsHUB[totalSeatsHUB];
int indexHUB = 0;
int availHUB = 60;
int resHUB = 0;
int takenHUB = 0;
long *seatsCode1HUB[totalSeatsHUB];
long *seatsCode2HUB[totalSeatsHUB];
long *seatsCode3HUB[totalSeatsHUB];

const int totalSeatsSSC = 100;
char *seatsSSC[totalSeatsSSC];
int indexSSC = 0;
int availSSC = 100;
int resSSC = 0;
int takenSSC = 0;
long *seatsCode1SSC[totalSeatsSSC];
long *seatsCode2SSC[totalSeatsSSC];
long *seatsCode3SSC[totalSeatsSSC];

const int totalSeatsRivera = 150;
char *seatsRivera[totalSeatsRivera]; 
int indexRivera = 0;
int availRivera = 150;
int resRivera = 0;
int takenRivera = 0;
long *seatsCode1Rivera[totalSeatsRivera];
long *seatsCode2Rivera[totalSeatsRivera];
long *seatsCode3Rivera[totalSeatsRivera];

const int totalSeatsOrbach = 150;
char *seatsOrbach[totalSeatsOrbach]; 
int indexOrbach = 0;
int availOrbach = 150;
int resOrbach = 0;
int takenOrbach = 0;
long *seatsCode1Orbach[totalSeatsOrbach];
long *seatsCode2Orbach[totalSeatsOrbach];
long *seatsCode3Orbach[totalSeatsOrbach];

long code1;
long code2;
long code3;
int seatCodeIndexHUB = 0;
int seatCodeIndexSSC = 0;
int seatCodeIndexRivera = 0;
int seatCodeIndexOrbach = 0;

int duration;
int distance;
int certainSeatTaken = 0;

//part of the display for the main menu
void printSeatsLCD(){
  if(loc[locIndex] == "HUB   "){
    lcd.print("A:");
    lcd.print(availHUB);
    lcd.print(" ");
    lcd.print("R:");
    lcd.print(resHUB);
    lcd.print(" ");
    lcd.print("T:");
    lcd.print(takenHUB);
  }
  else if(loc[locIndex] == "SSC   "){
    lcd.print("A:");
    lcd.print(availSSC);
    lcd.print(" ");
    lcd.print("R:");
    lcd.print(resSSC);
    lcd.print(" ");
    lcd.print("T:");
    lcd.print(takenSSC);
  }
  else if(loc[locIndex] == "Rivera"){
    lcd.print("A:");
    lcd.print(availRivera);
    lcd.print(" ");
    lcd.print("R:");
    lcd.print(resRivera);
    lcd.print(" ");
    lcd.print("T:");
    lcd.print(takenRivera);
  }
  else if(loc[locIndex] == "Orbach"){
    lcd.print("A:");
    lcd.print(availOrbach);
    lcd.print(" ");
    lcd.print("R:");
    lcd.print(resOrbach);
    lcd.print(" ");
    lcd.print("T:");
    lcd.print(takenOrbach);
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
}

//For testing purposes
/*void printSeats(){
  Serial.println("HUB:");
  for(int i = 0; i < totalSeatsHUB; i++){
    Serial.println(i);
    Serial.print(seatsHUB[i]);
  }
  Serial.println("SSC:");
  for(int j = 0; j < totalSeatsSSC; j++){
    Serial.println(j);
    Serial.print(seatsSSC[j]);
  }
  Serial.println("Rivera:");
  for(int k = 0; k < totalSeatsRivera; k++){
    Serial.println(k);
    Serial.print(seatsRivera[k]);
  }
  Serial.println("Orbach:");
  for(int n = 0; n < totalSeatsOrbach; n++){
    Serial.println(n);
    Serial.print(seatsOrbach[n]);
  }
}*/

//increment location index to either go to next location or back to first location
void incrementLocIndex(){
  if(locIndex < 4){
    locIndex++;
  }
  else{ //go back to start
    locIndex = 1;
  }
}

//generates the unique code for a reserved seat
void generateUniqueCode(int seatNum){
  //lcd.print(seatNum);
  delay(2000);
  code1 = random(0, 9);
  code2 = random(0, 9);
  code3 = random(0, 9);
  lcd.clear();
  lcd.print("Seat ");
  //lcd.print(seatNum);
  lcd.print("code:");
  lcd.print(code1);
  lcd.print(code2);
  lcd.print(code3);
  delay(3000);
}

int codeIndexHUB = 0;
int codeIndexSSC = 0;
int codeIndexRivera = 0;
int codeIndexOrbach = 0;
//when a person reserves a seat
//it's no longer available, it's reserved
//a unique code is produced and shown to the user
void ReserveSeat(){
  lcd.clear();
  lcd.print("Reserved seat");
  int seatNum;
  if(loc[locIndex] == "HUB   "){
    availHUB--;
    resHUB++;
    generateUniqueCode(seatNum);
    seatsCode1HUB[codeIndexHUB] = code1;
    seatsCode2HUB[codeIndexHUB] = code2;
    seatsCode3HUB[codeIndexHUB] = code3;
    codeIndexHUB++;
  }
  else if(loc[locIndex] == "SSC   "){
    availSSC--;
    resSSC++;
    generateUniqueCode(seatNum);
    seatsCode1SSC[codeIndexSSC] = code1;
    seatsCode2SSC[codeIndexSSC] = code2;
    seatsCode3SSC[codeIndexSSC] = code3;
    codeIndexSSC++;
  }
  else if(loc[locIndex] == "Rivera"){
    availRivera--;
    resRivera++;
    generateUniqueCode(seatNum);
    seatsCode1Rivera[codeIndexRivera] = code1;
    seatsCode2Rivera[codeIndexRivera] = code2;
    seatsCode3Rivera[codeIndexRivera] = code3;
    codeIndexRivera++;
  }
  else if(loc[locIndex] == "Orbach"){
    availOrbach--;
    resOrbach++;
    generateUniqueCode(seatNum);
    seatsCode1Orbach[codeIndexOrbach] = code1;
    seatsCode2Orbach[codeIndexOrbach] = code2;
    seatsCode3Orbach[codeIndexOrbach] = code3;
    codeIndexOrbach++;
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
  delay(1000);
  lcd.clear();
  lcd.print("Location: ");
  lcd.print(loc[locIndex]);
  //print on second line of LCD Display
  lcd.setCursor(0, 2);
  printSeatsLCD();
  delay(1000);
}

//when the person chooses to give up their reserved seat
//it's no longer reserved, it's available
void GiveUpReserve(){
  if(loc[locIndex] == "HUB   "){
    if(resHUB > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("reserved seat");
      availHUB++;
      resHUB--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "SSC   "){
    if(resSSC > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("reserved seat");
      availSSC++;
      resSSC--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "Rivera"){
    if(resRivera > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("reserved seat");
      availRivera++;
      resRivera--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "Orbach"){
    if(resOrbach > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("reserved seat");
      availOrbach++;
      resOrbach--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
  delay(1000);
  lcd.clear();
  lcd.print("Location: ");
  lcd.print(loc[locIndex]);
  //print on second line of LCD Display
  lcd.setCursor(0, 2);
  printSeatsLCD();
  delay(1000);
}

//when a person sits on a seat
//it's no longer available, it's taken
void takeSeat(){
  if(loc[locIndex] == "HUB   "){
    if(availHUB > 0){
      lcd.clear();
      lcd.print("Seat taken");
      availHUB--;
      takenHUB++;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "SSC   "){
    if(availSSC > 0){
      lcd.clear();
      lcd.print("Seat taken");
      availSSC--;
      takenSSC++;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "Rivera"){
    if(availRivera > 0){
      lcd.clear();
      lcd.print("Seat taken");
      availRivera--;
      takenRivera++;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else if(loc[locIndex] == "Orbach"){
    if(availOrbach > 0){
      lcd.clear();
      lcd.print("Seat taken");
      availOrbach--;
      takenOrbach++;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
    }
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
  delay(1000);
  lcd.clear();
  lcd.print("Location: ");
  lcd.print(loc[locIndex]);
  //print on second line of LCD Display
  lcd.setCursor(0, 2);
  printSeatsLCD();
  delay(1000);
}

//when person stands up from seat
//that seat is no longer taken, it's available
void GiveUpTaken(){
  if(loc[locIndex] == "HUB   "){
    if(takenHUB > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("taken seat");
      availHUB++;
      takenHUB--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
      delay(1000);
    }
  }
  else if(loc[locIndex] == "SSC   "){
    if(takenSSC > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("taken seat");
      availSSC++;
      takenSSC--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
      delay(1000);
    }
  }
  else if(loc[locIndex] == "Rivera"){
    if(takenRivera > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("taken seat");
      availRivera++;
      takenRivera--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
      delay(1000);
    }
  }
  else if(loc[locIndex] == "Orbach"){
    if(takenOrbach > 0){
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      delay(500);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      lcd.clear();
      lcd.print("Gave up");
      lcd.setCursor(0,2);
      lcd.print("taken seat");
      availOrbach++;
      takenOrbach--;
    }
    else{
      lcd.clear();
      lcd.print("Invalid");
      delay(1000);
    }
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
  delay(1000);
  lcd.clear();
  lcd.print("Location: ");
  lcd.print(loc[locIndex]);
  //print on second line of LCD Display
  lcd.setCursor(0, 2);
  printSeatsLCD();
  delay(1000);
}

//makes sure user's input code is correct in order for a seat to be taken
void checkCode(){
  char codeKey1 = keypad.waitForKey();
  String tempKey1 = String(codeKey1);
  long key1Long = tempKey1.toInt();
  if(loc[locIndex] == "HUB   "){
    for(int i = 0; i < seatsCode1HUB; i++){
      if(key1Long == seatsCode1HUB[i]){
        lcd.print("*");
          char codeKey2 = keypad.waitForKey();
          String tempKey2 = String(codeKey2);
          long key2Long = tempKey2.toInt();
          if(key2Long == seatsCode2HUB[i]){
            lcd.print("*");
              char codeKey3 = keypad.waitForKey();
              String tempKey3 = String(codeKey3);
              long key3Long = tempKey3.toInt();
              if(key3Long == seatsCode3HUB[i]){
                lcd.print("*");
                if(resHUB > 0){
                  resHUB--;
                  takenHUB++;
                  lcd.clear();
                  lcd.print("Seat ");
                  lcd.print("taken");
                  delay(1000);
                }
                else{
                  lcd.clear();
                  lcd.print("Invalid");
                  delay(1000);
                }
                lcd.clear();
                lcd.print("Location: ");
                lcd.print(loc[locIndex]);
                //print on second line of LCD Display
                lcd.setCursor(0, 2);
                printSeatsLCD();
                delay(1000);
                return;
              }
         }
     }
    }
  }
  else if(loc[locIndex] == "SSC   "){
    for(int i = 0; i < seatsCode1SSC; i++){
      if(key1Long == seatsCode1SSC[i]){
        lcd.print("*");
          char codeKey2 = keypad.waitForKey();
          String tempKey2 = String(codeKey2);
          long key2Long = tempKey2.toInt();
          if(key2Long == seatsCode2SSC[i]){
            lcd.print("*");
              char codeKey3 = keypad.waitForKey();
              String tempKey3 = String(codeKey3);
              long key3Long = tempKey3.toInt();
              if(key3Long == seatsCode3SSC[i]){
                lcd.print("*");
                if(resSSC > 0){
                  resSSC--;
                  takenSSC++;
                  lcd.clear();
                  lcd.print("Seat ");
                  lcd.print("taken");
                  delay(1000);
                }
                else{
                  lcd.clear();
                  lcd.print("Invalid");
                  delay(1000);
                }
                lcd.clear();
                lcd.print("Location: ");
                lcd.print(loc[locIndex]);
                //print on second line of LCD Display
                lcd.setCursor(0, 2);
                printSeatsLCD();
                delay(1000);
                return;
              }
         }
     }
    }
  }
  else if(loc[locIndex] == "Rivera"){
    for(int i = 0; i < seatsCode1Rivera; i++){
      if(key1Long == seatsCode1Rivera[i]){
        lcd.print("*");
          char codeKey2 = keypad.waitForKey();
          String tempKey2 = String(codeKey2);
          long key2Long = tempKey2.toInt();
          if(key2Long == seatsCode2Rivera[i]){
            lcd.print("*");
              char codeKey3 = keypad.waitForKey();
              String tempKey3 = String(codeKey3);
              long key3Long = tempKey3.toInt();
              if(key3Long == seatsCode3Rivera[i]){
                lcd.print("*");
                if(resRivera > 0){
                  resRivera--;
                  takenRivera++;
                  lcd.clear();
                  lcd.print("Seat ");
                  lcd.print("taken");
                  delay(1000);
                }
                else{
                  lcd.clear();
                  lcd.print("Invalid");
                  delay(1000);
                }
                lcd.clear();
                lcd.print("Location: ");
                lcd.print(loc[locIndex]);
                //print on second line of LCD Display
                lcd.setCursor(0, 2);
                printSeatsLCD();
                delay(1000);
                return;
              }
         }
     }
    }
  }
  else if(loc[locIndex] == "Orbach"){
    for(int i = 0; i < seatsCode1Orbach; i++){
      if(key1Long == seatsCode1Orbach[i]){
        lcd.print("*");
          char codeKey2 = keypad.waitForKey();
          String tempKey2 = String(codeKey2);
          long key2Long = tempKey2.toInt();
          if(key2Long == seatsCode2Orbach[i]){
            lcd.print("*");
              char codeKey3 = keypad.waitForKey();
              String tempKey3 = String(codeKey3);
              long key3Long = tempKey3.toInt();
              if(key3Long == seatsCode3Orbach[i]){
                lcd.print("*");
                if(resOrbach > 0){
                  resOrbach--;
                  takenOrbach++;
                  lcd.clear();
                  lcd.print("Seat ");
                  lcd.print("taken");
                  delay(1000);
                }
                else{
                  lcd.clear();
                  lcd.print("Invalid");
                  delay(1000);
                }
                lcd.clear();
                lcd.print("Location: ");
                lcd.print(loc[locIndex]);
                //print on second line of LCD Display
                lcd.setCursor(0, 2);
                printSeatsLCD();
                delay(1000);
                return;
              }
         }
     }
    }
  }
  else{ //loc[locIndex] == "      "
    //do nothing
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //set up lcd cols and rows
  lcd.begin(16, 2);
  
  //initialize button pin as an input
  pinMode(button, INPUT_PULLUP);

  //initialize buzzer pin as output
  pinMode(buzzer,OUTPUT);


  //initialize ultrasonic sensor trigger and echo pins
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  //Testing purposes
  //printSeats();

  //for randomness of unique code for reserved seats
  randomSeed(analogRead(0));
  
}

void showMenu(bool currentState){
  if(currentState == pressed){
    lcd.clear();
    lcd.print("Location: ");
    incrementLocIndex();
    lcd.print(loc[locIndex]);
    //print on second line of LCD Display
    lcd.setCursor(0, 2);
    printSeatsLCD();
    
    while(digitalRead(button) == pressed){
    //Do nothing while button is still pressed
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //for active buzzer
  int buzz = 0;

  //Ultrasonic Sensor feature
  //buzzer sounds once when seat is successfully taken
  //twice when person stands up
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);

  //gives you time for waves to hit something and come back
  duration = pulseIn(echo, HIGH);
  //divide by 2 bc it’s the time to travel there and back
  distance = 0.034 * (duration / 2);

  //somebody just sat down on a certain seat
  //ideally, there would be 1 sensor for every seat
  //for demo, the ultrasonic sensor used is the sensor for 1 certain seat
  Serial.println(distance);
  if(distance <= 10 && !certainSeatTaken){ 
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    takeSeat();
    certainSeatTaken = 1;
  }

  //the person got up, seat is available now
  if(distance > 10 && certainSeatTaken){
    GiveUpTaken();
    certainSeatTaken = 0;
  }

  //for button presses to switch locations
  bool currentState = digitalRead(button);

  //keypad presses
  char key = keypad.getKey();
    if(key == 'A'){ //want to reserve a seat
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(buzzer, LOW);
      ReserveSeat();
    }
    if(key == 'B'){ //want to give up reserved seat
      GiveUpReserve();
    }
    if(key == 'C'){ //decrements from taken seats, increments available seats
      GiveUpTaken();
    }
    if(key == 'D'){ //if you mess up inputting the code, press 'D' and start again
      lcd.clear();
      lcd.print("Location: ");
      lcd.print(loc[locIndex]);
      //print on second line of LCD Display
      lcd.setCursor(0, 2);
      printSeatsLCD();
      delay(1000);
    }
    if(key == '*'){ //want to put in code for a reserved seat
      lcd.clear();
      lcd.print("Enter code:");
      checkCode();
    }

  showMenu(currentState);
  //Press button to select current location feature
  currentState = digitalRead(button);
  showMenu(currentState);
  
}

