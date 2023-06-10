import os
import random

def mkdir(path):
    b_exist = os.path.exists(path)

    if not b_exist:  # 判断是否存在文件夹如果不存在则创建为文件夹
        os.makedirs(path)  # makedirs 创建文件时如果路径不存在会创建这个路径
        print("---  new folder...  ---")
        print("---  OK  ---")

    else:
        print("---  folder already exists!  ---")


def generate_random_str(random_length=10, count=100):
    """
    生成一个指定长度的随机字符串
    """
    str_list = []
    random_str = ''
    base_str = 'ABCDEFG'
    length = len(base_str) -1
    for j in range(count):
        for i in range(random_length):
            random_str += base_str[random.randint(0, length)]

        random_str += '\n'
        str_list.append(random_str)
        random_str = ''

    return str_list


def generate_txt_file(file_name: str, msg: list):
    path = os.path.join(os.getcwd() + '/test500', file_name)
    with open(path, 'w', encoding='UTF-8') as f:
        for str in msg:
            f.write(str)


def generate_txt_files(count: int = 100):
    for i in range(count):
        file_name = str(i) + '.txt'
        msg = generate_random_str()
        generate_txt_file(file_name, msg)



if __name__ == '__main__':
    path = os.getcwd() + '/test500'
    mkdir(path)
    generate_txt_files(1000)



