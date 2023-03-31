#!/bin/bash
import sys
import numpy as np


class Map():
    def __init__(self):
        self.width = 50
        self.height = 50
        self.money = 0
        self.worktable_num = 0
        self.robots = []
        self.worktables_list = []  # by order
        self.wt_to_be_added = {1:[], 2:[], 3:[], 4:[], 5:[], 6:[], 7:[]}
        self.wt_to_be_purchased_123 = []
        self.wt_to_be_purchased_4567 = []
    
    #def reset(self):
    #    self.wt_to_be_purchased = []
    #    self.wt_to_be_added = []


class Robot():
    def __init__(self):
        self.id = -1
        # self.radius = 0.45
        # self.radius_with_goods = 0.53
        # self.density = 20
        # self.quality = np.pi * np.square(self.radius) * self.density
        # self.quality_with_goods = np.pi * \
        #     np.square(self.radius_with_goods) * self.density
        self.on_task_dest = -1  # the worktable robot heading for
        self.task = 0   # 1 buy, 2 sell, 3 
        self.check_flag = False

        self.loc = (0, 0)
        self.worktableID = -1
        self.goods_type_info = 0    # manual
        self.goods_type = 0  # [0, 7]
        self.optional_wt = []
        # self.time_coef = 0.0
        # self.collision_coef = 0.0
        self.angle_speed = 0.0
        self.line_speed = (0.0, 0.0)
        self.orientation = 0.0  # [-pi, pi]

    # The robot should go to the corresponding worktable(s) when it carries goods
    def get_optional_wt(self):
        ori_material = [0, 1, 2, 3, 4, 5, 6, 7]
        optional_worktables = [
            [], [4, 5, 9], [4, 6, 9], [5, 6, 9], [7, 9], [7, 9], [7, 9], [8, 9]
        ]
        self.optional_wt = optional_worktables[ori_material[self.goods_type]]

    def forward(self, line_speed):
        sys.stdout.write('forward %d %f\n' % (self.id, line_speed))

    def rotate(self, angle_speed):
        sys.stdout.write('rotate %d %f\n' % (self.id, angle_speed))

    def buy(self):
        sys.stdout.write('buy %d\n' % self.id)

    def sell(self):
        sys.stdout.write('sell %d\n' % self.id)

    def destroy(self):
        sys.stdout.write('destroy %d\n' % self.id)


class Worktable():
    def __init__(self, type):
        self.type = type
        self.id = -1
        self.id_sell_after_buy = -1
        self.loc = (0, 0)
        self.raw_material = [-1]
        self.work_cycle = 0
        self.production = 0
        self.remaining_time = 0
        self.raw_material_status = '00000000'
        self.material_ready = [0, 0, 0, 0, 0, 0, 0, 0]
        self.production_status = 0
        self.production_map_to_wt = []
        self.on_task_robot = -1  # judge whether the worktable is occupied
        self.get_info(type)

    def get_info(self, type):
        info = [
            [[-1], 50, 1],
            [[-1], 50, 2],
            [[-1], 50, 3],
            [[1, 2], 500, 4],
            [[1, 3], 500, 5],
            [[2, 3], 500, 6],
            [[4, 5, 6], 1000, 7],
            [[7], 1, 0],
            [[1, 2, 3, 4, 5, 6, 7], 1, 0]
        ]
        materials_map_to_wt = [
            [], [4, 5, 9], [4, 6, 9], [5, 6, 9], [7, 9], [7, 9], [7, 9], [8, 9]
        ]
        self.raw_material = info[type-1][0]
        self.work_cycle = info[type-1][1]
        self.production = info[type-1][2]
        self.production_map_to_wt = materials_map_to_wt[self.production]

    def to_be_purchased(self):
        if self.production_status:
            if len(map_info.wt_to_be_added[self.production]):
                min_dist_wt = float('inf')
                for wt_id in map_info.wt_to_be_added[self.production]:
                    wt = map_info.worktables_list[wt_id]
                    if wt.material_ready[self.production] == 0:
                        tmp_dist, _ = dist(self.loc, wt.loc)
                        if tmp_dist < min_dist_wt:
                            min_dist_wt = tmp_dist
                            min_id = wt_id
                if min_dist_wt != float('inf'):
                    self.id_sell_after_buy = min_id
                    map_info.worktables_list[min_id].material_ready[self.production] = 1
                    return True
        return False


    def to_be_added(self):
        if self.production_status and self.remaining_time > 0:
            return False
        else:
            for i in range(1,8):
                if i in self.raw_material and self.raw_material_status[i] == '0':
                    if self.id not in map_info.wt_to_be_added[i]:
                        map_info.wt_to_be_added[i].append(self.id)
            return True
        
