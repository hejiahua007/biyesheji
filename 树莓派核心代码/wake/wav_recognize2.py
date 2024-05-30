#文件名：wav_recognize2.py
#功能描述：识别。它支持使用百度语音识别和 Rhino 意图识别模型，通过语音命令控制智能家居设备。

from aip import AipSpeech
import time
import rhino0

# 替换为你的 Porcupine 访问密钥和模型路径
porcupine_key = "*/*=="
porcupine_model = '../file/model/hello-chat_en_raspberry-pi_v3_0_0.ppn'

def get_file_content(filePath):
    """读取音频文件内容
    
    Args:
        filePath (str): 待读取文件的路径
    
    Returns:
        bytes: 音频文件内容
    """
    with open(filePath, 'rb') as fp:
        return fp.read()

# 替换为你的百度云语音服务的 APPID、API_KEY 和 SECRET_KEY
APP_ID = '*'
API_KEY = '*'
SECRET_KEY = '*'
textPath = '../file/tmp/text.txt'

# 新建一个 AipSpeech 实例
client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)

def baidu_stt(filename):
    """使用百度语音识别进行语音转文字
    
    Args:
        filename (str): 音频文件路径
    
    Returns:
        str: 转换后的文字
    """
    wordStr = ''
    result = client.asr(get_file_content(filename), 'wav', 16000, {'dev_pid': 1536})

    if result['err_msg'] == 'success.':
        print("stt successful")
        word = result['result'][0].encode('utf-8')  # utf-8编码
        wordStr = word.decode('utf-8')  # 解码为字符串

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
    """监听音频输入并进行语音识别和意图识别
    
    Args:
        model (str): 使用的语音识别模型，默认为百度
    
    Returns:
        str: 识别的语音指令或意图
    """
    start_time = time.time()  # 记录开始时间

    # 使用 Rhino 进行意图识别
    result = rhino0.rhino_file()


    print(result)

    user_words = ""

    if isinstance(result, str):  # 如果result是字符串，说明是路径
        if model == "baidu":
            user_words = baidu_stt(result)
        else:
            user_words = "Unsupported speech recognition model"

        if user_words == "":
            print("你什么都没说")
        else:
            print("你说了: ", user_words)

    elif isinstance(result, dict):  # 如果result是字典，提取值
        intent = result.get('intent', '未知')
        slots = result.get('slots', {})

        print("意图: ", intent)

        for slot, value in slots.items():
            print(f"{slot}: {value}")

        # 检查并打印特定关键词
        if intent == '灯光':
            if '打开' in slots.values() or '开' in slots.values() or '开一点' in slots.values():
                user_words = '打开灯光'
            elif '关闭' in slots.values() or '关' in slots.values():
                user_words = '关闭灯光'

        elif intent == '窗帘':
            if '打开' in slots.values() or '开' in slots.values():
                user_words = '打开窗帘'
            elif '关闭' in slots.values() or '关' in slots.values():
                user_words = '关闭窗帘'
            elif '开一点' in slots.values():
                user_words = '开一点窗帘'

        elif intent == '制雾器':
            if '打开' in slots.values() or '开' in slots.values():
                user_words = '打开制雾器'
            elif '关闭' in slots.values() or '关' in slots.values():
                user_words = '关闭制雾器'

        elif intent == '风扇':
            if '打开' in slots.values() or '开' in slots.values() or '开二档' in slots.values():
                user_words = '打开风扇'
            elif '关闭' in slots.values() or '关' in slots.values():
                user_words = '关闭风扇'
            elif '开一档' in slots.values():
                user_words = '开一档风扇'

        elif intent == '音乐':
            if '打开' in slots.values() or '开' in slots.values():
                if '每日' in slots and '晨间' in slots:
                    user_words = '打开每日晨间音乐'
                elif '每日' in slots and '睡前' in slots:
                    user_words = '打开每日睡前音乐'
                else:
                    user_words = '打开音乐'
            elif '关闭' in slots.values() or '关' in slots.values():
                if '每日' in slots and '晨间' in slots:
                    user_words = '关闭每日晨间音乐'
                elif '每日' in slots and '睡前' in slots:
                    user_words = '关闭每日睡前音乐'
                else:
                    user_words = '关闭音乐'

        elif intent == '天气预报':
            if '打开' in slots.values() or '开' in slots.values():
                if '每日' in slots:
                    user_words = '打开每日天气预报'
                else:
                    user_words = '打开天气预报'
            elif '关闭' in slots.values() or '关' in slots.values():
                if '每日' in slots:
                    user_words = '关闭每日天气预报'
                else:
                    user_words = '关闭天气预报'

        elif intent == '一言':
            if '打开' in slots.values() or '开' in slots.values():
                if '每日' in slots:
                    user_words = '打开每日一言'
                else:
                    user_words = '打开一言'
            elif '关闭' in slots.values() or '关' in slots.values():
                if '每日' in slots:
                    user_words = '关闭每日一言'
                else:
                    user_words = '关闭一言'

        elif intent == '备忘录':
            if '打开' in slots.values() or '开' in slots.values():
                user_words = '打开备忘录'
            elif '关闭' in slots.values() or '关' in slots.values():
                user_words = '关闭备忘录'

        elif intent == '睡前状态检测':
            if '打开' in slots.values() or '开' in slots.values():
                if '每日' in slots:
                    user_words = '打开每日睡前状态检测'
                else:
                    user_words = '打开睡前状态检测'
            elif '关闭' in slots.values() or '关' in slots.values():
                if '每日' in slots:
                    user_words = '关闭每日睡前状态检测'
                else:
                    user_words = '关闭睡前状态检测'

    else:
        print("未知的结果类型")
        user_words = '命令指令未知类型'
    
    end_time = time.time()  # 记录结束时间
    elapsed_time = end_time - start_time
    print(f"listen函数执行时间: {elapsed_time}秒")

    return user_words




#print(listen())
#print(baidu_stt('/home/pi/Desktop/sleep/file/tmp/recorded_audio.wav'))
