#文件名：translate.py
#功能描述：文本翻译，主要通过调用有道云API实现日语到中文（简体）的翻译。
import hashlib
import uuid
import time
import requests
import json

# 有道云API的key
youdao_Id = "**"  # 应用ID
youdao_key = "**"  # 应用密钥

# 文本翻译函数
def text2text_translate(words, model="youdao", src_lang="ja", target_lang="zh-CHS"):
    """
    使用有道云API将文本从一种语言翻译为另一种语言。
    
    参数：
        words (str): 待翻译的文本。
        model (str): 使用的翻译模型，默认值为 "youdao"。
        src_lang (str): 源语言代码，默认值为 "ja"（日语）。
        target_lang (str): 目标语言代码，默认值为 "zh-CHS"（中文简体）。
    
    返回值：
        str: 翻译后的文本。
    """
    if model == "youdao":
        def encrypt(signStr):
            """
            使用SHA-256算法加密字符串。
            
            参数：
                signStr (str): 待加密的字符串。
            
            返回值：
                str: 加密后的字符串。
            """
            hash_algorithm = hashlib.sha256()
            hash_algorithm.update(signStr.encode('utf-8'))
            return hash_algorithm.hexdigest()

        def truncate(q):
            """
            截断字符串，如果长度超过20，则保留前10个字符和后10个字符，中间用字符串长度代替。
            
            参数：
                q (str): 待截断的字符串。
            
            返回值：
                str: 截断后的字符串。
            """
            if q is None:
                return None
            size = len(q)
            return q if size <= 20 else q[0:10] + str(size) + q[size - 10:size]

        def do_request(data):
            """
            发送POST请求到有道云API以获取翻译结果。
            
            参数：
                data (dict): 请求数据。
            
            返回值：
                Response: 请求响应。
            """
            youdao_url = '**'  # 有道云API的URL
            headers = {'Content-Type': 'application/x-www-form-urlencoded'}  # 请求头
            return requests.post(youdao_url, data=data, headers=headers)

        q = words  # 待翻译的文本
        data = {}
        data['from'] = src_lang  # 翻译源语言
        data['to'] = target_lang  # 翻译目标语言
        data['signType'] = 'v3'
        curtime = str(int(time.time()))
        data['curtime'] = curtime  # 时间戳
        salt = str(uuid.uuid1())
        signStr = youdao_Id + truncate(q) + salt + curtime + youdao_key
        sign = encrypt(signStr)
        data['appKey'] = youdao_Id  # 应用ID
        data['q'] = q  # 翻译语句
        data['salt'] = salt
        data['sign'] = sign
        response = do_request(data)

        # 回复解码
        json_data = response.content.decode('utf-8')
        data = json.loads(json_data)
        translation = data['translation']

    return translation[0]
