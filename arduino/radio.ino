#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String song_and_artist = "";
int song_and_artist_length = 0;
String station = "W-FUV: Alternate Side";
int station_length = 0;
int longest_string = 0;

int button = 7;
boolean current_button = LOW;

boolean move_right = LOW;
boolean move_left = LOW;
boolean scroll_right = LOW;
boolean scroll_left = LOW;

long previous_move_right = 0;
long previous_move_left = 0;
long previous_more_right = 0;
long previous_more_right = 0;

void setup()
{
  Serial.begin(9600);
  Serial.flush();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);    
 }

void loop()
{
  while (Serial.available())
  {
    lcd.clear();
    delay(10);
    char c = Serial.read();
      
    if (c == '~')
      {
        break;
      }
      song_and_artist += c;
      song_and_artist_length = song_and_artist.length();
      station_length = station.length();      
  }
   
   lcd.setCursor(0,0);
   lcd.print(song_and_artist);
   lcd.setCursor(0,1);
   lcd.print(station);
   song_and_artist = "";
   
   if(song_and_artist_length > station_length)
   {
     longest_string = song_and_artist_length;
   }
   
   if(station_length > song_and_artist_length)
   {
     longest_string = station_length;
   }
  
  if(longest_string > 16)
  {
    delay(1000);
    for (int positionCounter = 0; positionCounter < (longest_string - 16); positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    delay(1000);
    for (int positionCounter = 0; positionCounter < (longest_string - 16); positionCounter++) 
    {
      // scroll one position right:
      lcd.scrollDisplayRight();
      // wait a bit:
      delay(300);
    }
  }
   
  if (digitalRead(button) == HIGH) 
  {
    delay(60);
    if (digitalRead(button) == HIGH)
    {
      current_button = HIGH;
     }
  }
  
  if (current_button == HIGH)
  {
    current_button = LOW;
    Serial.println("Button Press!");
    delay(200);
  } 

}
