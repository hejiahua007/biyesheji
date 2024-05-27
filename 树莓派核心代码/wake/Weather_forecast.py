import requests
import time
import speaking
from clock import Alarm_add,Alarm_del
api_key = "*"
city_name = "麻章"

# 通过城市查询接口获取城市ID
city_url = f"*"

def weather_forecast():
    city_response = requests.get(city_url)
    city_data = city_response.json()

    if city_data["code"] == "200":
        city_id = city_data["location"][0]["id"]
        result = f"城市 {city_name} 对应的城市ID 为：{city_id}\n\n"

        # 使用城市ID查询实时天气信息
        weather_url = f"https://devapi.qweather.com/v7/weather/3d?key={api_key}&location={city_id}"
        weather_response = requests.get(weather_url)
        weather_data = weather_response.json()

        if weather_data["code"] == "200":
            for i in range(3):  # Change the range to 1
                daily = weather_data["daily"]
                result += (
                    f"{daily[i]['fxDate']},{city_name}的天气情况：\n"
                    f"天气状况：{daily[i]['textDay']}\n"
                    f"最高温度：{daily[i]['tempMax']}℃\n"
                    f"最低温度：{daily[i]['tempMin']}℃\n"
                    f"白天风向：{daily[i]['windDirDay']}\n"
                    f"白天风速：{daily[i]['windSpeedDay']}km/h\n"
                    f"紫外线强度指数：{daily[i]['uvIndex']}\n"
                    f"相对湿度：{daily[i]['humidity']}%\n"
                    f"大气压强：{daily[i]['pressure']}hPa\n"
                    f"能见度：{daily[i]['vis']}km\n\n"
                )

            return result
        else:
            return "获取天气信息失败！"
    else:
        return "获取城市信息失败！"

def open_weather_forecast():#
    Alarm_add("7:00:00", 1)
    # 播报"天气预报已开启，今后每天都将播报往后三天的天气"
    #speaking.play_audio("../file/Weather_forecast/kaiqi.wav")

def close_weather_forecast():
    Alarm_del("7:00:00", 1)
    # 播报"天气预报已关闭"
    #speaking.play_audio("../file/Weather_forecast/guanbi.wav")
    
def speak_weather_forecast():
    speaking.speak2(weather_forecast())
#speak_weather_forecast()

#print(weather_forecast())

