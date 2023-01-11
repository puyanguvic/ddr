import numpy as np
import matplotlib.pyplot as plt
t = np.arange(0.0, 2.0, 0.01)
s = np.sin(2*np.pi*t)
#绘制函数图像
plt.plot(t,s)
#设置标题
plt.title(r'$\alpha_i> \beta_i$', fontsize=20)
#设置数学表达式
plt.text(0.6, 0.6, r'$\mathcal{A}\mathrm{sin}(2 \omega t)$', fontsize = 20)
#设置数学表达式
plt.text(0.1, -0.5, r'$\sqrt{2}$', fontsize=10)
plt.xlabel('time (s)')
plt.ylabel('volts (mV)')
plt.show()















import matplotlib.pyplot as plt
import numpy as np

algname = ['DGR', 'OSPF']
alg = {'DGR': [], 'OSPF': []}

# 处理数据
def load_data(f):
    a = np.loadtxt(f)
    lnum = 0
    asum = 0
    for item in a:
        asum += item
        lnum += 1
    avg = asum / lnum
    return avg


# 循环输入数据
def input_data(toponame):
    # 循环1-10Mbps
    for i in range(1, 11):
        # 循环算法名字
        for k in range(0, 5):
            file = 'E:\\Code\\infocomm2023\\exp1\\' + toponame + '\\' + algname[k] + '-' + str(i) + 'Mbps.txt'
            alg[algname[k]].append(load_data(file))

    # for item in algname:
    #     print(alg[item])

# 初始化画板
fig = plt.plot ()
# 字体设置
plt.rc('font',family='Times New Roman')
font1 = {'family': 'Times New Roman',
         'weight': 'normal',
         'size': 12,
         }
# x轴 10个点
x = range(1, 11)

# 画子图
def draw(ax):
    # 标题
    # ax.set_title('Abilene', font1)
    # 设置刻度范围
    # ax.set_xlim(1, 10)
    # ax.set_ylim(0, 350)
    # 开启网格
    ax.grid(True)
    # 画线
    form = ['s', 'p', 'd', 'o', '>']
    for m in range(0, 5):
        ax.plot(x, alg[algname[m]], form[m] + '-', markerfacecolor='none', label=algname[m])
    # x轴标签
    # ax.set_xlabel('Sending Rate (Mbps)', font1)
    # y轴标签
    # ax.set_ylabel('Delay (ms)', font1)

# 清空list
def clear():
    for n in algname:
        alg[n].clear()


axlist = [a1, a2, a3, a4]
for j in range(0, 4):
    input_data(topo[j])
    draw(axlist[j])
    clear()

# 图例
lines, labels = a1.get_legend_handles_labels()
fig.legend(lines, labels, loc='right')
fig.text(0.08, 0.5,'Delay (ms)', va='center', rotation='vertical')
# fig.text(0.5, 0.04, 'common X', ha='center')
plt.show()
