#!/bin/bash
# exp 1
echo "experiment 1"
echo ""

DIR="contrib/dgr/infocomm2023/exp1/"
FILE="dgr-packet.delay"

# abilene 0 --> 10
TOPO="abilene"
SINK="10"
SENDER="0"
for i in {1..10}
do
DATARATE="${i}Mbps"
echo "${TOPO}${DATARATE}"
./waf --run "scratch/dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/dgr-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=true --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ecmp-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=false --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ospf-${DATARATE}.txt"
done

# att 20 --> 14
TOPO="att"
SINK="14"
SENDER="20"
for i in {1..10}
do
DATARATE="${i}Mbps"
echo "${TOPO}${DATARATE}"
./waf --run "scratch/dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/dgr-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=true --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ecmp-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=false --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ospf-${DATARATE}.txt"
done

# cernet 7 --> 13
TOPO="cernet"
SINK="13"
SENDER="7"
for i in {1..10}
do
DATARATE="${i}Mbps"
echo "${TOPO}${DATARATE}"
./waf --run "scratch/dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/dgr-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=true --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ecmp-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=false --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ospf-${DATARATE}.txt"
done

# geant 16 --> 14
TOPO="geant"
SINK="14"
SENDER="16"
for i in {1..10}
do
DATARATE="${i}Mbps"
echo "${TOPO}${DATARATE}"
./waf --run "scratch/dgr.cc --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/dgr-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=true --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ecmp-${DATARATE}.txt"
./waf --run "scratch/ospf.cc --ns3::Ipv4GlobalRouting::RandomEcmpRouting=false --topo=${TOPO} --sink=${SINK} --sender=${SENDER} --dataRate=${DATARATE}"
cp $FILE "${DIR}${TOPO}/ospf-${DATARATE}.txt"
done
