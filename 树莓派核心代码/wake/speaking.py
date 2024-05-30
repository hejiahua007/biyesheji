# 文件名：speaking.py
# 功能描述：实现语音合成和播放功能，包括支持不同语言的文本转语音，并播放生成的音频文件。


import argparse
import torch
import time
from torch import no_grad, LongTensor
import asyncio
import utils
from models import SynthesizerTrn
from text import text_to_sequence
import commons
from scipy.io import wavfile
import pygame
from pydub import AudioSegment
# coding=utf-8
import sys
import json
import pygame
IS_PY3 = sys.version_info.major == 3
if IS_PY3:
    from urllib.request import urlopen
    from urllib.request import Request
    from urllib.error import URLError
    from urllib.parse import urlencode
    from urllib.parse import quote_plus
API_KEY = '**'
SECRET_KEY = '**'
PER = 4#yin se 
SPD = 5# 语速，取值0-15，默认为5中语速
PIT = 5# 音调，取值0-15，默认为5中语调
VOL = 5# 音量，取值0-9，默认为5中音量
# 下载的文件格式, 3：mp3(default) 4： pcm-16k 5： pcm-8k 6. wav
AUE = 6
FORMATS = {3: "mp3", 4: "pcm", 5: "pcm", 6: "wav"}
FORMAT = FORMATS[AUE]
CUID = "**"
TTS_URL = '**'
#定义一个自定义异常类 DemoError，用于处理可能的异常情况。

commands_to_audio = {
    '好的，主人。正在开门': "../file/zhiling/kaimen.wav",
    '好的，主人。正在关门': "../file/zhiling/guanmen.wav",
    '好的，主人。正在开窗': "../file/zhiling/kaichuang.wav",
    '好的，主人。正在关窗': "../file/zhiling/guanchuang.wav",
    '好的，主人。正在开灯': "../file/zhiling/kaideng.wav",  # Corrected to kaichuang.wav
    '好的，主人。正在关灯': "../file/zhiling/guandeng.wav",  # Corrected to guanchuang.wav
    '好的，主人。正在开窗帘': "../file/zhiling/kaichuanglian.wav",
    '好的，主人。正在关窗帘': "../file/zhiling/guanchuanglian.wav",
    '好的，主人。正在开香薰': "../file/zhiling/kaixiangxun.wav",
    '好的，主人。正在关香薰': "../file/zhiling/guanxiangxun.wav",
    '好的，主人。正在开制雾器': "../file/zhiling/kaizhiwuqi.wav",
    '好的，主人。正在关制雾器': "../file/zhiling/guanzhiwuqi.wav",
    '好的，主人。正在开闹钟': "../file/zhiling/kainaozhong.wav",
    '好的，主人。正在关闹钟': "../file/zhiling/guannaozhong.wav",
    '好的，主人。正在开音乐': "../file/zhiling/kaiyinyue.wav",
    '好的，主人。正在关音乐': "../file/zhiling/guanyinyue.wav",
    '好的，主人。正在开备忘录': "../file/zhiling/kaibeiwanglu.wav",
    '好的，主人。正在关备忘录': "../file/zhiling/guanbeiwanglu.wav",
    '好的，主人。正在开风扇': "../file/zhiling/kaifengshan.wav",
    '好的，主人。正在关风扇': "../file/zhiling/guanfengshan.wav",
    '好的，主人。正在开天气预报': "../file/zhiling/kaitianqiyubao.wav",
    '好的，主人。正在关天气预报': "../file/zhiling/guantianqiyubao.wav",
    '好的，主人。正在开睡前状态检测': "../file/zhiling/kaishuiqianzhuangtaijiancha.wav",
    '好的，主人。正在关睡前状态检测': "../file/zhiling/guanshuiqianzhuangtaijiancha.wav",
    '好的，主人。正在汇报睡眠环境': "../file/zhiling/huibaoShuiMianHuanJing.wav",
}
commands_to_audio2 = {

    'かしこまりました、ご主人様。ライトをつけています': "../file/zhiling_jan/kaideng.wav",

    'かしこまりました、ご主人様。ライトを消しています': "../file/zhiling_jan/guandeng.wav",

    'かしこまりました、ご主人様。カーテンを開けています': "../file/zhiling_jan/kaichuanglian.wav",

    'かしこまりました、ご主人様。カーテンを閉めています': "../file/zhiling_jan/guanchuanglian.wav",

    'かしこまりました、ご主人様。カーテンを少し開けています': "../file/zhiling_jan/kaiyidianchuanglian.wav",

    'かしこまりました、ご主人様。加湿器をつけています': "../file/zhiling_jan/kaizhiwuqi.wav",

    'かしこまりました、ご主人様。加湿器を消しています': "../file/zhiling_jan/guanzhiwuqi.wav",

    'かしこまりました、ご主人様。ファンをつけています': "../file/zhiling_jan/kaifengshan.wav",

    'かしこまりました、ご主人様。ファンを消しています': "../file/zhiling_jan/guanfengshan.wav",

    'かしこまりました、ご主人様。ファンを1段階にしています': "../file/zhiling_jan/kaiyidangfengshan.wav",

    'かしこまりました、ご主人様。毎朝の音楽をつけています': "../file/music/zhiling_jan/dakaimeirichenjianyinyue.wav",

    'かしこまりました、ご主人様。毎朝の音楽を消しています': "../file/music/zhiling_jan/guanbimeirichenjianyinyue.wav",

    'かしこまりました、ご主人様。毎晩の音楽をつけています': "../file/music/zhiling_jan/dakaimeirishuiqianyinyue.wav",

    'かしこまりました、ご主人様。毎晩の音楽を消しています': "../file/music/zhiling_jan/guanbimeirishuiqianyinyue.wav",

    'かしこまりました、ご主人様。音楽をつけています': "../file/music/zhiling_jan/dakaiyinyue.wav",

    'かしこまりました、ご主人様。メモを開いています': "../file/beiwanglu/zhiling_jan/kaibeiwanglu.wav",

    'かしこまりました、ご主人様。毎日の天気予報を起動しています': "../file/Weather_forecast/zhiling_jan/kaiqi.wav",

    'かしこまりました、ご主人様。毎日の天気予報を消しています': "../file/Weather_forecast/zhiling_jan/guantianqiyubao.wav",

    'かしこまりました、ご主人様。天気予報を開いています': "../file/Weather_forecast/zhiling_jan/kaitianqiyubao.wav",

    'かしこまりました、ご主人様。毎日の一言をつけています': "../file/aphorisms/zhiling_jan/dakaimeiriyiyan.wav",

    'かしこまりました、ご主人様。毎日の一言を消しています': "../file/aphorisms/zhiling_jan/guanbimeiriyiyan.wav",

    'かしこまりました、ご主人様。一言をつけています': "../file/aphorisms/zhiling_jan/dakaiyiyan.wav",

    'かしこまりました、ご主人様。毎晩の状態チェックを起動しています': "../file/detection/zhiling_jan/kaishuiqianzhuangtaijiancha.wav",

    'かしこまりました、ご主人様。毎晩の状態チェックを消しています': "../file/detection/zhiling_jan/guanshuiqianzhuangtaijiancha.wav",

    'かしこまりました、ご主人様。状態チェックを開いています': "../file/detection/zhiling_jan/zhengzaidakaishuiqianzhuangtaijiance.wav"

}




