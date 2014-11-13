SLAP - v0.1.1
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
./slap data/test/06D4eu.dat 1.588 BB4
```

Replace the arguments with your LC file, redshift and model of choice respectively.


Models
------
BB4 - dynamic BB model parametrised by dR/dt, T0, dT/dt and MJD_explosion 

BB6 - dynamic BB model parametrised by dR/dt, d2R/dt2, T0, dT/dt, d2T/dt2 and MJD_explosion 


Releases
--------
**v0.1.1**: Minor performance and functionality enhancements.

**v0.1.0**: Initial working version of the code.


Licence
-------
SLAP - Supernova Lightcurve Analysis Package v0.1.1

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