def read_map():
    map_info = Map()
    row = 0
    worktable_num = 0
    robot_count = 0
    while True:
        col = 0
        line = input()
        if line == "OK":
            map_info.worktable_num = worktable_num
            return map_info
        for char in line:
            if char >= "1" and char <= "9":
                worktable = Worktable(int(char))
                worktable.id = worktable_num
                worktable.loc = np.array([0.25+col*0.5, 49.75-row*0.5])
                map_info.worktables_list.append(worktable)
                worktable_num += 1
            elif char == "A":
                robot = Robot()
                robot.id = robot_count
                robot.loc = np.array([0.25+col*0.5, 49.75-row*0.5])
                map_info.robots.append(robot)
                robot_count += 1
            col += 1
        row += 1


def read_util_ok():
    while True:
        worktable_total = int(input())
        for worktable_id in range(worktable_total):
            line = input()
            parts = line.split(' ')
            map_info.worktables_list[worktable_id].remaining_time = int(
                parts[3])
            map_info.worktables_list[worktable_id].raw_material_status = bin(int(parts[4]))[
                ::-1][:-2].ljust(8, "0")
            map_info.worktables_list[worktable_id].production_status = int(
                parts[5])
            # find free worktables
            if map_info.worktables_list[worktable_id].on_task_robot == -1:
                if parts[0] not in ['1', '2', '3']:
                    map_info.worktables_list[worktable_id].to_be_added()
                if parts[0] in ['1', '2', '3']:
                    if worktable_id not in map_info.wt_to_be_purchased_123:
                        if map_info.worktables_list[worktable_id].to_be_purchased():
                            map_info.wt_to_be_purchased_123.append(worktable_id)
                elif parts[0] in ['4', '5', '6', '7']:
                    if worktable_id not in map_info.wt_to_be_purchased_4567:
                        if map_info.worktables_list[worktable_id].to_be_purchased():
                            map_info.wt_to_be_purchased_4567.append(worktable_id)
        for robot_id in range(4):
            line = input()
            parts = line.split(' ')
            map_info.robots[robot_id].worktableID = int(parts[0])
            map_info.robots[robot_id].goods_type = int(parts[1])
            map_info.robots[robot_id].time_coef = float(parts[2])
            map_info.robots[robot_id].collision_coef = float(parts[3])
            map_info.robots[robot_id].angle_speed = float(parts[4])
            map_info.robots[robot_id].line_speed = (
                float(parts[5]), float(parts[6]))
            map_info.robots[robot_id].orientation = float(parts[7])
            map_info.robots[robot_id].loc = np.array(
                [float(parts[8]), float(parts[9])])
        line = input()
        if line == "OK":
            break


def finish():
    sys.stdout.write('OK\n')
    sys.stdout.flush()


def dist(ori, dest):
    dist = np.sqrt(np.sum(np.square(dest - ori)))
    delta_x = dest[0] - ori[0]
    delta_y = dest[1] - ori[1]
    if delta_x * delta_y == 0:
        if delta_x:
            ori_to_dest_radian = 0 if delta_x > 0 else np.pi
        elif delta_y:
            ori_to_dest_radian = np.pi / 2 if delta_y > 0 else -np.pi/2
        else:
            ori_to_dest_radian = 0
    elif delta_x > 0:
        ori_to_dest_radian = np.arctan(delta_y/delta_x)
    elif delta_x < 0:
        tmp_radian = np.arctan(delta_y/delta_x)
        ori_to_dest_radian = tmp_radian + np.pi if tmp_radian < 0 else tmp_radian - np.pi
    return dist, ori_to_dest_radian


