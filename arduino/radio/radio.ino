#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String read_string = "";

String line1 = "Internet Radio";
int line1_length = line1.length();
String line2 = "   Phil Gonzalez";
int line2_length = line2.length();

int longest_string = 0;

int next_button = 7;
boolean current_next_button = LOW;

int disp_button = 8;
boolean current_disp_button = LOW;


int left_scroll_counter = 0;
int right_scroll_counter = 0;
long previous_move_right = 0;
long previous_move_left = 0;
long previous_scroll_right = 0;
long previous_scroll_left = 0;

String new_info = "null";

boolean move_right = LOW;
boolean move_left = LOW;

void setup()
{
  Serial.begin(9600);
  Serial.flush();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
 
  if (line1_length >= line2_length)
  {
    longest_string = line1_length;
  }
  if (line2_length > line1_length)
  {
    longest_string = line2_length;
  }
  //Serial.println(longest_string);
}

void loop()
{
  unsigned long current_millis = millis();
  while (Serial.available())
  {
    lcd.clear();
    delay(10);
    char c = Serial.read();
      
    if (c == '~')
      {
        line1 = read_string;
        line1_length = line1.length();
        read_string = "";
        if (line1_length >= line2_length)
        {
          longest_string = line1_length;
        }
        if (line2_length > line1_length)
        {
          longest_string = line2_length;
        }
        break;
      }
    if ( c == '`')
      {
        line2 = read_string;
        line2_length = line2.length();
        read_string = "";
        if (line1_length >= line2_length)
        {
          longest_string = line1_length;
        }
        if (line2_length > line1_length)
        {
          longest_string = line2_length;
        }
        break;
      }
    read_string += c;
    
    //line1_length = line1.length();
    //line2_length = line2.length();
      
    new_info = "Yes";
    move_right = LOW;
    move_left = LOW;
    left_scroll_counter = 0;
    right_scroll_counter = 0;
      
      
    //if(line1_length >= line2_length)
    //{
     //longest_string = line1_length;
     //Serial.println(line1_length);
    //}
   
    //if(line2_length > line1_length)
    //{
     //longest_string = line2_length;
     //Serial.println(line1_length);
    //}
  
  }
   
   lcd.setCursor(0,0);
   lcd.print(line1);
   lcd.setCursor(0,1);
   lcd.print(line2);
   //read_string = "";
   
  
  if(longest_string > 16) 
  {
    if (new_info == "Yes")
    {
      
      //Serial.println(left_scroll_counter);
      //Serial.println(right_scroll_counter);
      new_info = "Yes2";
      previous_move_left = current_millis;
    }
    if ((current_millis - previous_move_left > 1000) && (new_info == "Yes2"))
    {
      move_left = HIGH;
      new_info = "No";
    }
  }
      
  if ((move_left == HIGH) && (new_info == "No") && (longest_string > 16))
  {
    if (current_millis - previous_scroll_left > 300)
      {
        if (left_scroll_counter < (longest_string - 16))
        {
        //Serial.println(left_scroll_counter - (longest_string - 16));
        lcd.scrollDisplayLeft();
        previous_scroll_left = current_millis;
        left_scroll_counter = left_scroll_counter + 1;
        }
        else
        {
          move_left = LOW;
          left_scroll_counter = 0;
          new_info = "No2";
        }
      }
  }
  
  if ((move_left == LOW) && (new_info == "No2") && (longest_string > 16))
  {
    new_info = "No3";
    previous_move_right = current_millis;
    //Serial.println("we got it");
  }
  if ((current_millis - previous_move_right > 1000) && (new_info == "No3") && (move_left == LOW))
  {
    //Serial.println("again");
    move_right = HIGH;
    new_info = "No4";
  }
  
  if ((move_right == HIGH) && (new_info == "No4"))
  {
    //Serial.println("we got it");
    if (current_millis - previous_scroll_right > 300)
      {
        if (right_scroll_counter < (longest_string - 16))
        {
        //Serial.println(right_scroll_counter - (longest_string - 16));
        lcd.scrollDisplayRight();
        previous_scroll_right = current_millis;
        right_scroll_counter = right_scroll_counter + 1;
        }
        else
        {
          move_right = LOW;
          right_scroll_counter = 0;
          new_info = "No5"; 
        }
      }
  }
  
  if ((move_right == LOW) && (new_info == "No5"))
  {
    new_info = "Yes2";
    previous_move_left = current_millis;
  }
  if ((current_millis - previous_move_left > 1000) && (new_info == "Yes2"))
  {
    move_left = HIGH;
    new_info = "No";
  }
  
  if (digitalRead(next_button) == HIGH) 
  {
    delay(20);
    if (digitalRead(next_button) == HIGH)
    {
      current_next_button = HIGH;
     }
  }
  
  if (current_next_button == HIGH)
  {
    current_next_button = LOW;
    Serial.println("Next!");
    delay(200);
  }
 
  if (digitalRead(disp_button) == HIGH) 
  {
    delay(60);
    if (digitalRead(disp_button) == HIGH)
    {
      current_disp_button = HIGH;
     }
  }
  
  if (current_disp_button == HIGH)
  {
    current_disp_button = LOW;
    //Serial.println("Display!");
    Serial.println("Tweet!");
    delay(200);
  }  

}
