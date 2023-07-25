#!/bin/bash
# exp 1
echo "EXP_1 empty network"
echo ""

DIR="contrib/dgrv2/infocomm2024/EXP1/EPOCH2/"
FILE="dgr-packet.delay"

cp "${DIR}code/ddr.cc" "scratch/EPOCH2_ddr.cc"

# abilene 3 --> 0
# back traffic 6 --> 7
TOPO="abilene"
SINK="0"
SENDER="3"
TCPSINK="7"
TCPSENDER="6"
BEGIN=19500
STEP=500
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH2_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
done

# att 0 --> 17
# back traffic 2 --> 11
TOPO="att"
SINK="17"
SENDER="0"
TCPSINK="11"
TCPSENDER="2"
BEGIN=5000
STEP=700
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH2_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
done

# cernet 7 --> 15
# back traffic 2 --> 21
TOPO="cernet"
SINK="15"
SENDER="7"
TCPSINK="21"
TCPSENDER="2"
BEGIN=5000
STEP=700
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH2_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
done

# geant 19 --> 8
# back traffic 5 --> 4
TOPO="geant"
SINK="8"
SENDER="19"
TCPSINK="4"
TCPSENDER="5"
BEGIN=24500
STEP=500
for i in {1..50}
do
TIME=$(($i*$STEP))
BUDGET=$(($BEGIN+$TIME))
echo "${TOPO}${DATARATE}"
./ns3 run "scratch/EPOCH2_ddr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --budget=${BUDGET} --tcpSink=${TCPSINK} --tcpSender=${TCPSENDER}"
cp $FILE "${DIR}result/${TOPO}/DDR-${i}.txt"
done
