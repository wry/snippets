#!/bin/sh

K_POWER=116
K_CHANNEL_UP=402
K_CHANNEL_DOWN=403
K_VOLUME_DOWN=114
K_VOLUME_UP=115

DEV=$1
CMD=$2

function main()
{
  if [ -z $DEV ] || [ -z $CMD ]; then
    echo "$0 <device> <cmd>"
    exit 1
  fi

  if [ "$CMD" == "power" ]; then
    power
  elif [ "$CMD" == "chanup" ]; then
    chanup
  elif [ "$CMD" == "chandown" ]; then
    chandown
  elif [ "$CMD" == "volup" ]; then
    volup
  elif [ "$CMD" == "voldown" ]; then
    voldown
  elif [ "$CMD" == "allops" ]; then
    for i in $(seq 1 400); do
      echo sending keycode $i
      sleep 1
      input_key_press $i
    done
  fi
}

function input_event()
{
  local EV=$1
  local KEY=$2
  local VAL=$3

  sendevent $DEV $EV $KEY $VAL
}

function input_key_event()
{
  local KEY=$1
  local VAL=$2

  input_event 1 $KEY $VAL
  input_sync
}

function input_key_press()
{
  local KEY=$1

  input_key_event $KEY 1
  input_key_event $KEY 0
}

function input_sync()
{
  input_event 0 0 0
}

function power()
{
  input_key_press $K_POWER
}

function chanup()
{
  input_key_press $K_CHANNEL_UP
}

function volup()
{
  input_key_press $K_VOLUME_UP
}

function voldown()
{
  input_key_press $K_VOLUME_DOWN
}

function cec_mute()
{
  # 765 4050 
  # 17475 17220
  # 17217 16707

  KEY=765 # 0x02 0xfd cec control?
  VAL=17475 # 0x44 remote control / 0x43 mute
  VAL=17217 # 0x44 remote control / 0x41 volup

  KEY=4050 # 0x02 0xfd cec control?
  VAL=17220 # 0x44 remote control / 0x43 mute
  VAL=16707 # 0x44 remote control / 0x41 volup

  sendevent $DEV 1 $KEY $VAL
  sendevent $DEV 0 0 0
  sendevent $DEV 1 $KEY $VAL
  sendevent $DEV 0 0 0
}

main

