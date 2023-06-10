import pandas as pd
import matplotlib.pyplot as plt

# 读取a.txt文件，并用":"分割每一行的两个数字，创建一个二元组列表
with open('a.txt', 'r') as f:
    a_list = [tuple(map(int, line.strip().split(':'))) for line in f]

# 读取b.txt文件，并用":"分割每一行的两个数字，创建一个二元组列表
with open('b.txt', 'r') as f:
    b_list = [tuple(map(int, line.strip().split(':'))) for line in f]

# 将a_list和b_list转换为pandas的DataFrame格式，方便后续处理
df_a = pd.DataFrame(a_list, columns=['x', 'y'])
df_b = pd.DataFrame(b_list, columns=['x', 'y'])

# 将b.txt的y值除以a.txt的y值得到新的纵坐标值
df_b['y'] = df_b['y'] / df_a['y']

print(df_b['y'])

# 绘制曲线图
plt.plot(df_a['x'], df_b['y'])
plt.show()

