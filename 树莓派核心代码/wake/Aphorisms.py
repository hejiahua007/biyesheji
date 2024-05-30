# 文件名：Aphorisms.py
# 功能描述：这个模块用于从网络获取一句格言（一言）并提供相关的功能，
# 包括播放开启和关闭格言的提示音，定时获取格言，以及通过语音输出格言。

import requests
import speaking
from clock import Alarm_add, Alarm_del

# 定义请求一言的API URL
url = ""

def yiyan():
    """从指定URL获取一言并打印出来"""
    response = requests.get(url)
    data = response.json()

    # 检查响应状态码并处理数据
    if response.status_code == 200:
        hitokoto = data["hitokoto"]
        from_who = data.get("from_who")  # 使用.get避免KeyError
        print("Hitokoto:", hitokoto)
    else:
        print("请求失败:", data.get("message", "未知错误"))
    return hitokoto

def open_yiyan():
    """设置定时任务，每天晚上21:50提醒格言"""
    # speaking.play_audio("../file/aphorism/kaiqi.wav")
    Alarm_add("21:50:00", 4)

def close_yiyan():
    """取消定时任务"""
    # speaking.play_audio("../file/aphorism/close.wav")
    Alarm_del("21:46:00", 4)

def speak_yiyan():
    """使用语音功能朗读一言"""
    speaking.speak2(yiyan())

# 下面的测试代码被注释，若需要测试功能，请取消注释
# yiyan()
# speak_yiyan()
