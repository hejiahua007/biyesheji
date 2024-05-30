import pygame
import wave
import pyaudio
from pydub import AudioSegment

# 文件：musicplay.py
# 功能描述：用于播放、检查和转换音频文件格式。提供了基于pygame和pyaudio的音频播放功能，以及使用pydub进行音频格式转换的功能。

def play_audio(audio_file_name):
    """
    播放指定的音频文件，使用pygame库实现。
    """
    # 初始化pygame
    pygame.init()
    # 设置音频文件
    pygame.mixer.music.load(audio_file_name)
    # 播放音频
    pygame.mixer.music.play()
    # 等待音频播放完毕
    while pygame.mixer.music.get_busy():
        pygame.time.Clock().tick(10)
    # 退出pygame
    pygame.quit()

def play_audio2():
    """
    播放指定的音频文件，使用pyaudio库实现。
    """
    chunk = 1024
    wf = wave.open(r"/home/pi/Desktop/sleep/file/music/music1.wav", 'rb')
    p = pyaudio.PyAudio()
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()), 
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(), 
                    output=True)

    data = wf.readframes(chunk)  # 读取数据
    print(data)
    while data != b'':  # 播放
        stream.write(data)
        data = wf.readframes(chunk)
        print('while循环中！')
        print(data)
    stream.stop_stream()  # 停止数据流
    stream.close()
    p.terminate()  # 关闭 PyAudio

def check_audio(file_path):
    """
    检查音频文件的采样宽度、通道数和帧速率。
    """
    with wave.open(file_path, 'rb') as wf:
        print("采样宽度:", wf.getsampwidth())
        print("通道数:", wf.getnchannels())
        print("帧速率:", wf.getframerate())


def convert_audio_format(input_file, output_file, sample_width=2, channels=2):
    """
    将音频文件转换为指定的格式。
    """
    audio = AudioSegment.from_file(input_file)
    audio = audio.set_sample_width(sample_width)
    audio = audio.set_channels(channels)
    audio.export(output_file, format="wav")

