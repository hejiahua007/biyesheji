# 腾讯云asr,输入base64编码的wav音频，输出text，此函数需异步调用，以节约请求事件
#from tencentcloud.common import credential
#from tencentcloud.common.profile.client_profile import ClientProfile
#from tencentcloud.common.profile.http_profile import HttpProfile
#from tencentcloud.asr.v20190614 import asr_client, models
from aip import AipSpeech
import struct
import pvporcupine
import pyaudio
#import asyncio
import json
import base64
import io
import wave
import pvcobra
import time

porcupine_key = "cinmq/v7vHEzd3vrbTD9I24KiGgxbmUBXjxCcgG8kGnx8l48h57L6g=="
porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'
# 读取文件
def get_file_content(filePath):  # filePath  待读取文件名
    with open(filePath, 'rb') as fp:
        return fp.read()

def sound_record():
    
    porcupine = pvporcupine.create(access_key=porcupine_key, keyword_paths=[porcupine_model])
    cobra = pvcobra.create(access_key=porcupine_key)
    # 开启录音流
    kws_audio = pyaudio.PyAudio()
    audio_stream = kws_audio.open(
        rate=porcupine.sample_rate,
        channels=1,
        format=pyaudio.paInt16,
        input=True,
        frames_per_buffer=1024,
        input_device_index=None,
    )
    # 开始录音
    is_voiced = 1
    frames = []
    print("开始录音...")
    silence_count = 0
    recording = []
    time.sleep( 0.1 )
    while is_voiced:
        pcm = audio_stream.read(porcupine.frame_length)
        _pcm = struct.unpack_from("h" * porcupine.frame_length, pcm)
        is_voiced = cobra.process(_pcm)
        print(is_voiced,silence_count)
        silence_count = 0 if is_voiced > 0.5 else silence_count  + 1
        if silence_count <= 100:
            if silence_count < 70:
                recording.extend(_pcm)
        else:
            break
    filename = save_audio(recording)
    audio_stream.stop_stream()
    audio_stream.close()
    porcupine.delete()
    kws_audio.terminate()
    # 保存录音结果为WAV文件
    print(f"录音已保存:recorded_audio.wav")
    return filename
    
def save_audio(data, filename="../file/tmp/recorded_audio.wav"):
    wf = wave.open(filename, 'wb')
    wf.setnchannels(1)
    wf.setsampwidth(2)
    wf.setframerate(16000)  # Adjust the sample rate as needed
    wf.writeframes(b''.join(struct.pack('h', sample) for sample in data))
    wf.close()
    return filename

''' 你的APPID AK SK  参数在申请的百度云语音服务的控制台查看'''
APP_ID = '41921615'
API_KEY = 'fOgFWfWduay5wB63zkZ4khIF'
SECRET_KEY = 'K0ze3w6Pt6aKO6avbPxea31iIxgZ21qy'
textPath = '../file/tmp/text.txt'
# 新建一个AipSpeech
client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)

def baidu_stt(filename):
    wordStr = ''
    result = client.asr(get_file_content(filename), 'wav', 16000, {'dev_pid': 1536})

    if result['err_msg'] == 'success.':
        print("stt successful")
        word = result['result'][0].encode('utf-8')  # utf-8编码
        wordStr = word.decode('utf-8')  # Decode the bytes to a string

        if wordStr != '':
            if wordStr[-3:] == '，':
                print(wordStr[:-3])
                with open(textPath, 'w', encoding='utf-8') as f:
                    f.write(wordStr[:-3])
            else:
                print(wordStr)
                with open(textPath, 'w', encoding='utf-8') as f:
                    f.write(wordStr)
        else:
            print("音频文件不存在或格式错误")
    else:
        print("错误")

    return wordStr


def listen(model: str = "baidu"):
    start_time = time.time()  # 记录开始时间
    filepath = sound_record()

    if model == "baidu":
        user_words = baidu_stt(filepath)
    else:
        user_words = "Unsupported speech recognition model"

    if user_words == "":
        print("你什么都没说")
    else:
        print("你说了: ", user_words)
    end_time = time.time()  # 记录结束时间
    elapsed_time = end_time - start_time
    print(f"listen函数执行时间: {elapsed_time}秒")
       

    return user_words


#listen()
#print(baidu_stt('/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav'))
