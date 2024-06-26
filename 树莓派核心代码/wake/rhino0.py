import pyaudio
import wave
import struct
import time
import pvcobra
import pvrhino

# 文件：wake_word_record_and_inference.py
# 功能描述：实现关键词唤醒、录音和语音指令识别功能。通过Porcupine检测唤醒词并启动录音，然后使用Rhino进行语音指令的识别。

# Porcupine的API密钥和模型文件路径
porcupine_key = "YOUR_PORCUPINE_KEY"
porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'

# Rhino的API密钥和模型文件路径
ACCESS_KEY = "YOUR_RHINO_KEY"
WAV_PATH = '/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav'
CONTEXT_PATH = '/home/pi/Desktop/sleep/file/model/zhiling_zh_raspberry-pi_v3_0_0.rhn'
LIBRARY_PATH = None
MODEL_PATH = '/home/pi/Desktop/sleep/file/model/rhino_params_zh.pv'
SENSITIVITY = 0.5
ENDPOINT_DURATION_SEC = 1.0
REQUIRE_ENDPOINT = True

def record_wav(output_filename='/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav', sample_rate=16000, channels=1, frames_per_buffer=512):
    """
    录制音频并保存为wav文件，通过静默检测来停止录音。

    Args:
        output_filename (str): 输出文件名。
        sample_rate (int): 采样率。
        channels (int): 通道数。
        frames_per_buffer (int): 缓冲区帧数。

    Returns:
        None
    """
    audio = pyaudio.PyAudio()
    cobra = pvcobra.create(access_key=porcupine_key)
    # 打开音频流
    stream = audio.open(format=pyaudio.paInt16,
                        channels=channels,
                        rate=sample_rate,
                        input=True,
                        frames_per_buffer=frames_per_buffer)
    is_voiced = 1
    print("Recording...")

    frames = []
    silence_count = 0

    start_time = time.time()
    while is_voiced:
        pcm = stream.read(frames_per_buffer)
        _pcm = struct.unpack_from("h" * frames_per_buffer, pcm)
        is_voiced = cobra.process(_pcm)
        print(is_voiced, silence_count)
        silence_count = 0 if is_voiced > 0.1 else silence_count + 1
        if silence_count <= 100:
            if silence_count < 70:
                frames.append(pcm)
        else:
            break
    end_time = time.time()
    print("Finished recording.")
    
    # 停止音频流
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # 保存录音
    with wave.open(output_filename, 'wb') as wf:
        wf.setnchannels(channels)
        wf.setsampwidth(audio.get_sample_size(pyaudio.paInt16))
        wf.setframerate(sample_rate)
        wf.writeframes(b''.join(frames))

    elapsed_time = end_time - start_time
    print(f"record_wav函数执行时间: {elapsed_time}秒")

def read_file(file_name, sample_rate):
    """
    读取wav文件并返回音频帧。

    Args:
        file_name (str): wav文件路径。
        sample_rate (int): 采样率。

    Returns:
        list: 音频帧。
    """
    wav_file = wave.open(file_name, mode="rb")
    channels = wav_file.getnchannels()
    sample_width = wav_file.getsampwidth()
    num_frames = wav_file.getnframes()

    if wav_file.getframerate() != sample_rate:
        raise ValueError("Audio file should have a sample rate of %d. got %d" % (sample_rate, wav_file.getframerate()))
    if sample_width != 2:
        raise ValueError("Audio file should be 16-bit. got %d" % sample_width)
    if channels == 2:
        print("Picovoice processes single-channel audio but stereo file is provided. Processing left channel only.")

    samples = wav_file.readframes(num_frames)
    wav_file.close()

    frames = struct.unpack('h' * num_frames * channels, samples)
    return frames[::channels]

def rhino_file():
    """
    使用Rhino进行语音指令识别。

    Returns:
        dict: 识别的意图和槽位。
        str: 音频文件路径。
    """
    record_wav()
    try:
        rhino = pvrhino.create(
            access_key=ACCESS_KEY,
            library_path=LIBRARY_PATH,
            model_path=MODEL_PATH,
            context_path=CONTEXT_PATH,
            sensitivity=SENSITIVITY,
            endpoint_duration_sec=ENDPOINT_DURATION_SEC,
            require_endpoint=REQUIRE_ENDPOINT)
    except pvrhino.RhinoError as e:
        print("Failed to initialize Rhino with error: ", e)
        raise e

    audio = read_file(WAV_PATH, rhino.sample_rate)

    num_frames = len(audio) // rhino.frame_length
    for i in range(num_frames):
        frame = audio[i * rhino.frame_length:(i + 1) * rhino.frame_length]
        is_finalized = rhino.process(frame)
        if is_finalized:
            inference = rhino.get_inference()
            if inference.is_understood:
                rhino.delete()
                return {
                    "intent": inference.intent,
                    "slots": inference.slots
                }
            else:
                print("Didn't understand the command.")
                rhino.delete()
            break

    return '/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav'
