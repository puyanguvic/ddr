import matplotlib.pyplot as plt
import numpy as np
# plt.style.use('ieee')
plt.style.available

dgr0 = []
dw0 = []
dwe0 = []
ecmp0 = []
ospf0 = []

def load_data(f):
    a = np.loadtxt(f)
    lnum = 0
    asum = 0
    for item in a:
        asum += item
        lnum += 1
    avg = asum / lnum
    return avg


for i in range(1, 11):
    file = 'E:\\Code\\infocomm2023\\exp1\\abilene\\dgr-' + str(i) + 'Mbps.txt'
    dgr0.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\abilene\\dw-' + str(i) + 'Mbps.txt'
    dw0.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\abilene\\dwe-' + str(i) + 'Mbps.txt'
    dwe0.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\abilene\\ecmp-' + str(i) + 'Mbps.txt'
    ecmp0.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\abilene\\ospf-' + str(i) + 'Mbps.txt'
    ospf0.append(load_data(file))

font1 = {'family': 'Times New Roman',
         'weight': 'normal',
         'size': 12,
         }

# fig = plt.figure()
fig, (a1,a2,a3,a4) = plt.subplots(1,4,figsize=(8,2),sharey=True)
x = range(1, 11)
# a1 = fig.add_subplot(141)
# 设置刻度范围
a1.set_xlim(1, 10)
a1.set_ylim(0, 350)
# 开启网格
a1.grid(True)
# 画线

l1 = a1.plot(x, dgr0, 's-', markerfacecolor='none', label='DGR')
l2 = a1.plot(x, dw0, 'o-', markerfacecolor='none', label='DW')
l3 = a1.plot(x, dwe0, 'D-', markerfacecolor='none', label='DWE')
l4 = a1.plot(x, ecmp0, '^-', markerfacecolor='none', label='ECMP')
l5 = a1.plot(x, ospf0, 'v-', markerfacecolor='none', label='OSPF')

# 标题
a1.set_title('Abilene', font1)
# x轴标签
a1.set_xlabel('Sending Rate (Mbps)', font1)
# y轴标签
a1.set_ylabel('Delay (ms)', font1)
# 图例
# a1.legend()
#################################################################################
dgr1 = []
dw1 = []
dwe1 = []
ecmp1 = []
ospf1 = []

for i in range(1, 11):
    file = 'E:\\Code\\infocomm2023\\exp1\\att\\dgr-' + str(i) + 'Mbps.txt'
    dgr1.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\att\\dw-' + str(i) + 'Mbps.txt'
    dw1.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\att\\dwe-' + str(i) + 'Mbps.txt'
    dwe1.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\att\\ecmp-' + str(i) + 'Mbps.txt'
    ecmp1.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\att\\ospf-' + str(i) + 'Mbps.txt'
    ospf1.append(load_data(file))

font1 = {'family': 'Times New Roman',
         'weight': 'normal',
         'size': 12,
         }

# a2 = fig.add_subplot(142)
# 设置刻度范围
a2.set_xlim(1, 10)
a2.set_ylim(0, 350)
# 开启网格
a2.grid(True)
# 画线

m1 = a2.plot(x, dgr1, 's-', markerfacecolor='none', label='DGR')
m2 = a2.plot(x, dw1, 'o-', markerfacecolor='none', label='DW')
m3 = a2.plot(x, dwe1, 'D-', markerfacecolor='none', label='DWE')
m4 = a2.plot(x, ecmp1, '^-', markerfacecolor='none', label='ECMP')
m5 = a2.plot(x, ospf1, 'v-', markerfacecolor='none', label='OSPF')

# 标题
a2.set_title('ATT', font1)
# x轴标签
a2.set_xlabel('Sending Rate (Mbps)', font1)
# y轴标签
# a2.set_ylabel('Delay (ms)', font1)
# 图例
# a2.legend()
#################################################################################
dgr2 = []
dw2 = []
dwe2 = []
ecmp2 = []
ospf2 = []

for i in range(1, 11):
    file = 'E:\\Code\\infocomm2023\\exp1\\cernet\\dgr-' + str(i) + 'Mbps.txt'
    dgr2.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\cernet\\dw-' + str(i) + 'Mbps.txt'
    dw2.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\cernet\\dwe-' + str(i) + 'Mbps.txt'
    dwe2.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\cernet\\ecmp-' + str(i) + 'Mbps.txt'
    ecmp2.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\cernet\\ospf-' + str(i) + 'Mbps.txt'
    ospf2.append(load_data(file))

font1 = {'family': 'Times New Roman',
         'weight': 'normal',
         'size': 12,
         }

# a3 = fig.add_subplot(143)
# 设置刻度范围
a3.set_xlim(1, 10)
a3.set_ylim(0, 350)
# 开启网格
a3.grid(True)
# 画线

m1 = a3.plot(x, dgr2, 's-', markerfacecolor='none', label='DGR')
m2 = a3.plot(x, dw2, 'o-', markerfacecolor='none', label='DW')
m3 = a3.plot(x, dwe2, 'D-', markerfacecolor='none', label='DWE')
m4 = a3.plot(x, ecmp2, '^-', markerfacecolor='none', label='ECMP')
m5 = a3.plot(x, ospf2, 'v-', markerfacecolor='none', label='OSPF')

# 标题
a3.set_title('CERNET', font1)
# x轴标签
a3.set_xlabel('Sending Rate (Mbps)', font1)
# y轴标签
# a3.set_ylabel('Delay (ms)', font1)
# 图例
# a3.legend()
#################################################################################
dgr3 = []
dw3 = []
dwe3 = []
ecmp3 = []
ospf3 = []

for i in range(1, 11):
    file = 'E:\\Code\\infocomm2023\\exp1\\geant\\dgr-' + str(i) + 'Mbps.txt'
    dgr3.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\geant\\dw-' + str(i) + 'Mbps.txt'
    dw3.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\geant\\dwe-' + str(i) + 'Mbps.txt'
    dwe3.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\geant\\ecmp-' + str(i) + 'Mbps.txt'
    ecmp3.append(load_data(file))
    file = 'E:\\Code\\infocomm2023\\exp1\\geant\\ospf-' + str(i) + 'Mbps.txt'
    ospf3.append(load_data(file))

font1 = {'family': 'Times New Roman',
         'weight': 'normal',
         'size': 12,
         }

# a4 = fig.add_subplot(144)
# 设置刻度范围
a4.set_xlim(1, 10)
a4.set_ylim(0, 350)
# 开启网格
a4.grid(True)
# 画线

m1 = a4.plot(x, dgr3, 's-', markerfacecolor='none', label='DGR')
m2 = a4.plot(x, dw3, 'o-', markerfacecolor='none', label='DW')
m3 = a4.plot(x, dwe3, 'D-', markerfacecolor='none', label='DWE')
m4 = a4.plot(x, ecmp3, '^-', markerfacecolor='none', label='ECMP')
m5 = a4.plot(x, ospf3, 'v-', markerfacecolor='none', label='OSPF')

# 标题
a4.set_title('GEANT', font1)
# x轴标签
a4.set_xlabel('Sending Rate (Mbps)', font1)
# y轴标签
# a4.set_ylabel('Delay (ms)', font1)
# 图例
# a4.legend()

lines, labels = a1.get_legend_handles_labels()
fig.legend(lines, labels,loc = 'right')
# plt.savefig('C:\\Users\\ff\\Desktop\\1.eps',dpi=300)


plt.show()
