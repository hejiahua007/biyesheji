"""
这个 Python 脚本提供了一个简单的界面,用于录制和保存语音备忘录。

主要功能包括:
1. 自动创建或打开位于 `"../file/beiwanglu/"` 目录下的备忘录文件。
2. 根据当前日期和时间生成备忘录文件名,格式为 `"YYYYMMDDHH.txt"`。
3. 使用 `wav_recognize.listen()` 函数录制语音备忘录。
4. 将录制的备忘录内容写入相应的文本文件。
5. 打印备忘录保存的文件路径。

该脚本依赖于 `os`、`datetime` 和 `wav_recognize` 模块来处理文件操作和语音录制。
"""
import os
import datetime
import wav_recognize

# 打开或创建备忘录文件
def open_memo():
    # 设置备忘录文件的目录路径
    memo_directory = "../file/beiwanglu/"
    # 如果目录不存在,则创建它
    if not os.path.exists(memo_directory):
        os.makedirs(memo_directory)

    # 获取当前时间并格式化为文件名
    current_time = datetime.datetime.now().strftime("%Y%m%d%H")
    memo_file_path = os.path.join(memo_directory, f"{current_time}.txt")

    # 如果文件已存在,打印提示信息
    if os.path.exists(memo_file_path):
        print(f"Opening existing memo file: {memo_file_path}")
    # 如果文件不存在,创建一个新的备忘录文件
    else:
        with open(memo_file_path, "w") as memo_file:
            print(f"Creating new memo file: {memo_file_path}")

    return memo_file_path

# 录制备忘录并保存到文件
def memorandum():
    # 打开备忘录文件
    file_path = open_memo()
    # 录制语音备忘录
    result = wav_recognize.listen()

    # 将录制的内容写入备忘录文件
    with open(file_path, "w") as memo_file:
        memo_file.write(result)

    print(f"Memo saved to: {file_path}")

# 打开备忘录
def open_memorandum():
    results = memorandum()