def task_manager():
    for i in range(4):
        if map_info.robots[i].on_task_dest == -1:
            if not map_info.robots[i].goods_type:
                if len(map_info.wt_to_be_purchased_4567):
                    min_dist_wt_4567 = float('inf')
                    for wt_4567 in map_info.wt_to_be_purchased_4567:
                        tmp_dist, _ = dist(map_info.robots[i].loc, map_info.worktables_list[wt_4567].loc)
                        if tmp_dist < min_dist_wt_4567:
                            min_dist_wt_4567 = tmp_dist
                            min_index = wt_4567
                    wt = map_info.worktables_list[min_index]
                    map_info.wt_to_be_purchased_4567.remove(min_index)
                    map_info.robots[i].on_task_dest = wt.id
                    map_info.robots[i].task = 1
                    wt.on_task_robot = i  
                elif len(map_info.wt_to_be_purchased_123):
                    min_dist_wt_123 = float('inf')
                    for wt_123 in map_info.wt_to_be_purchased_123:
                        tmp_dist, _ = dist(map_info.robots[i].loc, map_info.worktables_list[wt_123].loc)
                        if tmp_dist < min_dist_wt_123:
                            min_dist_wt_123 = tmp_dist
                            min_index = wt_123
                    wt = map_info.worktables_list[min_index]
                    map_info.wt_to_be_purchased_123.remove(min_index)
                    map_info.robots[i].on_task_dest = wt.id
                    map_info.robots[i].task = 1
                    wt.on_task_robot = i
            # else:
            #     map_info.robots[i].get_optional_wt()
            #     min_dist_wt = float('inf')
            #     for wt_id in map_info.wt_to_be_added:
            #         wt = map_info.worktables_list[wt_id]
            #         if wt.on_task_robot != -1:
            #             continue
            #         if wt.type in map_info.robots[i].optional_wt:
            #             if wt.raw_material_status[map_info.robots[i].goods_type] == '0':
            #                    tmp_dist, _ = dist(map_info.robots[i].loc, wt.loc)
            #                    if tmp_dist < min_dist_wt:
            #                        min_dist_wt = tmp_dist
            #                        min_wt = wt
            #     if min_dist_wt == float('inf'): # destroy
            #         map_info.robots[i].task = 3
            #     else:  
            #         map_info.robots[i].on_task_dest = min_wt.id
            #         map_info.robots[i].task = 2
            #         min_wt.on_task_robot = i
            #         map_info.wt_to_be_added.remove(min_wt.id)
                
                
                
def reset(robot_id):
    if map_info.robots[robot_id].task == 1:
        tmp_wt_id = map_info.robots[robot_id].on_task_dest
        map_info.robots[robot_id].on_task_dest = map_info.worktables_list[map_info.robots[robot_id].on_task_dest].id_sell_after_buy
        map_info.worktables_list[tmp_wt_id].id_sell_after_buy = -1
        map_info.worktables_list[tmp_wt_id].on_task_robot = -1
        map_info.worktables_list[map_info.robots[robot_id].on_task_dest].on_task_robot = robot_id
        map_info.robots[robot_id].task = 2
        map_info.robots[robot_id].goods_type_info = map_info.robots[robot_id].goods_type
        map_info.robots[robot_id].check_flag = False
        return
    if map_info.robots[robot_id].task == 2:
        map_info.worktables_list[map_info.robots[robot_id].on_task_dest].on_task_robot = -1
        map_info.worktables_list[map_info.robots[robot_id].on_task_dest].material_ready[map_info.robots[robot_id].goods_type_info] = 0
        map_info.wt_to_be_added[map_info.robots[robot_id].goods_type_info].remove(map_info.robots[robot_id].on_task_dest)
    map_info.robots[robot_id].goods_type_info = 0
    map_info.robots[robot_id].on_task_dest = -1
    map_info.robots[robot_id].task = 0
    map_info.robots[robot_id].check_flag = False

