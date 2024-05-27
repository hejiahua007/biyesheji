import socket
import threading
from multiprocessing import Pipe
import sqlite1
import select
HOST = '192.168.43.94'
APP_address = '192.168.43.1'
#APP_address = '192.168.43.1'
PC_address = '192.168.43.112'
DUOJI_address = '192.168.43.22'
PORT = 8888
commands = {
    'kaideng': lambda: send_to_client("LED:2\n", "192.168.43.20", APP_address) ,
    'guandeng': lambda: send_to_client("LED:0\n", "192.168.43.20", APP_address) ,
    'kaichuanglian': lambda: send_to_client("CHUANGLIAN:2\n", "192.168.43.22", APP_address),
    'kaichuanglian1': lambda: send_to_client("CHUANGLIAN:1\n", "192.168.43.22", APP_address),
    'guanchuanglian': lambda: send_to_client("CHUANGLIAN:0\n", "192.168.43.22", APP_address),
    'kaizhiwuqi': lambda: send_to_client("ZHIWUQI:2\n", "192.168.43.21", APP_address),   
    'guanzhiwuqi': lambda: send_to_client("ZHIWUQI:0\n", "192.168.43.21", APP_address),
    'kaifengshan': lambda: send_to_client("FENGSHAN:2\n", "192.168.43.22", APP_address),
    'guanfengshan': lambda: send_to_client("FENGSHAN:0\n", "192.168.43.22", APP_address),
    'kaifengshan1': lambda: send_to_client("FENGSHAN:1\n", "192.168.43.22", APP_address)
}
server_state = {
    "kongtiao": "0",
    "zhiwuqi": "0",
    "chuanglian": "0",
    "chuanghu": "0",
    "tishi": "0",
    "fengshan": "0",
    "led":"0"
}

lock = threading.Lock()
client_sockets = {}

def broadcast_message(message, sender_ip):
    with lock:
        for ip, socket_item in client_sockets.items():
            if ip != sender_ip:
                try:
                    socket_item.send(message.encode())
                except socket.error as e:
                    print(f"Error sending message to {ip}: {e}")

def send_to_client(message, target_ip,target_ip2):
    with lock:
        if target_ip in client_sockets:
            try:
                client_sockets[target_ip].send(message.encode())
            except socket.error as e:
                print(f"Error sending message to {target_ip}: {e}")
        else:
            print(f"Client with IP {target_ip} not found.Sending message to {target_ip2}")
            try:
                client_sockets[target_ip2].send(message.encode())
            except socket.error as e:
                print(f"Error sending message to {target_ip2}: {e}")

