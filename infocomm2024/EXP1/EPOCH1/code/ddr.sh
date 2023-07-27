#!/bin/bash
# exp 1
echo "EXP_1 empty network"
echo ""

DIR="contrib/dgrv2/infocomm2024/EXP1/EPOCH1/"
FILE="dgr-packet.delay"

cp "${DIR}code/ddr.cc" "scratch/EPOCH1_ddr.cc"

# # abilene 0 --> 10
# TOPO="abilene"
# SINK="10"
# SENDER="0"
# BEGIN=20000
# STEP=500
# for i in {1..50}
# do
# TIME=$(($i*$STEP))
# BUDGET=$(($BEGIN+$TIME))
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/EPOCH1_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
# cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
# done

# # att 20 --> 14
# TOPO="att"
# SINK="14"
# SENDER="20"
# BEGIN=5000
# STEP=700
# for i in {1..50}
# do
# TIME=$(($i*$STEP))
# BUDGET=$(($BEGIN+$TIME))
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/EPOCH1_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
# cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
# done

# # cernet 7 --> 13
# TOPO="cernet"
# SINK="13"
# SENDER="7"
# BEGIN=5000
# STEP=600
# for i in {1..50}
# do
# TIME=$(($i*$STEP))
# BUDGET=$(($BEGIN+$TIME))
# echo "${TOPO}${DATARATE}"
# ./ns3 run "scratch/EPOCH1_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
# cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
# done

# geant 14 --> 16
TOPO="geant"
SINK="16"
SENDER="14"
BEGIN=24500
STEP=700
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH1_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
done