class DemoError(Exception):
    pass
"""  TOKEN start """
TOKEN_URL = '*'
SCOPE = 'audio_tts_post'  # 有此scope表示有tts能力，没有请在网页里勾选
#noise_scale(控制感情变化程度)
vitsNoiseScale = 0.6
#noise_scale_w(控制音素发音长度)
vitsNoiseScaleW = 0.668
#length_scale(控制整体语速)
vitsLengthScale = 1.0
_init_vits_model = False

hps_ms = None
device = None
net_g_ms = None
def fetch_token():
    print("fetch token begin")
    params = {'grant_type': 'client_credentials',
              'client_id': API_KEY,
              'client_secret': SECRET_KEY}
    post_data = urlencode(params)
    if (IS_PY3):
        post_data = post_data.encode('utf-8')
    req = Request(TOKEN_URL, post_data)
    try:
        f = urlopen(req, timeout=5)
        result_str = f.read()
    except URLError as err:
        print('token http response http code : ' + str(err.code))
        result_str = err.read()
    if (IS_PY3):
        result_str = result_str.decode()

    print(result_str)
    result = json.loads(result_str)
    print(result)
    if ('access_token' in result.keys() and 'scope' in result.keys()):
        if not SCOPE in result['scope'].split(' '):
            raise DemoError('scope is not correct')
        print('SUCCESS WITH TOKEN: %s ; EXPIRES IN SECONDS: %s' % (result['access_token'], result['expires_in']))
        return result['access_token']
    else:
        raise DemoError('MAYBE API_KEY or SECRET_KEY not correct: access_token or scope not found in token response')
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
# def play_audio(audio_file_name):
#     command = f'mpv.exe -vo null {audio_file_name}'
#     subprocess.run(command, shell=True)

def init_vits_model():
    # 全局变量引入
    global hps_ms, device, net_g_ms

    # 创建参数解析器
    parser = argparse.ArgumentParser()
    parser.add_argument('--device', type=str, default='cpu')
    parser.add_argument('--api', action="store_true", default=False)
    parser.add_argument("--share", action="store_true", default=False, help="share gradio app")
    parser.add_argument("--colab", action="store_true", default=False, help="share gradio app")
    args = parser.parse_args()
    device = torch.device(args.device)
    # 从文件中获取VITS模型的参数
    hps_ms = utils.get_hparams_from_file('config.json')
    # 创建VITS模型
    net_g_ms = SynthesizerTrn(
        len(hps_ms.symbols),
        hps_ms.data.filter_length // 2 + 1,
        hps_ms.train.segment_size // hps_ms.data.hop_length,
        n_speakers=hps_ms.data.n_speakers,
        **hps_ms.model)
    # 将VITS模型设置为评估模式，并移动到指定设备
    _ = net_g_ms.eval().to(device)
    # 获取说话者信息
    speakers = hps_ms.speakers
    # 加载模型和优化器
    model, optimizer, learning_rate, epochs = utils.load_checkpoint('../file/model/G_953000.pth', net_g_ms, None)
    # 标记VITS模型已初始化
    _init_vits_model = True

