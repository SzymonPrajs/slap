SLAP - v0.8.0
=============

Supernova Lightcurve Analysis Package


Install
-------
The code is currently set up in a way which requires path modification on other than my machines but with the v0.9.0 release this will be fixed.
The installation procedure will be:
```
./configure --prefix=/path/to/install --CXX=compiler_to_use
make
sudo make install
```
Default path will be set as /usr/local which will require the use of ```sudo```.

Usage
-----
Full documentation for SLAP will be available soon. For the purpose of testing, the following can be used:

```
slap analyse LC=data/test/06D4eu.dat z=1.588 model=Magnetar abs=06D4eu
```
Running SLAP with only parameters and no command will enter an interactive shell. The uses of this will be documented shortly. 

Models
------
BB4 - dynamic BB model: dR/dt, T0, dT/dt and explosion_MJD

BB6 - dynamic BB model: dR/dt, d2R/dt2, T0, dT/dt, d2T/dt2 and explosion_MJD 

Magnetar - Magnetar model: tau_m (days), B (1e14 G), P (ms), explosion_MJD 

MagnetarR - Magnetar model + Initial radius: tau_m (days), B (1e14 G), P (ms), R0 (1e14 cm), explosion_MJD 

Releases
--------
**v0.8.0**: Huge performance exhancement by redesigning how the SEDs are treated thoughout the code. Fixing a lot of bugs. Dropped the dependence on Boost.

**v0.3.0**: Introducing MultiNest fitting. Improving the Magnetar model (Wang et. al), introducing spectral corrections and Analyse.

**v0.2.2**: Introducing a plotting routine implemented in Matplotlib. Some small bug fixes.

**v0.2.0**: Major revamp to the interface. Still experimental.

**v0.1.2**: Added the Magnetar model.

**v0.1.0**: Initial working version of the code.

Licence
-------
SLAP - Supernova Lightcurve Analysis Package v0.8.0

Copyright (C) 2014-2015  Szymon Prajs

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
