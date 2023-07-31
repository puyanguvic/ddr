#!/bin/bash
# exp 1
echo "EXP_2 empty network"
echo ""

DIR="contrib/dgrv2/infocomm2024/EXP2/cpu_time/"
FILE="dgr-packet.delay"

cp "${DIR}ospf.cc" "scratch/cpu_time_ospf.cc"

# OSPF
TOPO="abilene"
./ns3 run "scratch/cpu_time_ospf.cc --topo=${TOPO}"

TOPO="att"
./ns3 run "scratch/cpu_time_ospf.cc --topo=${TOPO}"

TOPO="cernet"
./ns3 run "scratch/cpu_time_ospf.cc --topo=${TOPO}"
TOPO="geant"
./ns3 run "scratch/cpu_time_ospf.cc --topo=${TOPO}"

# DDR
cp "${DIR}ddr.cc" "scratch/cpu_time_ddr.cc"
TOPO="abilene"
./ns3 run "scratch/cpu_time_ddr.cc --topo=${TOPO}"

TOPO="att"
./ns3 run "scratch/cpu_time_ddr.cc --topo=${TOPO}"

TOPO="cernet"
./ns3 run "scratch/cpu_time_ddr.cc --topo=${TOPO}"
TOPO="geant"
./ns3 run "scratch/cpu_time_ddr.cc --topo=${TOPO}"