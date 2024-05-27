import os
import sys
from multiprocessing import Process, Pipe
import time

def run_server(pipe):
    import tcp_server
    tcp_server.run_server(pipe)
    
if __name__ == '__main__':
    parent_pipe, child_pipe = Pipe()

    child_pid = os.fork()

    if child_pid == 0:
        # 子进程执行的代码
        child_pipe.close()
        import tcp_server
        tcp_server.run_server(parent_pipe)
        sys.exit()
    else:
        #while(1):
           #time.sleep( 1 )
        # 父进程执行的代码
        parent_pipe.close()
        
        import wake
        import wav_recognize2
        #import recognize2
        import speaking
        import chat
        import translate
        import Weather_forecast
        import clock
        import Aphorisms
        import time
        import music
        import Aphorisms
        import memorandum
        import sleep_detection
        # Define a dictionary to map user commands to audio files
        commands_to_audio11 = {
            '打开灯光': "../file/zhiling/kaideng.wav",
            '关闭灯光': "../file/zhiling/guandeng.wav",
            '打开窗帘': "../file/zhiling/kaichuanglian.wav",
            '关闭窗帘': "../file/zhiling/guanchuanglian.wav",
            '开一点窗帘':"../file/zhiling/kaiyidianchuanglian.wav",
            '打开制雾器': "../file/zhiling/kaizhiwuqi.wav",
            '关闭制雾器': "../file/zhiling/guanzhiwuqi.wav",
            '打开风扇': "../file/zhiling/kaifengshan.wav",
            '关闭风扇': "../file/zhiling/guanfengshan.wav",
            '开一档风扇':"../file/zhiling/kaiyidangfengshan.wav",
            
            '打开每日早晨音乐': "../file/music/zhiling/dakaimeirichenjianyinyue.wav",
            '关闭每日早晨音乐': "../file/music/zhiling/guanbimeirichenjianyinyue.wav",
            '打开每日睡前音乐': "../file/music/zhiling/dakaimeirishuiqianyinyue.wav",
            '关闭每日睡前音乐': "../file/music/zhiling/guanbimeirishuiqianyinyue.wav",
            '打开音乐': "../file/music/zhiling/dakaiyinyue.wav",                        
            '打开备忘录': "../file/beiwanglu/zhiling/kaibeiwanglu.wav",            
            '启动每日天气预报': "../file/Weather_forecast/zhiling/kaiqi.wav",
            '关闭每日天气预报': "../file/Weather_forecast/zhiling/guantianqiyubao.wav",
            '打开天气预报': "../file/Weather_forecast/zhiling/kaitianqiyubao.wav",
            '打开每日一言': "../file/aphorisms/zhiling/dakaimeiriyiyan.wav",
            '关闭每日一言': "../file/aphorisms/zhiling/guanbimeiriyiyan.wav",
            '打开一言': "../file/aphorisms/zhiling/dakaiyiyan.wav",
                        
            '开启每日睡前状态检测': "../file/detection/zhiling/kaishuiqianzhuangtaijiancha.wav",
            '关闭每日睡前状态检测': "../file/detection/zhiling/guanshuiqianzhuangtaijiancha.wav",
            '打开睡前状态检测': "../file/detection/zhiling/zhengzaidakaishuiqianzhuangtaijiance.wav"
        }
        commands_to_audio12 = {
            '打开灯光': "../file/zhiling_jan/kaideng.wav",
            '关闭灯光': "../file/zhiling_jan/guandeng.wav",
            '打开窗帘': "../file/zhiling_jan/kaichuanglian.wav",
            '关闭窗帘': "../file/zhiling_jan/guanchuanglian.wav",
            '开一点窗帘':"../file/zhiling_jan/kaiyidianchuanglian.wav",
            '打开制雾器': "../file/zhiling_jan/kaizhiwuqi.wav",
            '关闭制雾器': "../file/zhiling_jan/guanzhiwuqi.wav",
            '打开风扇': "../file/zhiling_jan/kaifengshan.wav",
            '关闭风扇': "../file/zhiling_jan/guanfengshan.wav",
            '开一档风扇':"../file/zhiling_jan/kaiyidangfengshan.wav",
            
            '打开每日早晨音乐': "../file/music/zhiling_jan/dakaimeirichenjianyinyue.wav",
            '关闭每日早晨音乐': "../file/music/zhiling_jan/guanbimeirichenjianyinyue.wav",
            '打开每日睡前音乐': "../file/music/zhiling_jan/dakaimeirishuiqianyinyue.wav",
            '关闭每日睡前音乐': "../file/music/zhiling_jan/guanbimeirishuiqianyinyue.wav",
            '打开音乐': "../file/music/zhiling_jan/dakaiyinyue.wav",                        
            '打开备忘录': "../file/beiwanglu/zhiling_jan/kaibeiwanglu.wav",            
            '启动每日天气预报': "../file/Weather_forecast/zhiling_jan/kaiqi.wav",
            '关闭每日天气预报': "../file/Weather_forecast/zhiling_jan/guantianqiyubao.wav",
            '打开天气预报': "../file/Weather_forecast/zhiling_jan/kaitianqiyubao.wav",
            '打开每日一言': "../file/aphorisms/zhiling_jan/dakaimeiriyiyan.wav",
            '关闭每日一言': "../file/aphorisms/zhiling_jan/guanbimeiriyiyan.wav",
            '打开一言': "../file/aphorisms/zhiling_jan/dakaiyiyan.wav",
                        
            '开启每日睡前状态检测': "../file/detection/zhiling_jan/kaishuiqianzhuangtaijiancha.wav",
            '关闭每日睡前状态检测': "../file/detection/zhiling_jan/guanshuiqianzhuangtaijiancha.wav",
            '打开睡前状态检测': "../file/detection/zhiling_jan/zhengzaidakaishuiqianzhuangtaijiance.wav"
        }
        functions = {
             '打开灯光': lambda:child_pipe.send("kaideng"),
             '关闭灯光': lambda:child_pipe.send("guandeng"),
             '打开窗帘': lambda:child_pipe.send("kaichuanglian"),
             '关闭窗帘': lambda:child_pipe.send("guanchuanglian"),
             '开一点窗帘':lambda:child_pipe.send("kaichuanglian1"),
             '打开制雾器': lambda:child_pipe.send("kaizhiwuqi"),
             '关闭制雾器': lambda:child_pipe.send("guanzhiwuqi"),
             '打开风扇': lambda:child_pipe.send("kaifengshan"),
             '关闭风扇': lambda:child_pipe.send("guanfengshan"),
             '开一档风扇':lambda:child_pipe.send("kaifengshan1"),
             
            '打开每日早晨音乐': lambda:music.open_n_music(),
            '关闭每日早晨音乐': lambda:music.stop_n_music(),
            '打开每日睡前音乐': lambda:music.open_m_music(),
            '关闭每日睡前音乐': lambda:music.stop_m_music(),
            '打开音乐': lambda:music.open_music(),             
            '打开备忘录': lambda:memorandum.open_memorandum(),
            '打开每日天气预报': lambda:Weather_forecast.open_weather_forecast(),
            '关闭每日天气预报': lambda:Weather_forecast.close_weather_forecast(),
            '打开天气预报': lambda:Weather_forecast.speak_weather_forecast(),            
            '打开每日一言': lambda:Aphorisms.open_yiyan(),
            '关闭每日一言': lambda:Aphorisms.close_yiyan(),
            '打开一言': lambda:Aphorisms.speak_yiyan(),
            
             '开启每日睡前状态检测': lambda:sleep_detection.start_sleep_status_detection(),
             '关闭每日睡前状态检测': lambda:sleep_detection.stop_sleep_status_detection(),
             '打开睡前状态检测': lambda:sleep_detection.detection()
        }
        
        language = 0
        chat.tmp_prompt = []
        clock.Alarm_init()
        action_id=0
        #chat.tmp_prompt = chat.prompt_ch
        while 1:
            action_id = clock.get_ready_id()
            print(action_id)
            if action_id==1:
                print("weatherforecast action")
                results1 = Weather_forecast.weather_forecast()
                speaking.speak2(results1)
            if action_id==2:
                print("morning music action")
                music.m_play_audio()
            if action_id==3:
                print("night music action")
                music.n_play_audio()
            if action_id==4:
                print("night music action")
                results4 = Aphorisms.yiyan()
                speaking.speak2(results4)
            if action_id==5:
                print("shui qian jian ce action")
                sleep_detection.detection()

                
            print("进入循环")
            
            if wake.keyword_wake_up() == 1:
                print("打开识别")
                speaking.play_audio("../file/yuyin/yoush.wav")
                user_word = wav_recognize2.listen()
                #user_word = '打开音乐'
                print("打印所识别到的：",user_word)
            else:
                continue
            
            
            if user_word in commands_to_audio11:
                speaking.play_audio(commands_to_audio11[user_word]) if language == 0 else speaking.play_audio(commands_to_audio12[user_word])
                functions[user_word]()
                user_word=''
            elif user_word != '':
                if '说中文' in user_word:
                    language = 0
                    chat.tmp_prompt = chat.prompt_ch
                    speaking.play_audio("../file/yuyin/haode.wav")
                    #speaking.speak("好的，主人！", language)
                    #speaking.speak2("好的，主人！")
                    continue
                elif '说日语' in user_word:
                    language = 1
                    chat.tmp_prompt = chat.prompt_jan
                    speaking.play_audio("../file/yuyin_jan/haode.wav")
                    #speaking.speak("はい、ご主人様です!", language)
                    continue
                if language == 0:
                    chatword = chat.chat(user_word, chat.tmp_prompt)
                    print("打印chat的返回：", chatword)
                    #speaking.speak(chatword, language)
                    speaking.speak2(chatword)
                else:
                    user_word = translate.text2text_translate(user_word,"youdao","zh-CHS","ja")#将用户讲的中文翻译成日文
                    print("打印翻译后的用户的话语：",user_word)
                    chatword = chat.chat(user_word, chat.tmp_prompt)#发送日文给gpt，获得日文回答
                    print("打印chat的回答：", chatword)  # 打印日文
                    speaking.speak(chatword,language)#合成日文语音
                    chatword = translate.text2text_translate(chatword,"youdao","ja","zh-CHS")#将用户讲的日文翻译成中文
                    print("打印chat回答的翻译：",chatword)#打印日文的翻译
            elif user_word == '':
                print('你貌似没有说话')
