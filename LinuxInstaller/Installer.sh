#!/bin/sh

sudo apt install -y ./packages/libglew1.13_1.13.0-2_amd64.deb
sudo apt install -y ./packages/libsdl2-2.0-0_2.0.2+dfsg1-3ubuntu1_amd64.deb
sudo apt install -y ./packages/libsdl2-mixer-2.0-0_2.0.0+dfsg1-3_amd64.deb
sudo apt install -y ./packages/TowerPackage.deb

cd /usr/local/bin/Tower
./tower
