# DDR: A Deadline-Driven Routing Protocol

This repository contains the implementation of DDR, a Deadline-Driven Routing Protocol designed to provide delay-guaranteed service in networks. This work was presented at IEEE INFOCOM 2024.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## Introduction

DDR (Deadline-Driven Routing) is a distributed traffic-aware adaptive routing protocol that provides delay-guaranteed service. Time-sensitive applications have become increasingly prevalent in modern networks, necessitating the development of Delay-Guaranteed Routing (DGR) solutions. Finding an optimal DGR solution remains a challenging task due to the NP-hard nature of the problem and the dynamic nature of network traffic. Inspired by online navigation techniques, DDR leverages real-time traffic conditions to optimize routing decisions and ensure on-time packet delivery. By combining network topology-based path generation with real-time traffic knowledge, each router can adjust packet forwarding directions to meet its heterogeneous latency requirements.

Comprehensive simulations on real-world network topologies demonstrate that DDR can consistently provide delay-guaranteed service in different network topologies with varying traffic conditions. In addition, DDR ensures backward compatibility with legacy devices and existing routing protocols, making it a viable solution for supporting delay-guaranteed service.

DDR is implemented based on ns-3.38. Running this code requires cloning it into the ns-3 `contrib/` folder.

## Features

- Deadline-aware packet scheduling
- Adaptive routing based on network conditions
- Support for various network topologies
- Performance evaluation tools

## Installation

To install the necessary dependencies and set up the environment for DDR, follow these steps:

1. Clone the ns-3.38 repository and navigate to the `contrib/` directory:
    ```bash
    git clone https://gitlab.com/nsnam/ns-3-dev.git ns-3.38
    cd ns-3.38/contrib
    ```

2. Clone the DDR repository into the `contrib/` directory:
    ```bash
    git clone https://github.com/yourusername/DDR.git
    ```

3. Build ns-3 with the DDR module:
    ```bash
    cd ..
    ./ns3 configure --enable-examples
    ./ns3 build
    ```

## Usage

### Running the Simulation

To run a demo of DDR:

```bash
cp contrib/ddr/examples/ddr-demo.cc scratch/ddr-demo.cc
./ns3 --run "scratch/ddr-demo.cc"
```

To reproduce the experimental results:
```bash
cd ns-3.38
bash contrib/ddr/infocomm2024/EXP1/EPOCH1/code/exp1-1.sh
bash contrib/ddr/infocomm2024/EXP1/EPOCH2/code/exp1-2.sh
bash contrib/ddr/infocomm2024/EXP1/EPOCH3/code/exp1-3.sh
```

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments
We would like to thank the contributors and the research community for their valuable feedback and support. Special thanks to the IEEE INFOCOM 2024 conference for providing a platform to present our work.