def check(robot_id):
    if map_info.robots[robot_id].on_task_dest > -1:
        if map_info.robots[robot_id].task == 1:    # buy
            if map_info.robots[robot_id].goods_type == map_info.worktables_list[map_info.robots[robot_id].on_task_dest].type:
                reset(robot_id)
                return True
            else:
                map_info.robots[robot_id].task = 3
                return False
        elif map_info.robots[robot_id].task == 2:  # sell
            if map_info.worktables_list[map_info.robots[robot_id].on_task_dest].raw_material_status[map_info.robots[robot_id].goods_type] == '0':
                reset(robot_id)
            else:
                map_info.robots[robot_id].task = 3
            return False

def collision_detect(robot_id, delta_radian, line_speed):
    for i in range(4):
        if i != robot_id:
            d, _ = dist(map_info.robots[robot_id].loc, map_info.robots[i].loc)
            if d < 2.12 and \
                abs(map_info.robots[robot_id].orientation) + abs(map_info.robots[i].orientation) > np.pi / 2:
                delta_radian = (delta_radian + (robot_id + 1) * np.pi / 4) % np.pi - np.pi
                break
    return delta_radian, line_speed


def action():
    for i in range(4):
        if map_info.robots[i].task == 3:    # destroy
            map_info.robots[i].destroy()
            reset(i)

        elif map_info.robots[i].on_task_dest != -1:
            d, dest_radian = dist(
                map_info.robots[i].loc, map_info.worktables_list[map_info.robots[i].on_task_dest].loc)
            ori_radian = map_info.robots[i].orientation
            delta_radian = dest_radian - ori_radian
            if abs(delta_radian) > np.pi:
                delta_radian = delta_radian + \
                2 * np.pi if delta_radian < 0 else delta_radian - 2 * np.pi
            if abs(delta_radian) <= np.pi / 2:  #line speed can only be generated when the delta_angle <= 90
                #s = (-36) / np.pi * delta_radian + 6
                s = 6
                if d < 1.5:
                    s = 2
            else:
                s = 3.5
            if d < 0.4 and map_info.robots[i].worktableID == map_info.worktables_list[map_info.robots[i].on_task_dest].id:
                s = 0.8
                on_task_dest = map_info.robots[i].on_task_dest
                if map_info.robots[i].check_flag:
                    if check(i):
                        continue
                if map_info.robots[i].task == 1:
                    map_info.robots[i].buy()
                    map_info.robots[i].check_flag = True
                elif map_info.robots[i].task == 2:
                    map_info.robots[i].sell()
                    map_info.robots[i].check_flag = True
                # elif map_info.robots[i].task == 0 and map_info.robots[i].goods_type == 0:  # buy after sell
                #     if map_info.worktables_list[on_task_dest].on_task_robot == -1:
                #         if map_info.worktables_list[on_task_dest].production_status == 1:
                #             if on_task_dest in map_info.wt_to_be_purchased_123:
                #                 map_info.wt_to_be_purchased_123.remove(on_task_dest)
                #             elif on_task_dest in map_info.wt_to_be_purchased_4567:
                #                 map_info.wt_to_be_purchased_4567.remove(on_task_dest)
                #             map_info.robots[i].on_task_dest = on_task_dest
                #             map_info.robots[i].task = 1
                #             map_info.worktables_list[on_task_dest].on_task_robot = i

            delta_radian, s = collision_detect(i, delta_radian, s)
            map_info.robots[i].rotate(delta_radian / 0.02)
            map_info.robots[i].forward(s)



if __name__ == '__main__':
    map_info = read_map()
    finish()
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        parts = line.split(' ')
        frame_id = int(parts[0])
        map_info.money = int(parts[1])
        read_util_ok()
        sys.stdout.write('%d\n' % frame_id)
        task_manager()
        action()
        finish()
