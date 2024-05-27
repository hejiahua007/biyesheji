import requests
import speaking
from clock import Alarm_add,Alarm_del

url = "https://v1.hitokoto.cn/?c=a&c=c&c=e&c=f&c=h&c=i&c=k"

def yiyan():
	response = requests.get(url)
	data = response.json()

	# 检查响应状态码
	if response.status_code == 200:
		hitokoto = data["hitokoto"]
		from_who = data["from_who"]
		print("Hitokoto:", hitokoto)
	else:
		print("请求失败:", data["message"])
	#speaking.speak(hitokoto,0)
	return hitokoto
	
def open_yiyan():
    #speaking.play_audio("../file/aphorism/kaiqi.wav")
    #Alarm_add("21:50:00", 4)
    Alarm_add("15:55:00", 4)
    Alarm_add("15:56:00", 4)
    Alarm_add("15:57:00", 4)
    
    
def close_yiyan():
    #speaking.play_audio("../file/aphorism/close.wav")
    Alarm_del("21:46:00", 4)

def speak_yiyan():
    speaking.speak2(yiyan())
    
    
#yiyan()
#speak_yiyan()
