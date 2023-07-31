#!/bin/bash
# exp 2
echo "EXP_2 TCP com"
echo ""

DIR="contrib/dgrv2/infocomm2024/EXP2/tcp/"
FILE="exp2-results/throughput.txt"

cp "${DIR}code/tcp-variance.cc" "scratch/EXP2-tcp-variance.cc"

# abilene 0 --> 10

TCP_PROT="TcpBbr"
./ns3 run "scratch/EXP2-tcp-variance.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/DDR-${TCP_PROT}-throughput.txt"

TCP_PROT="TcpVegas"
./ns3 run "scratch/EXP2-tcp-variance.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/DDR-${TCP_PROT}-throughput.txt"

TCP_PROT="TcpCubic"
./ns3 run "scratch/EXP2-tcp-variance.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/DDR-${TCP_PROT}-throughput.txt"


TCP_PROT="TcpDctcp"
./ns3 run "scratch/EXP2-tcp-variance.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/DDR-${TCP_PROT}-throughput.txt"


# --------------------- OSPF ---------------------
DIR="contrib/dgrv2/infocomm2024/EXP2/tcp/"
FILE="exp2-results/throughput.txt"

cp "${DIR}code/ospf-tcp.cc" "scratch/ospf-tcp.cc"

# abilene 0 --> 10

TCP_PROT="TcpBbr"
./ns3 run "scratch/ospf-tcp.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/OSPF-${TCP_PROT}-throughput.txt"

TCP_PROT="TcpVegas"
./ns3 run "scratch/ospf-tcp.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/OSPF-${TCP_PROT}-throughput.txt"

TCP_PROT="TcpCubic"
./ns3 run "scratch/ospf-tcp.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/OSPF-${TCP_PROT}-throughput.txt"


TCP_PROT="TcpDctcp"
./ns3 run "scratch/ospf-tcp.cc --transport_prot=${TCP_PROT}"
cp $FILE "${DIR}result/OSPF-${TCP_PROT}-throughput.txt"

