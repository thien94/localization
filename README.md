# localization

*Ongoing development*

Updated version of the [`localization`](https://github.com/sair-lab/localization.git) - A localization ROS package for range sensors, i.e. ultra wide-band (UWB). New functionalities include:
- [x] Compilable with Ubuntu 20.04
- [x] Run examples for [MCD-VIRAL dataset](https://mcdviral.github.io/)
- [ ] Run examples for [NTU-VIRAL dataset](https://ntu-aris.github.io/ntu_viral_dataset/)

This repo also provides sensor fusion for multi-source, e.g. Optical Flow and Visual SLAM.

# Preparation
    System higher than Ubuntu 16.04 (with ROS).

Install dependencies:

    sudo apt-get update

    sudo apt install python-pip

    sudo pip install --user numpy scipy matplotlib ipython jupyter pandas sympy nose
    
    sudo easy_install -U statsmodels

    sudo apt install libcholmod3.0.6 libcsparse3.1.4 libsuitesparse-dev python-cvxopt 
    
    sudo apt install ros-kinetic-hector-trajectory-server
    
Install G2O from source. Download g2o to your non-ros source folder, make and install:
  
    git clone https://github.com/RainerKuemmerle/g2o

Please checkout the following commit for compatibility.

    cd g2o ; git checkout deafc01ee8315b9405351fb145238c5d62f82dc7
    mkdir build ; cd build
    cmake .. ; make -j4
    sudo make install
 
Download uwb_driver to your ros workspace and install its dependencies:

    git clone https://github.com/wang-chen/uwb_driver.git
    sudo apt-get install ros-kinetic-serial

New support for UWB from bitcrazy:

    please find its driver on:
    https://github.com/wang-chen/lps-ros
    
To use UWB from bitcrazy / linktrackP, comment the following lines in the CMakeLists.txt

    add_definitions(-DLINK_TRACKP)
    add_definitions(-DTIME_DOMAIN)
    
# Usage

## 1. The following ROS param should be set before the program is started

    /uwb/nodesId: an array, the IDs of the UWB anchors (the last ID is the moving module to be localized)
    /uwb/nodesPos: an array, the postions (x, y, z) of the UWB anchors (the last three elements, is the initial postion of the moving module to be localized)
    /uwb/antennaOffset: the antenna offset on the moving module, there may be multiple antennas. DONOT SET IT IF THERE IS NO OFFSET  
    
    One example can be found in the uwb_driver repo:
    
    https://github.com/wang-chen/uwb_driver/blob/master/cfg/anchor.yaml

## 2. Program Parameters
    It can be set by modifying the yaml files in the cfg folder.
    Include the specific yaml file in your launch file. 
    For example, the "uwb_only.yaml" sets parameters for uwb-only localization.
    
## 3. Topic subscriber
    This localizaiton repo subsribes the specific sensor measurement topic.
    Make sure they are published before the program is started.
    The topic name can also be changed in the aformentioned yaml files.
    
# If you are interested in this work, you may cite:

    @article{fang2020graph,
      title={Graph Optimization Approach to Range-based Localization},
      author={Fang, Xu and Wang, Chen and Nguyen, Thien-Minh and Xie, Lihua},
      journal={IEEE Transactions on Systems, Man and Cybernetics: Systems},
      year={2020}
    }
    
    @inproceedings{fang2018model,
      title={Model-free approach for sensor network localization with noisy distance measurement},
      author={Fang, Xu and Wang, Chen and Nguyen, Thien-Minh and Xie, Lihua},
      booktitle={2018 15th International Conference on Control, Automation, Robotics and Vision (ICARCV)},
      pages={1973--1978},
      year={2018},
      organization={IEEE}
    }
    
    @inproceedings{wang2017ultra,
      title={Ultra-Wideband Aided Fast Localization and Mapping System},
      author={Wang, Chen and Zhang, Handuo and Nguyen, Thien-Minh and Xie, Lihua},
      booktitle={IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS)},
      pages={1602--1609},
      year={2017},
      organization={IEEE}
    }
