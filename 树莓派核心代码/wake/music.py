# 这是一个使用 Pygame 库播放音频文件的 Python 脚本。
# 它可以播放指定目录下的所有 MP3 文件,并提供了开启和停止音乐的功能。
# 音乐播放可以通过设置闹钟来实现,并提供相应的开启和停止函数。
import pygame
import os
from clock import Alarm_add, Alarm_del

# 播放指定音频文件的函数
def play_audio(audio_file_name):
    # 初始化 Pygame
    pygame.init()
    # 加载音频文件
    pygame.mixer.music.load(audio_file_name)
    # 播放音频
    pygame.mixer.music.play()
    # 等待音频播放完成
    while pygame.mixer.music.get_busy():
        pygame.time.Clock().tick(10)
    # 退出 Pygame
    pygame.quit()

# 播放 `/home/pi/Desktop/sleep/file/music/banzou` 目录下的所有 MP3 文件
def m_play_audio():
    directory = "/home/pi/Desktop/sleep/file/music/banzou"
    for filename in os.listdir(directory):
        if filename.endswith(".mp3"):
            file_path = os.path.join(directory, filename)
            play_audio(file_path)

# 播放 `/home/pi/Desktop/sleep/file/music/piano` 目录下的所有 MP3 文件 
def n_play_audio():
    directory = "/home/pi/Desktop/sleep/file/music/piano"
    for filename in os.listdir(directory):
        if filename.endswith(".mp3"):
            file_path = os.path.join(directory, filename)
            play_audio(file_path)

# 开启 `banzou` 目录下的音乐,设置闹钟时间为 18:05:00
def open_m_music():
    Alarm_add("18:05:00", 2)

# 停止 `banzou` 目录下的音乐,设置闹钟时间为 21:30:00
def stop_m_music():
    Alarm_del("21:30:00", 2)

# 开启 `piano` 目录下的音乐,设置闹钟时间为 21:35:00
def open_n_music():
    Alarm_add("21:35:00", 3)

# 停止 `piano` 目录下的音乐,设置闹钟时间为 21:35:00
def stop_n_music():
    Alarm_del("21:35:00", 3)

# 调用 `m_play_audio()` 播放 `banzou` 目录下的音乐
def open_music():
    m_play_audio()