def get_text(text, hps):
    # 使用text_to_sequence将输入文本转换为模型可用的格式
    text_norm, clean_text = text_to_sequence(text, hps.symbols, hps.data.text_cleaners)
    # 如果需要在文本中添加空白符，则在文本序列中插入0（代表空白符）
    if hps.data.add_blank:
        text_norm = commons.intersperse(text_norm, 0)
    # 将文本序列转换为LongTensor（长整数）格式
    text_norm = LongTensor(text_norm)
    return text_norm, clean_text
def convert_audio_format(input_file, output_file, sample_width, channels):
    audio = AudioSegment.from_file(input_file)
    audio = audio.set_sample_width(sample_width)
    audio = audio.set_channels(channels)
    audio.export(output_file, format="wav")

def vits(text, language, speaker_id, noise_scale, noise_scale_w, length_scale):
    global over
    # 记录生成开始时间
    start = time.perf_counter()
    # 处理输入文本
    if not len(text):
        return "输入文本不能为空！", None, None
    text = text.replace('\n', ' ').replace('\r', '').replace(" ", "")
    if len(text) > 600:
        return f"输入文字过长！{len(text)}>300", None, None
    # 根据语言类型添加标记
    if language == 0:
        text = f"[ZH]{text}[ZH]"
    elif language == 1:
        text = f"[JA]{text}[JA]"
    else:
        text = f"{text}"
    # 调用get_text将文本转换为模型可用的格式
    stn_tst, clean_text = get_text(text, hps_ms)
    # 使用VITS模型生成音频
    with no_grad():
        x_tst = stn_tst.unsqueeze(0).to(device)
        x_tst_lengths = LongTensor([stn_tst.size(0)]).to(device)
        speaker_id = LongTensor([speaker_id]).to(device)
        audio = net_g_ms.infer(x_tst, x_tst_lengths, sid=speaker_id, noise_scale=noise_scale, noise_scale_w=noise_scale_w,
                               length_scale=length_scale)[0][0, 0].data.cpu().float().numpy()
    # 返回生成成功信息、音频信息以及生成耗时
    return "生成成功!", (22050, audio), f"生成耗时 {round(time.perf_counter()-start, 2)} s"

async def start(input_str=None,language = 0):
    if input_str == None:
        print("请输入 >")
        input_str = await asyncio.get_event_loop().run_in_executor(None, input, '')
    if "关闭AI" in input_str:
        return
    result = input_str
    status, audios, time = vits(result,language, 124, vitsNoiseScale, vitsNoiseScaleW, vitsLengthScale)
    print("VITS : ", status, time)
    if language ==1:
        wavfile.write(commands_to_audio2[input_str], audios[0], audios[1])
        convert_audio_format(commands_to_audio2[input_str],commands_to_audio2[input_str],2,2)
        play_audio(commands_to_audio2[input_str])
    elif language == 0:
        wavfile.write("../file/zhiling/kaiyidangfengshan.wav", audios[0], audios[1])
        convert_audio_format("../file/zhiling/kaiyidangfengshan.wav","../file/zhiling/kaiyidangfengshan.wav",2,2)
        #play_audio("../file/detection/zhiling/jiancedaomubiaorenwukanshu.wav")

def speak(input_str = None,language = 0):
    if not _init_vits_model:
        init_vits_model()
    if input_str != None:
        asyncio.run(start(input_str,language))
def speak2(TEXT):
    token = fetch_token()
    tex = quote_plus(TEXT)  # 此处TEXT需要两次urlencode
    print(tex)
    params = {'tok': token, 'tex': tex, 'per': PER, 'spd': SPD, 'pit': PIT, 'vol': VOL, 'aue': AUE, 'cuid': CUID,
              'lan': 'zh', 'ctp': 1}  # lan ctp 固定参数

    data = urlencode(params)
    print('test on Web Browser' + TTS_URL + '?' + data)

    req = Request(TTS_URL, data.encode('utf-8'))
    has_error = False
    try:
        f = urlopen(req)
        result_str = f.read()

        headers = dict((name.lower(), value) for name, value in f.headers.items())

        has_error = ('content-type' not in headers.keys() or headers['content-type'].find('audio/') < 0)
    except  URLError as err:
        print('asr http response http code : ' + str(err.code))
        result_str = err.read()
        has_error = True
    save_file = "../file/tmp/output2.txt" if has_error else '../file/tmp/output2.' + FORMAT
#    save_file = "error.txt" if has_error else 'result.' + FORMAT
    with open(save_file, 'wb') as of:
        of.write(result_str)
    if has_error:
        if (IS_PY3):
            result_str = str(result_str, 'utf-8')
        print("tts api  error:" + result_str)

    print("result saved as :" + save_file)

    play_audio("../file/tmp/output2.wav")



