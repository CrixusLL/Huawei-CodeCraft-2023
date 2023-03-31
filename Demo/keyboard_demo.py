#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
import sys
import keyboard

def read_util_ok():         # 该函数用于读取输入直到得到一个OK
    while input() != "OK": pass

def is_key_down(key):       # 该函数用于判定按键是否按下, 可持续按下, 用于移动控制
    return keyboard.is_pressed(key)

pressed = [False] * 256
def is_key_pressed(key):    # 该函数用于对持续按下做判重，按住不放时只算一次, 避免每帧都产生购买等事件
    if is_key_down(key):
        ret = not pressed[ord(key)]
        pressed[ord(key)] = True
        return ret
    pressed[ord(key)] = False
    return False

if __name__ == '__main__':
    read_util_ok()      # 初始化时读入地图数据, 直到OK结束
    print('OK')         # 输出OK 表示初始化结束
    sys.stdout.flush()  # 别忘了flush一下标准输出

    try:
        robot_id = 0
        while True:
            # 首先输出读入的第一个整数: 帧ID
            frame_id = input().split(' ')[0]
            print(frame_id)

            # 忽略其他输入数据，读到OK为止
            read_util_ok()

            # 1234: 切换机器人
            if is_key_down('1') : robot_id = 0
            if is_key_down('2') : robot_id = 1
            if is_key_down('3') : robot_id = 2
            if is_key_down('4') : robot_id = 3

            # WSAD: 移动
            if is_key_down('w') : print("forward", robot_id, 9999)  # 判题器自动缩减9999为合法的最接近值
            elif is_key_down('s') : print("forward", robot_id, -9999)
            else: print("forward", robot_id, 0)
            
            if is_key_down('a') : print("rotate", robot_id, 9999)
            elif is_key_down('d') : print("rotate", robot_id, -9999)
            else: print("rotate", robot_id, 0)
            
            # JKL: 购买、出售、销毁指令
            if is_key_pressed('j') : print("buy", robot_id)
            if is_key_pressed('k') : print("sell", robot_id)
            if is_key_pressed('l') : print("destroy", robot_id)

            print('OK')         # 当前帧控制指令结束，输出OK
            sys.stdout.flush()  # flush标准输出，以便判题器及时接收数据

    except EOFError:    # 读到EOF时, 程序结束
        pass
