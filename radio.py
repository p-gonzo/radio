# coding=utf-8 
import serial # imports serial module
import time # for the delay between mail checks  
import os # to search for the arduino
import difflib # to search for the arduino
from time import gmtime, strftime

#sets a null for current track to be checked against the info comming in from the MPC

# lists evertying in the /dev derectory
connected_devices = os.listdir('/dev') 

#sets the arduino as a string that is typically asigned by a mac (the 1 restricts the number of responses)
arduino = str(difflib.get_close_matches('ttyACM', connected_devices, 1))
#remove filler on the string and add a path
arduino = arduino.strip('[]')
arduino = arduino.strip("''")
arduino = '/dev/' + arduino

#---------------------------------------------------------------------------
line1 = ""
line2 = ""
arduino_line1 = ""
arduino_line2 = ""
current_track = "none"
current_time = ""
weather = ""
current_display = "music"
last_checked = "On Five"

#---------------------------------------------------------------------------

#serail is connected to our derived arduino variable
ser = serial.Serial(arduino)
print "Starting on " +ser.portstr;
time.sleep(3)

#Open the serialport up, send it a dummy string  (The dummy is needed b/c the first string sent never displays)
ser.write(str("Starting...~"))
print "Initial string sent."

#---------------------------------------------------------------------------

def interval_check():
    if strftime("%S", gmtime())[-1] == '0':
        last_checked = "On Zero"
    if strftime("%S", gmtime())[-1] == '5':
        last_checked = "On Five" 


def get_song_and_artist():
    global last_checked
    global current_track
    global current_display
    raw_song_info = os.popen("mpc current")
    song_info = raw_song_info.read()
    song_info = song_info[10:]
    info_start = song_info.index(":")
    artist_and_title = song_info[info_start + 2:]
    artist_and_title = artist_and_title.rstrip()
    artist_and_title = artist_and_title[:40]
    #if strftime("%S", gmtime())[-1] == '0':
     #   last_checked = "On Zero"
    #if strftime("%S", gmtime())[-1] == '5':
     #   last_checked = "On Five"
    if current_track != artist_and_title:
        current_track = artist_and_title
    print current_track
        #line1 = current_track
        #line2 = "Station Here"
        #ser.write(str(current_track + "~"))  #Written Here
        #ser.write(str("Station Here`"))   #Written Here
        #print current_track + " written!"


def get_time():
    global current_display
    global current_time
    current_time = os.popen("date").read()
    current_time = current_time[:16]
    print current_time
    #ser.write(str(time + "`"))  #Written Here

def get_weather():
    global current_display
    global weather
    weather = os.popen('weather "jinotepe, nicaragua"').read()
    weather = weather.strip()
    weather = weather.replace("°","'F")
    print weather
    #ser.write(str(weather + "`"))  #Written Here
    

#---------------------------------------------------------------------------
while True:
    if strftime("%S", gmtime())[-1] == '0' and last_checked == "On Five":    
        interval_check()
        get_time()
        get_weather()
        try:
            get_song_and_artist()
            #print current_track
        except:
            pass

    if strftime("%S", gmtime())[-1] == '5' and last_checked == "On Zero":
        interval_check()
        get_time()
        get_weather()
        try:
            get_song_and_artist()
            #print current_track
        except:
            pass
    
    if current_display == "music":
        line1 = current_track
        line2 = "Station Here"
    if current_display == "time_music":
        line1 = current_time
        line2 = current_track
    if current_display == "time_weather":
        line1 = current_time
        short_weather = weather[5:9]
        line2 = short_weather

    if ser.inWaiting() > 0:
        x = ser.readline()
        x = x.strip()
        if x == "Next!":
            if os.popen("mpc current").read() == "":
                os.system("mpc play")                    
            else:
                os.system("mpc next")
        
        if x == "Display!" and current_display == "music":
            current_display = "time_music"
            x = ""
        if x == "Display!" and current_display == "time_music":
            current_display = "time_weather" 
            x = ""
        if x == "Display!" and current_display == "time_weather":
           current_display = "music"
           x = ""
    if arduino_line1 != line1:
        ser.write(str(line1+ "~"))
        arduino_line1 = line1
    if arduino_line2 != line2:
        ser.write(str(line2+ "`"))
        arduino_line2 = line2