def handle_client(client_socket, client_address):
    global server_state  # 声明 server_state 为全局变量
    with lock:
        client_sockets[client_address[0]] = client_socket
    data_batch = []  # 初始化空列表

    while True:
        try:
            request = client_socket.recv(1024)
        # 处理请求...
        except ConnectionResetError:
            print("连接被客户端重置。")
        # 这里可以添加一些清理或重连的代码

        if not request:
            break

        message = request.decode()
        print(f"Received from {client_address[0]}:{client_address[1]}: {message}")

        if message.lower().strip() == "quit":
            print(f"Client {client_address[0]}:{client_address[1]} requested to quit")
            break

        if message.startswith("send_to"):
            parts = message.split(":")
            # 格式: send_to:192.168.43.1:fengsan=1
            if len(parts) == 3:
                target_ip = parts[1].strip()
                send_to_client(parts[2], target_ip)
                continue
        if message.startswith("select_row&"):
            parts = message.split("&")
            if len(parts) == 7:
                year = int(parts[1])
                month = int(parts[2])
                day = int(parts[3])
                hour = int(parts[4])
                minute = int(parts[5])
                # 利用日期和时间信息确定数据库文件和表格
                db_path = sqlite1.select_db_path(year, month, day)
                table_name = sqlite1.select_table_name(hour)
                # 取出选择的表格的第一行数据
                select_row_data = sqlite1.query_select_row(db_path, table_name, minute)
                formatted_data = format_data_for_client(select_row_data)
                print(formatted_data)
                send_to_client(formatted_data, PC_address, APP_address)
                continue
        if message.startswith("select_table&"):
            parts = message.split("&")
            if len(parts) == 7:
                year = int(parts[1])
                month = int(parts[2])
                day = int(parts[3])
                hour = int(parts[4])
                minute = int(parts[5])
                # 利用日期和时间信息确定数据库文件和表格
                db_path = sqlite1.select_db_path(year, month, day)
                table_name = sqlite1.select_table_name(hour)
                # 获取整个表的数据
                table_data = sqlite1.query_select_table(db_path, table_name) 
                formatted_data = format_data_for_client_table(table_data)
                print(formatted_data)
                send_to_client(formatted_data, PC_address, APP_address)
                continue
        if message.startswith("select_day&"):
            table_data_day = []
            parts = message.split("&")
            if len(parts) == 7:
                year = int(parts[1])
                month = int(parts[2])
                day = int(parts[3])
                for i in range(24):
                    db_path = sqlite1.select_db_path(year, month, day)
                    table_name = sqlite1.select_table_name(i)
                    table_data_tmp = sqlite1.query_select_table(db_path, table_name) 
                    # 检查 table_data_tmp 是否为空
                    if table_data_tmp:
                        # 初始化每个小时的数据
                        table_data_hour = [0.0] * len(table_data_tmp[0])
                        # 对每一条数据进行累加
                        for data_row in table_data_tmp:
                            for j in range(len(data_row)):
                                # 跳过时间戳列
                                if j == 1:
                                    continue
                                table_data_hour[j] += float(data_row[j])

                        # 计算平均值
                        data_count = len(table_data_tmp)
                        if data_count > 0:
                            table_data_hour = [value / data_count for value in table_data_hour]
                        table_data_hour[0]=i
                        table_data_day.append(table_data_hour)
                    else:
                        table_data_hour = [0.0] * 8
                        table_data_hour[0]=i
                        table_data_day.append(table_data_hour)

            formatted_data = format_data_for_client_table(table_data_day)
            print(formatted_data)
            send_to_client(formatted_data, PC_address, APP_address)
            continue

        if message.startswith("wendu"):
            parts = message.split("&")
            wendu = float(parts[0].strip().split("=")[1])
            shidu = float(parts[1].strip().split("=")[1])
            guangzhao = float(parts[2].strip().split("=")[1])
            co2 = float(parts[3].strip().split("=")[1])
            shengxiang = float(parts[4].strip().split("=")[1])
            fengli = float(parts[5].strip().split("=")[1])
            data_batch.append((wendu, shidu, guangzhao, co2, shengxiang, fengli))  # 将元组添加到列表中
        
        if message.startswith("GET_STATE_"):
            parts = message.split("&")
            if parts[1].strip()=="led_ready":
                server_state["led"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="kongtiao_ready":
                server_state["kongtiao"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="zhiwuqi_ready":
                server_state["zhiwuqi"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="chuanglian_ready":
                server_state["chuanglian"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="chuanghu_ready":
                server_state["chuanghu"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="tishi_ready":
                server_state["tishi"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            elif parts[1].strip()=="fengshan_ready":
                server_state["fengshan"]="1"
                state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
                state_data = state_data + "\n"
            continue
                
        if message.startswith("askstates"): 
            state_data = "&".join([f"{key}={value}" for key, value in server_state.items()])
            state_data = state_data + "\n"
            print(state_data)
            #state_data = "kongtiao=1&zhiwuqi=1&chuanglian=1&chuanghu=1&tishi=1&fengshan=1&led=1\n"
            #print(state_data)
            send_to_client(state_data, PC_address, APP_address)
            continue
            
        if message.startswith("app_ledon"):
            send_to_client("LED:2\n", "192.168.43.20", APP_address) 
            continue
        if message.startswith("app_ledoff"):
            send_to_client("LED:0\n", "192.168.43.20", APP_address)  
            continue
            
        if message.startswith("app_zhiwuqion"):
            send_to_client("ZHIWUQI:2\n", "192.168.43.21", APP_address) 
            continue
        if message.startswith("app_zhiwuqioff"):
            send_to_client("ZHIWUQI:0\n", "192.168.43.21", APP_address)   
            continue
            
        if message.startswith("app_kongtiaoon"):
            send_to_client("KONGTIAO:2\n", "192.168.1.6", APP_address) 
            continue
        if message.startswith("app_kongtiaooff"):
            send_to_client("KONGTIAO:0\n", "192.168.1.6", APP_address)  
            continue
        if message.startswith("app_chuanghuon"):
            send_to_client("CHUANGHU:2\n", "192.168.1.6", APP_address) 
            continue
        if message.startswith("app_chuanghuoff"):
            send_to_client("CHUANGHU:0\n", "192.168.1.6", APP_address)   
            continue
        if message.startswith("app_chuanglianon1"):
            send_to_client("CHUANGLIAN:1\n", "192.168.43.22", APP_address) 
            continue
        if message.startswith("app_chuanglianon2"):
            send_to_client("CHUANGLIAN:2\n", "192.168.43.22", APP_address) 
            continue
        if message.startswith("app_chuanglianoff"):
            send_to_client("CHUANGLIAN:0\n", "192.168.43.22", APP_address)   
            continue
        if message.startswith("app_tishion"):
            send_to_client("TISHI:2\n", "192.168.1.6", APP_address) 
            continue
        if message.startswith("app_tishioff"):
            send_to_client("TISHI:0\n", "192.168.1.6", APP_address)   
            continue
        if message.startswith("app_fengshanon1"):
            send_to_client("FENGSHAN:1\n", "192.168.43.22", APP_address) 
            continue
        if message.startswith("app_fengshanon2"):
            send_to_client("FENGSHAN:2\n", "192.168.43.22", APP_address) 
            continue
        if message.startswith("app_fengshanoff"):
            send_to_client("FENGSHAN:0\n", "192.168.43.22", APP_address)   
            continue

        if len(data_batch) == 4:  # 检查列表中的元组数量是否达到20
            db_path = sqlite1.get_db_path()
            table_name = sqlite1.get_table_name()
            sqlite1.insert_batch_data(db_path, table_name, data_batch)
            data_batch = []  # 清空 data_batch 列表
        # 处理接收到的其他消息
        handle_custom_commands(message, client_address[0])

    with lock:
        del client_sockets[client_address[0]]

    client_socket.close()

def handle_custom_commands(message, sender_ip):
    custom_commands = ["kongtiao", "jiashi", "chuanglian", "jingshi", "fengshan"]

    for command in custom_commands:
        if command in message:
            # 根据需要修改目标 IP
            target_ip = "192.168.43.112"
            send_to_client(message, '192.168.43.1',target_ip)
            return

    # 在这里添加其他处理接收到的消息的逻辑，0例如广播给所有客户端
    broadcast_message(message, sender_ip)

def format_data_for_client(row_data):
    if row_data is not None:
        formatted_data = f"wendu={row_data[2]}&shidu={row_data[3]}&guangzhao={row_data[4]}&co2={row_data[5]}&shengxiang={row_data[6]}&fengli={row_data[7]}&time={row_data[0]}\n"
        return formatted_data
    else:
        return f"wendu=none&shidu=none&guangzhao=none&co2=none&shengxiang=none&fengli=none&time=none\n"
def format_data_for_client_table(table_data):
    formatted_data=''
    if table_data is not None:
        for row_data in table_data:
            #wendu:1,shidu:2,guagnzhao:3,co2:4,shengxiang:5,
            formatted_data = formatted_data + f"1={row_data[2]}&2={row_data[3]}&3={row_data[4]}&4={row_data[5]}&5={row_data[6]}&6={row_data[7]}&7={row_data[0]}||"
        formatted_data = formatted_data + "\n"
        return formatted_data
    else:
        return f"1=none&2=none&3=none&4=none&5=none&6=none&7=none\n"


def run_server(pipe):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print(f"Server listening on {HOST}:{PORT}")
    server_socket.settimeout(5)  # 设置超时时间为5秒
    while True:
        try:
            client_socket, client_address = server_socket.accept()
            print(f"Accepted connection from {client_address[0]}:{client_address[1]}")
            
            client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
            client_thread.start()
        except socket.timeout:
            # 如果在超时时间内没有新连接尝试，执行下面的逻辑
            print("\n")
        #print("2")
        # 继续执行其他逻辑
        ready_sockets, _, _ = select.select([pipe], [], [], 5)  # 等待5秒钟来检查管道是否可读
        #print("3")
        if pipe in ready_sockets:  # 如果管道可读
            command = pipe.recv()
            #print("4")
            if command in commands:
                #print("5")
                print(command)
                commands[command]()
