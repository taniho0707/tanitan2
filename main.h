/**
 * @file main.h
 */
#pragma once

#include "stm32f4xx.h"

#include "Led.h"
#include "Switch.h"
#include "Speaker.h"
#include "Timer.h"
#include "Encoder.h"
#include "Motor.h"
#include "MotorControl.h"
#include "SlalomParams.h"
#include "VelocityControl.h"
#include "MotorCollection.h"

#include "WallSensor.h"

#include "ComPc.h"
#include "Nfc.h"

#include "Gyro.h"
#include "Mram.h"

#include "Walldata.h"
#include "Position.h"
#include "Footmap.h"
#include "Map.h"

#include "MethodAdachi.h"

#include "Path.h"
#include "PathAdachi.h"

#include "Datalog.h"


const uint16_t GOAL_X = 1;
const uint16_t GOAL_Y = 0;

int main();

