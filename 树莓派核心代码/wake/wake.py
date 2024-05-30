#文件名：wake.py
#功能描述：功能描述：这个模块用于检测唤醒词。当检测到指定的唤醒词时，系统会被唤醒以执行后续的指令。该模块使用了 Porcupine 的关键词检测引擎。
import time
import pvporcupine
import pyaudio
import struct

# 替换为你的 Porcupine 访问密钥
porcupine_key = **

# Porcupine 模型路径
porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'

def keyword_wake_up():
    start_time = time.time()  # 记录函数开始时间
    print("wake start")
    
    # 创建 Porcupine 实例，用于检测唤醒词
    porcupine = pvporcupine.create(access_key=porcupine_key, keyword_paths=[porcupine_model])

    # 初始化 PyAudio
    kws_audio = pyaudio.PyAudio()

    # 打开音频流，配置为单声道、16位深度、输入模式
    audio_stream = kws_audio.open(
        rate=porcupine.sample_rate,
        channels=1,
        format=pyaudio.paInt16,
        input=True,
        frames_per_buffer=porcupine.frame_length,
        input_device_index=None,
    )

    print("等待唤醒中,唤醒词:hello chat...")

    recording = []
    limit_time = 0

    # 循环检测唤醒词
    while True:
        pcm = audio_stream.read(porcupine.frame_length)
        _pcm = struct.unpack_from("h" * porcupine.frame_length, pcm)
        keyword_index = porcupine.process(_pcm)
        recording.extend(_pcm)
        
        # 如果检测到唤醒词，跳出循环
        if keyword_index >= 0:
            print("唤醒了捏！")
            break
        else:
            limit_time += 1
            if limit_time == 200:  # 设置超时时间
                break

    # 停止并关闭音频流
    audio_stream.stop_stream()
    audio_stream.close()
    
    # 释放 Porcupine 实例
    porcupine.delete()
    
    # 终止 PyAudio
    kws_audio.terminate()

    end_time = time.time()  # 记录函数结束时间
    elapsed_time = end_time - start_time
    print(f"keyword_wake_up函数执行时间: {elapsed_time}秒")
    
    # 返回唤醒结果
    if limit_time != 200:
        return 1 
    else:
        return 0

# 测试唤醒功能
# print(keyword_wake_up())
