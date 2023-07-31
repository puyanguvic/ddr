#!/bin/bash
# exp 1
echo "EXP_1 empty network"
echo ""

DIR="contrib/dgrv2/infocomm2024/EXP1/routing/"
FILE="dgr-packet.delay"

cp "${DIR}code/ospf.cc" "scratch/routing_ospf.cc"

# abilene 0 --> 10
TOPO="abilene"
SINK="10"
SENDER="0"
BEGIN=19500
STEP=500
for i in {1..1}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/routing_ospf.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
cp $FILE "${DIR}result/${TOPO}/OSPF-${i}.txt"
done

# att 0 --> 17
TOPO="att"
SINK="17"
SENDER="0"
BEGIN=20000
STEP=700
for i in {1..1}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/routing_ospf.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
cp $FILE "${DIR}result/${TOPO}/OSPF-${i}.txt"
done

# cernet 7 --> 15
TOPO="cernet"
SINK="15"
SENDER="7"
BEGIN=5000
STEP=700
for i in {1..1}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/routing_ospf.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
cp $FILE "${DIR}result/${TOPO}/OSPF-${i}.txt"
done

# geant 14 --> 16
TOPO="geant"
SINK="16"
SENDER="14"
BEGIN=20000
STEP=800
for i in {1..1}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/routing_ospf.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET}"
cp $FILE "${DIR}result/${TOPO}/OSPF-${i}.txt"
done