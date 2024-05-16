#!/bin/bash
# exp 1
echo "EXP_1 empty network"
echo ""

DIR="contrib/ddr/infocomm2024/EXP1/EPOCH3/"
FILE="dgr-packet.delay"

cp "${DIR}code/kshort.cc" "scratch/EPOCH3_kshort.cc"

# abilene 0 --> 10
# back traffic 2 --> 5
TOPO="abilene"
SINK="10"
SENDER="0"
TCPSINK="5"
TCPSENDER="2"
BEGIN=19500
STEP=500
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH3_kshort.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/KSHORT-${i}.txt"
done

# att 0 --> 17
# back traffic 2 --> 9
TOPO="att"
SINK="17"
SENDER="0"
TCPSINK="9"
TCPSENDER="2"
BEGIN=20000
STEP=700
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH3_kshort.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/KSHORT-${i}.txt"
done

# cernet 7 --> 15
# back traffic 2 --> 14
TOPO="cernet"
SINK="15"
SENDER="7"
TCPSINK="14"
TCPSENDER="2"
BEGIN=5000
STEP=700
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH3_kshort.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/KSHORT-${i}.txt"
done

# geant 14 --> 16
# back traffic 3 --> 0
TOPO="geant"
SINK="16"
SENDER="14"
TCPSINK="0"
TCPSENDER="3"
BEGIN=20000
STEP=800
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH3_kshort.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/KSHORT-${i}.txt"
done