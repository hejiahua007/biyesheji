import os
import datetime
import wav_recognize

def open_memo():
    memo_directory = "../file/beiwanglu/"
    if not os.path.exists(memo_directory):
        os.makedirs(memo_directory)

    current_time = datetime.datetime.now().strftime("%Y%m%d%H")
    memo_file_path = os.path.join(memo_directory, f"{current_time}.txt")

    if os.path.exists(memo_file_path):
        print(f"Opening existing memo file: {memo_file_path}")
    else:
        with open(memo_file_path, "w") as memo_file:
            print(f"Creating new memo file: {memo_file_path}")

    return memo_file_path

def memorandum():
    file_path = open_memo()
    result = wav_recognize.listen()

    with open(file_path, "w") as memo_file:
        memo_file.write(result)

    print(f"Memo saved to: {file_path}")


def open_memorandum():
    #speaking.play_audio("../file/beiwanglu/zhiling/kaibeiwanglu.wav")
    results = memorandum()
    
#open_memorandum()
