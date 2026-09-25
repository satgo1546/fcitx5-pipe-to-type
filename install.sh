#!/bin/bash
set -ve
g++ -shared -fPIC -std=c++20 -Wall -O2 $(pkg-config --cflags --libs Fcitx5Core Fcitx5Utils) -o fcitx5-pipe-to-type.so fcitx5-pipe-to-type.cpp
mkdir -p "$HOME/.local/share/fcitx5/addon"
mv fcitx5-pipe-to-type.so "$HOME/.local/share/fcitx5/addon"
cp fcitx5-pipe-to-type.conf "$HOME/.local/share/fcitx5/addon"
echo "Library=$HOME/.local/share/fcitx5/addon/fcitx5-pipe-to-type" >> "$HOME/.local/share/fcitx5/addon/fcitx5-pipe-to-type.conf"
killall fcitx5 && fcitx5 -d > /dev/null 2>&1
