import struct
import pvporcupine
import pvcobra
import pyaudio
import wave
import time

# 文件：wake_word_detection.py
# 功能描述：实现关键词唤醒和录音功能。通过Porcupine检测唤醒词，并使用Cobra检测语音活动。
#           醒来后开始录音，并保存音频文件为WAV格式。

# Porcupine的API密钥
porcupine_key = "YOUR_PORCUPINE_KEY"

# Porcupine的模型文件路径
porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'

def keyword_wake_up():
    """
    检测唤醒词，并在唤醒后开始录音。
    """
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
    print("等待唤醒中,唤醒词:hello chat...")

    while True:
        pcm = audio_stream.read(porcupine.frame_length)
        _pcm = struct.unpack_from("h" * porcupine.frame_length, pcm)
        keyword_index = porcupine.process(_pcm)
        if keyword_index >= 0:
            print("唤醒了捏！")
            is_voiced = 1
            break
    
    # 开始录音
    frames = []
    print("开始录音...")
    silence_count = 0
    recording = []
    time.sleep(0.1)
    while is_voiced:
        pcm = audio_stream.read(porcupine.frame_length)
        _pcm = struct.unpack_from("h" * porcupine.frame_length, pcm)
        is_voiced = cobra.process(_pcm)
        print(is_voiced, silence_count)
        silence_count = 0 if is_voiced > 0.5 else silence_count + 1
        if silence_count <= 100:
            if silence_count < 70:
                recording.extend(_pcm)
        else:
            break
    
    save_audio(recording)
    audio_stream.stop_stream()
    audio_stream.close()
    porcupine.delete()
    kws_audio.terminate()
    print(f"录音已保存: recorded_audio.wav")

def save_audio(data, filename="../file/tmp/recorded_audio.wav"):
    """
    将录音数据保存为WAV文件。
    """
    wf = wave.open(filename, 'wb')
    wf.setnchannels(1)
    wf.setsampwidth(2)
    wf.setframerate(16000)  # 设置采样率
    wf.writeframes(b''.join(struct.pack('h', sample) for sample in data))
    wf.close()

# 记录开始时间
start_time = time.time()

# 调用关键词唤醒函数
keyword_wake_up()

# 记录结束时间并计算执行时间
end_time = time.time()
elapsed_time = end_time - start_time
print(f"listen函数执行时间: {elapsed_time}秒")
