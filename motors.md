Output power (Watts) = Torque (Nm) * Speed (rad/s)
Input power (Watts) = Voltage (V) * Current (A)
Motor Efficiency = Output power / Input power

## Gobilda
Watts 29
Speed 
Volts
Apms

## Gobilda
Watts
Speed
Volts
Apms

torque 1.0 nm
speed 30 rpm
     
x = desired rpm * motor rpm


Kv stands for velocity konstant
Kt stands for torque konstant
Km stands for motor konstant, aka the torque efficiency of the motor

Kv is measure of back emf. rpm spun for 1 volt output
Kv is *related* to windings
few large turns is high Kv
many small turns is low Kv
decrease Kv, increase Kt. Kt = 1/Kv
increased Kv, increased resistance
torque stays the same with different Kv

continuous maximums
*mav voltage*
*max current*

make sure the units are right when calculating the Kt

Theoretical torque produced = current(A) * Kt 

Kv != torque produced

1 Nm * 1 rad / s = 1 Watt

esc software:
Am32
blheli32

talk to jeremy

finalVelocity^2 = initialVelocity^2 + 2 \* acceleration \* displacement
finalVelocity^2 - initialVelocity^2 = (2 \* acceleration \* displacement)
(finalVelocity^2 - initialVelocity^2) / (2 \* displacement) = acceleration
acceleration = (finalVelocity^2 - initialVelocity^2) / (2 \* displacement)

finalVelocity = 5ft/s
initialVelocity = 0ft/s
displacement = 1ft
acceleration = (5ft/s^2 - 0ft/s^2) / (2 \* 1ft)
acceleration = 5ft/s^2 / (2 \* 1ft)
acceleration = 5ft/s^2 / 2ft
acceleration = 25ft/s / 2ft
acceleration = 12.5ft/s^2

force to accelerate robot
f= ma
f = l0lbs \* 12.5ft/s^2
f = 125 (lbs*ft)/s^2

f = 3.885 lbs
f = 17.281875 Newtons

Tau = F*r*sin[theta]
diameter = 72mm
radius = 36mm
radius = 0.11811 ft
torque = force * radius * sin(90)
torque = 3.885 lbs * 0.11811 ft * sin(90)
torque = 3.885 lbs * 0.11811 ft * 1
torque = 3.885 lbs * 0.11811 ft
torque = 3.885 lbs * 0.11811 ft
torque = 0.45885735 ft-lbs

