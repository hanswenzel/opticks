#!/usr/bin/env python
#
# Copyright (c) 2019 Opticks Team. All Rights Reserved.
#
# This file is part of Opticks
# (see https://bitbucket.org/simoncblyth/opticks).
#
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License.  
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
# See the License for the specific language governing permissions and 
# limitations under the License.
#

"""
::

    In [13]: evt.rpost_(slice(0,5))
    Out[13]: 
    A()sliced
    A([[[     -65.844,   -7.065, -299.997,    0.1  ],
            [ -65.844,   -7.065,  -99.994,    0.767],
            [ -65.844,   -7.065,   99.994,    1.905],
            [ -65.844,   -7.065,  500.   ,    3.239],
            [   0.   ,    0.   ,    0.   ,    0.   ]],

           [[ -33.967,   58.733, -299.997,    0.1  ],
            [ -33.967,   58.733,  -99.994,    0.767],
            [ -33.967,   58.733,   99.994,    1.889],
            [ -33.967,   58.733,  500.   ,    3.223],
            [   0.   ,    0.   ,    0.   ,    0.   ]],

           [[  20.02 ,   45.839, -299.997,    0.1  ],
            [  20.02 ,   45.839,  -99.994,    0.767],
            [  20.02 ,   45.839,   99.994,    1.91 ],
            [  20.02 ,   45.839,  500.   ,    3.244],
            [   0.   ,    0.   ,    0.   ,    0.   ]],

           ..., 
           [[ -51.408,   -2.457, -299.997,    0.1  ],
            [ -51.408,   -2.457,  -99.994,    0.767],
            [ -51.408,   -2.457,   99.994,    1.921],
            [ -51.408,   -2.457,  500.   ,    3.255],
            [   0.   ,    0.   ,    0.   ,    0.   ]],

           [[  35.829,   11.765, -299.997,    0.1  ],
            [  35.829,   11.765,  -99.994,    0.767],
            [  35.829,   11.765,   99.994,    1.863],
            [  35.829,   11.765,  500.   ,    3.197],
            [   0.   ,    0.   ,    0.   ,    0.   ]],

           [[ -44.527,   76.846, -299.997,    0.1  ],
            [ -44.527,   76.846,  -99.994,    0.767],
            [ -44.527,   76.846,   99.994,    1.911],
            [ -44.527,   76.846,  500.   ,    3.245],
            [   0.   ,    0.   ,    0.   ,    0.   ]]])

    In [14]: evt.rpost_(slice(0,5)).shape
    Out[14]: (500000, 5, 4)





"""
import os, sys, logging, numpy as np
log = logging.getLogger(__name__)

from opticks.ana.base import opticks_main
from opticks.ana.evt import Evt

if __name__ == '__main__':
    args = opticks_main(tag="1",src="torch", det="lens", doc=__doc__)
    np.set_printoptions(suppress=True, precision=3)

    evt = Evt(tag=args.tag, src=args.src, det=args.det, seqs=[], args=args)

    log.debug("evt") 
    print evt

    log.debug("evt.history_table") 
    evt.history_table(slice(0,20))
    log.debug("evt.history_table DONE") 
       

