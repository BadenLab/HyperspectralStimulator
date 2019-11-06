# Hyperspectral Stimulator

_Philipp Bartel & Filip Janiak for BadenLab_

Hardware description and software (Arduino and Python) for the hyperspectral stimulator (a.k.a. Skittles) adapted from:

Belušič, G., Ilić, M., Meglič, A., & Pirih, P. (2016). _A fast multispectral light synthesiser based on LEDs and a diffraction grating._ Scientific reports, 6, 32012.

The stimulator goes from boring...

<img src="/Images/OFF.jpg" width="256">

to Presto!

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

# Hardware

Lightguide:
https://www.newport.com/p/77555
The lightguide used has the following transmission characteristics:

<img src="https://www.newport.com/mam/celum/celum_assets/LS-254b_600w.gif?6" width="512">

To allow UV light to pass through the stimulation setup a liquid lightguide is preferred due to its high transmissivity in the short wavelength (UV) domain.

LEDs:
Full list used lies in the respective folder. The LEDs were ordered from Roithner Lasertechnik (http://www.roithner-laser.com/).

Optical rails - to hold the LEDs.

Microcontroller:
Arduino Due - perhaps best replaced with an EPS Feather. The Arduino code was only tested with the 1st edition of the Arduino Due.

LED driver:
TLC5947 Adafruit 24-Channel 12-bit PWM LED Driver (https://www.adafruit.com/product/1429)

Potentiometers & Resistors as required.

3D-prints

# Centre-Surround Stimulation, Stimulus Movement

It is possible to extend the construction to achieve moving hyperspectral centre-surround stimulation.

<img src="/Images/CSPrototype.jpg" width="256">

The idea behind centre-surround is as follows:
1. Modulate the stimulation spectrum with a high frequency
2. Add an ETL (Electric Focus-Tunable Lens) into the post-fibre lightpath
3. Couple the two to achieve concurrent spot size to spectrum changes

The Arduino software already allows you to do that.

Movement of the stimulus is achieved via a system of spectrally flat mirrors, driven with servomotors.
1. Add mirrors after the ETL
2. Done

The Arduino software already allows you to move the stimulus.

# Hardware required for Centre-Surround and movement improvements:

SparkFun Joystick Shield Kit: https://www.sparkfun.com/products/9760

Mirrors used: UV enhanced Thorlabs mirrors
https://www.thorlabs.com/thorproduct.cfm?partnumber=PFSQ05-03-F01
https://www.thorlabs.com/thorproduct.cfm?partnumber=PFSQ10-03-F01

To hold the optics we used:
SWC/M - Rotating Clamp for Ø1/2" Posts, 360° Continuously Adjustable, 5 mm Hex 
https://www.thorlabs.com/thorproduct.cfm?partnumber=SWC/M
