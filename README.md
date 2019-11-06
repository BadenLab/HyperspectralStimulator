# HyperspectralStimulator
Hardware description and software (Arduino and Python) for the hyperspectral stimulator (a.k.a. Skittles) adapted from:

Belušič, G., Ilić, M., Meglič, A., & Pirih, P. (2016). A fast multispectral light synthesiser based on LEDs and a diffraction grating. Scientific reports, 6, 32012.

Boring...

<img src="/Images/OFF.jpg" width="256">

Presto!

<img src="/Images/ON.jpg" width="256">

# Arduino Code

The goal of these Arduino scripts is to enable Belusic et al's Light Synthesiser to function correctly with a 2-Photon imaging setup. The following is achieved:
1. Full-field light stimulation concurrent with 2-p imaging.
2. Laser - synthesiser synchronisation. Stimulus is ON while the photomultipliers are OFF and vice versa.
3. Complex and long stimulus routines with 19 LEDs with easy serial port control.

How does it work?
1. Arduino Due is paired with an adafruit 24-channel PWM driver
2. The control board driving the laser gives a "Blanking" signal, which is split. This serves two purposes:
  a. Blanking signal serves as an internal clock for stimulus and trigger timing
  b. Blanking signal connected to the /oe pin on the Adafruit pwm driver disables any LED output with high temporal precision.
3. An additional Arduino board serves as a blanking pulse 'counter' board. E.g.: the blanking signal is HIGH every 2ms. The 'counter' Arduino counts the pulses and sends a pulse to Due for every five incoming pulses (i.e. every 10ms).
4. This 'counter' signal comes into an Arduino /interrupt/ pin. When the signal comes through, LED pwm values are updated.
5. The user provides a matrix of the form [number of LEDs x stimulus frames], as well as a vector of [frame durations in n('counter' pulses)].
6. The setup also produces a 'Trigger' signal. Since it is synchronised with the stimulus procession, it is great for aligning stimulus to the recording trace
