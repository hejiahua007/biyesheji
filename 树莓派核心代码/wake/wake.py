import time
import pvporcupine
import pyaudio
import struct
import wave
#porcupine_key = **
porcupine_key = **

porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'

def keyword_wake_up():
    start_time = time.time()  # 记录开始时间
    print("wake start")
    porcupine = pvporcupine.create(access_key=porcupine_key, keyword_paths=[porcupine_model])

    kws_audio = pyaudio.PyAudio()

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
    while True:
        pcm = audio_stream.read(porcupine.frame_length)
        _pcm = struct.unpack_from("h" * porcupine.frame_length, pcm)
        keyword_index = porcupine.process(_pcm)
        recording.extend(_pcm)
        if keyword_index >= 0:
            print("唤醒了捏！")
            break
        else:
            limit_time =limit_time+1
            #print(limit_time)
            if limit_time == 200:
                break
            
    audio_stream.stop_stream()
    audio_stream.close()
    porcupine.delete()
    kws_audio.terminate()
    end_time = time.time()  # 记录结束时间
    elapsed_time = end_time - start_time
    print(f"keyword_wake_up函数执行时间: {elapsed_time}秒")
    if limit_time != 200:
        return 1 
    else:
        return 0;
#print(keyword_wake_up())
