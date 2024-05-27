import pygame
import os
from clock import Alarm_add,Alarm_del

def play_audio(audio_file_name):
    # Initialize pygame
    pygame.init()
    # Set the audio file
    pygame.mixer.music.load(audio_file_name)
    # Play the audio
    pygame.mixer.music.play()
    # Wait for the audio to finish
    while pygame.mixer.music.get_busy():
        pygame.time.Clock().tick(10)
    # Quit pygame
    pygame.quit()

def m_play_audio():
    # Directory for MP3 files
    directory = "/home/pi/Desktop/sleep/file/music/banzou"
    # Iterate through all files in the directory
    for filename in os.listdir(directory):
        if filename.endswith(".mp3"):
            file_path = os.path.join(directory, filename)
            play_audio(file_path)

def n_play_audio():
    # Directory for MP3 files
    directory = "/home/pi/Desktop/sleep/file/music/piano"
    # Iterate through all files in the directory
    for filename in os.listdir(directory):
        if filename.endswith(".mp3"):
            file_path = os.path.join(directory, filename)
            play_audio(file_path)

def open_m_music():
    Alarm_add("18:05:00", 2)
    #play_audio("../file/music/zhiling/dakaichenjianyinyue.wav")
 
def stop_m_music():
    Alarm_del("21:30:00", 2)
    #play_audio("../file/music/zhiling/guanbichenjianyinyue.wav")
    
def open_n_music():
    Alarm_add("21:35:00", 3)
    #play_audio("../file/music/zhiling/dakaishuiqianyinyue.wav")
 
def stop_n_music():
    Alarm_del("21:35:00", 3)
    #play_audio("../file/music/zhiling/dakaishuiqianyinyue.wav")

def open_music():
    m_play_audio()
    #play_audio("../file/music/zhiling/dakaiyinyue.wav")
    
#open_music()


