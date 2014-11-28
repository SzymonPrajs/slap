SLAP - v0.2.3
=============

Supernova Lightcurve Analysis Package


Install
-------
In the testing phase only a local installation is possible. 

```
make
```


Usage
-----
The interface is still under construction. For the purpose of testing use the following:

```
./slap fit LC=data/test/06D4eu.dat z=1.588 model=BB4
```

Future releases will introduce a full documentation.

Models
------
BB4 - dynamic BB model parametrised by dR/dt, T0, dT/dt and MJD_explosion 

BB6 - dynamic BB model parametrised by dR/dt, d2R/dt2, T0, dT/dt, d2T/dt2 and MJD_explosion 

Magnetar - Magnetar model. Parameters: Diffusion timescale - tau_m (days), Magnetic field - B (1e14 G), Spin period - P (ms) , Initial Radius - R0 (1e14cm) and explosion MJD 

Releases
--------
**v0.2.3**: Add more filter responces.

**v0.2.2**: Introducing a plotting routine implemented in Matplotlib. Some small bug fixes.

**v0.2.1**: Many bug fixes. Laying ground for the plotting function.

**v0.2.0**: Major revamp to the interface. Still experimental.

**v0.1.2**: Added the Magnetar model.

**v0.1.1**: Minor performance and functionality enhancements.

**v0.1.0**: Initial working version of the code.

Future
------
In the near future the MPFIT will be replaced by Minuit2 as the fitting routine used. Boost and GSL will be used to replace all routines 
implemented by me that find their, better, alternatives in these libraries. This is mainly for integrations, interpolations and filesystem. 

Licence
-------
SLAP - Supernova Lightcurve Analysis Package v0.2.3

Copyright (C) 2014  Szymon Prajs

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
