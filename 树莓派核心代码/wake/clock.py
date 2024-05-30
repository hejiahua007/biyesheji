# clock.py
# 这个模块用于管理闹钟功能，可以添加、删除和触发闹钟。
# 它使用了一个独立的线程来监控当前时间并触发闹钟。
# 主进程可以通过获取准备就绪的闹钟ID来执行相应的操作。
# 这个模块提供了线程安全的接口，可以在多个进程中使用。

import time
import threading
import queue

class AlarmThread(threading.Thread):
    def __init__(self, alarm_queue):
        super().__init__()
        self.alarms = {}  # 存储闹钟的字典，键为时间点，值为闹钟的标识符
        self.alarm_queue = alarm_queue

    def add_alarm(self, alarm_time, alarm_id):
        # 添加一个新的闹钟
        self.alarms[alarm_time] = alarm_id

    def remove_alarm(self, alarm_time, alarm_id):
        # 删除一个已存在的闹钟
        if alarm_time in self.alarms and self.alarms[alarm_time] == alarm_id:
            del self.alarms[alarm_time]

    def run(self):
        # 监控当前时间并触发闹钟
        while True:
            current_time = time.strftime("%H:%M:%S")
            if current_time in self.alarms:
                alarm_id = self.alarms[current_time]
                print(f"Alarm {alarm_id} triggered at {current_time}")
                self.alarm_queue.put(alarm_id)  # 将alarm_id放入队列
                self.remove_alarm(current_time, alarm_id)
            time.sleep(1)

# 创建线程安全的队列和闹钟线程
alarm_queue = queue.Queue()
alarm_thread = AlarmThread(alarm_queue)

def Alarm_init():
    # 初始化闹钟线程
    alarm_thread.start()

def Alarm_add(time, id):
    # 设置一个新的闹钟
    alarm_thread.add_alarm(time, id)

def Alarm_del(time, id):
    # 删除一个已存在的闹钟
    alarm_thread.remove_alarm(time, id)

def get_ready_id():
    try:
        # 从队列中获取准备就绪的闹钟ID
        alarm_id = alarm_queue.get_nowait()
        # 这里你可以根据alarm_id执行主进程的相应操作
        print(f"Received alarm_id in the main process: {alarm_id}")
        return alarm_id
    except queue.Empty:
        # 队列为空时会抛出异常，可以忽略
        return 0