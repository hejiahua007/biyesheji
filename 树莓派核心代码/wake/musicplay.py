import pygame
import wave
import pyaudio

def play_audio(audio_file_name):
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
    chunk = 1024  
    wf = wave.open(r"/home/pi/Desktop/sleep/file/music/music1.wav", 'rb')
    p = pyaudio.PyAudio()
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()), channels=wf.getnchannels(),
                    rate=wf.getframerate(), output=True)

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
	with wave.open(file_path, 'rb') as wf:
		print("采样宽度:", wf.getsampwidth())
		print("通道数:", wf.getnchannels())
		print("帧速率:", wf.getframerate())

#check_audio("/home/pi/Desktop/sleep/file/music/music1.wav")
#check_audio("/home/pi/Desktop/sleep/file/music/music3.wav")

from pydub import AudioSegment

def convert_audio_format(input_file, output_file, sample_width = 2, channels = 2):
    audio = AudioSegment.from_file(input_file)
    audio = audio.set_sample_width(sample_width)
    audio = audio.set_channels(channels)
    audio.export(output_file, format="wav")
# 替换文件路径和参数
#input_file_path = "/home/pi/Desktop/sleep/file/music/music3.wav"
#output_file_path = "/home/pi/Desktop/sleep/file/music/music3_converted.wav"
#sample_width = 2  # 与 music1.wav 相同的采样宽度
#channels = 2      # 与 music1.wav 相同的通道数
#convert_audio_format(input_file_path, output_file_path, sample_width, channels)
#check_audio(output_file_path)

#play_audio('../file/zhiling/guanchuang.wav')












