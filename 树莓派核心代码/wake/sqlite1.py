# 文件名：sqlite1.py
# 功能描述：提供对SQLite数据库的操作，包括创建表、插入数据、查询数据等功能。

import sqlite3
import os
from datetime import datetime

# 指定数据库文件夹路径
db_dir = "/home/pi/Desktop/sleep/file/sqlite_file/"


def create_table(db_path, table_name):
    # 连接数据库，如果不存在则会创建
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    sql = '''
        CREATE TABLE IF NOT EXISTS '{}' (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            temperature FLOAT,
            humidity FLOAT,
            light_intensity FLOAT,
            co2_level FLOAT,
            sound_level FLOAT,
            wind_speed FLOAT
        )
    '''.format(table_name)
    # 创建表
    cursor.execute(sql)

    # 提交更改并关闭连接
    conn.commit()
    conn.close()


def insert_data(db_path, table_name, temperature, humidity, light_intensity, co2_level, sound_level, wind_speed):
    create_table(db_path, table_name)  # 创建表格

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # 获取当前时间
    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    # 插入数据
    cursor.execute(f'''
        INSERT INTO {table_name} (timestamp, temperature, humidity, light_intensity, co2_level, sound_level, wind_speed)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    ''', (current_time,temperature, humidity, light_intensity, co2_level, sound_level, wind_speed))

    # 提交更改并关闭连接
    conn.commit()
    conn.close()

def query_data(db_path, table_name):
    create_table(db_path, table_name)  # 创建表格

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # 查询数据
    cursor.execute(f'SELECT * FROM {table_name}')
    data = cursor.fetchall()

    # 关闭连接
    conn.close()

    return data

def insert_batch_data(db_path, table_name, batch_data):
    if not batch_data:
        return

    create_table(db_path, table_name)  # 创建表格
    
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()

    # 计算平均值
    avg_temperature = sum(entry[0] for entry in batch_data) / len(batch_data)
    avg_humidity = sum(entry[1] for entry in batch_data) / len(batch_data)
    avg_light_intensity = sum(entry[2] for entry in batch_data) / len(batch_data)
    avg_co2_level = sum(entry[3] for entry in batch_data) / len(batch_data)
    avg_sound_level = sum(entry[4] for entry in batch_data) / len(batch_data)
    avg_wind_speed = sum(entry[5] for entry in batch_data) / len(batch_data)

    # 获取当前时间
    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    # 插入平均值数据
    cursor.execute(f'''
        INSERT INTO {table_name} (timestamp, temperature, humidity, light_intensity, co2_level, sound_level, wind_speed)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    ''', (current_time, avg_temperature, avg_humidity, avg_light_intensity, avg_co2_level, avg_sound_level, avg_wind_speed))
    # 提交更改并关闭连接
    conn.commit()
    conn.close()

def get_db_path():
    # 获取当前日期
    current_date = datetime.now().strftime('%Y-%m-%d')
    # 构建数据库文件名
    db_name = f"{current_date}.db"
    # 构建数据库文件路径
    db_path = os.path.join(db_dir, db_name)
    
    return db_path

def get_table_name():
    # 获取当前小时
    current_hour = datetime.now().strftime('%H')
    # 构建表名
    table_name = f"table_{current_hour}"
    
    return table_name
def select_db_path(year, month, day):
    # 构建数据库文件名
    db_name = f"{year}-{month:02d}-{day:02d}.db"
    # 构建数据库文件路径
    db_path = os.path.join(db_dir, db_name)
    return db_path

def select_table_name(hour):
    # 构建表名
    table_name = f"table_{hour:02d}"    
    return table_name

def query_select_row(db_path, table_name,line):
    create_table(db_path, table_name)  # 创建表格

    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    
    # 查询第一行数据
    cursor.execute(f'SELECT * FROM {table_name} WHERE id = {line}')
    select_row_data = cursor.fetchone()

    # 关闭连接
    conn.close()

    return select_row_data
    
def query_select_table(db_path, table_name):
    create_table(db_path, table_name)  # 创建表格
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    # 查询所有数据
    cursor.execute(f'SELECT * FROM {table_name}')
    all_rows_data = cursor.fetchall()
    # 关闭连接
    conn.close()
    # 如果查询结果为空，返回一条全为零的数据
    if not all_rows_data:
        return [(0,) * len(cursor.description)]
    return all_rows_data

# 示例用法
#db_path = get_db_path()
#table_name = get_table_name()

# 插入单条数据
#insert_data(db_path, table_name, 25.5, 60, 500, 400, 70, 3.5)

# 插入批量数据
#data_batch = [
#    (25.5, 60, 500, 400, 70, 3.5),
#    (26.5, 62, 510, 410, 75, 3.8),
    # 添加更多数据
#]
#insert_batch_data(db_path, table_name, data_batch)

# 查询数据
#query_result = query_data(db_path, table_name)
#print(query_result)
