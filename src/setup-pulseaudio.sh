#!/bin/bash


# enable dbus

mkdir -p /var/run/dbus

dbus-uuidgen > /var/lib/dbus/machine-id

dbus-daemon --config-file=/usr/share/dbus-1/system.conf --print-address


# add root

 adduser root pulse-access

 adduser root audio


# Cleanup to be "stateless" on startup, otherwise pulseaudio daemon can't start

rm -rf /var/run/pulse /var/lib/pulse /root/.config/pulse

cp /etc/pulse/* ~/.config/pulse/


pulseaudio -D --exit-idle-time=-1


# Create a virtual speaker output

 pactl load-module module-null-sink sink_name=SpeakerOutput



pactl set-default-sink SpeakerOutput

pactl set-default-source SpeakerOutput.monitor


mkdir ~/.config

echo -e "[General]\nsystem.audio.type=default" > ~/.config/zoomus.conf

