import struct
import wave
import time
import pvrhino
from pvrecorder import PvRecorder

# 直接在文件中定义参数
ACCESS_KEY = "e/r9CarH95u5FzdL4hcS88BXYchJFKHCBSqGskmDuP24IuEpmOe0fg=="
CONTEXT_PATH = '/home/pi/Desktop/sleep/file/model/zhiling_zh_raspberry-pi_v3_0_0.rhn'
LIBRARY_PATH = None  # 或 'path/to/your/library'
MODEL_PATH = '/home/pi/Desktop/sleep/file/model/rhino_params_zh.pv'
SENSITIVITY = 0.5
ENDPOINT_DURATION_SEC = 1.0
REQUIRE_ENDPOINT = True
AUDIO_DEVICE_INDEX = -1
OUTPUT_PATH = '/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav'  # 可选：保存录音文件的路径

SILENCE_THRESHOLD = 100  # 静默阈值
SILENCE_DURATION_SEC = 0.5  # 静默检测的持续时间（秒）

def save_audio_to_file(wav_file, pcm_data):
    wav_file.writeframes(struct.pack("h" * len(pcm_data), *pcm_data))

def rhino_mic():
    try:
        # 初始化 Rhino
        rhino = pvrhino.create(
            access_key=ACCESS_KEY,
            library_path=LIBRARY_PATH,
            model_path=MODEL_PATH,
            context_path=CONTEXT_PATH,
            sensitivity=SENSITIVITY,
            endpoint_duration_sec=ENDPOINT_DURATION_SEC,
            require_endpoint=REQUIRE_ENDPOINT)
    except pvrhino.RhinoError as e:
        print("初始化 Rhino 失败，错误: ", e)
        raise e

    print('Rhino 版本: %s' % rhino.version)
    print('上下文信息: %s' % rhino.context_info)

    # 创建录音器
    recorder = PvRecorder(
        frame_length=rhino.frame_length,
        device_index=AUDIO_DEVICE_INDEX)
    recorder.start()

    print('使用设备: %s' % recorder.selected_device)
    print('正在监听 ... .\n')

    wav_file = None
    if OUTPUT_PATH is not None:
        # 打开 WAV 文件用于写入
        wav_file = wave.open(OUTPUT_PATH, 'wb')
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(rhino.sample_rate)  # 使用 rhino.sample_rate

    last_speech_time = time.time()
    try:
        while True:
            pcm = recorder.read()

            if wav_file is not None:
                save_audio_to_file(wav_file, pcm)

            if max(pcm) > SILENCE_THRESHOLD:
                last_speech_time = time.time()

            if time.time() - last_speech_time > SILENCE_DURATION_SEC:
                print("检测到静默，停止录音。")
                break

            is_finalized = rhino.process(pcm)
            if is_finalized:
                inference = rhino.get_inference()
                if inference.is_understood:
                    recorder.stop()
                    if wav_file is not None:
                        wav_file.close()
                    rhino.delete()
                    return {
                        "intent": inference.intent,
                        "slots": inference.slots
                    }
                else:
                    print("未能理解该命令。\n")
                    recorder.stop()
                    if wav_file is not None:
                        wav_file.close()
                    rhino.delete()
                    return OUTPUT_PATH
                    
    except KeyboardInterrupt:
        print('正在停止 ...')
    recorder.stop()
    recorder.delete()
    rhino.delete()
    wav_file.close()

    if OUTPUT_PATH is not None:
        return OUTPUT_PATH

# 调用函数
#result = rhino_mic()
#print(result)
