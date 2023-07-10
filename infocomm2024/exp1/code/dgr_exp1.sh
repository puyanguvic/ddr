#!/bin/bash
# exp 1
echo "experiment 1"
echo ""

DIR="contrib/dgrv2/infocomm2024/exp1/"
FILE="dgr-packet.delay"

# abilene 0 --> 10
TOPO="abilene"
SINK="10"
SENDER="0"
for i in {1..10}
do
DATARATE="${i}Mbps"
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/exp1_dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/dgrv2-${DATARATE}.txt"
done

# # att 20 --> 14
# TOPO="att"
# SINK="14"
# SENDER="20"
# for i in {1..10}
# do
# DATARATE="${i}Mbps"
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/exp1_dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
# cp $FILE "${DIR}${TOPO}/dgrv2-${DATARATE}.txt"
# done

# # cernet 7 --> 13
# TOPO="cernet"
# SINK="13"
# SENDER="7"
# for i in {1..10}
# do
# DATARATE="${i}Mbps"
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/exp1_dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
# cp $FILE "${DIR}${TOPO}/dgrv2-${DATARATE}.txt"
# done

# # geant 16 --> 14
# TOPO="geant"
# SINK="14"
# SENDER="16"
# for i in {1..10}
# do
# DATARATE="${i}Mbps"
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/exp1_dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
# cp $FILE "${DIR}${TOPO}/dgrv2-${DATARATE}.txt"
# done
