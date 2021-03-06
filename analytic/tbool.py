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

import logging, os
log = logging.getLogger(__name__)

template_head = r"""
join(){ local IFS="$1"; shift; echo "$*"; }

tbool%(name)s-vi(){   vi $BASH_SOURCE ; }
tbool%(name)s-env(){  olocal- ;  } 
tbool%(name)s-tag(){  echo 1 ; }
tbool%(name)s-det(){  echo tbool ; }
tbool%(name)s-src(){  echo torch ; }
tbool%(name)s-args(){ echo  --det $(tbool%(name)s-det) --src $(tbool%(name)s-src) ; }

tbool%(name)s-torchconfig()
{
    local identity=1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,1.000

    #local photons=1000000
    local photons=100000
    #local photons=1

    local torch_config_sphere=(
                 type=sphere
                 photons=10000
                 frame=-1
                 transform=1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,1000.000,1.000
                 source=0,0,0
                 target=0,0,1
                 time=0.1
                 radius=100
                 distance=400
                 zenithazimuth=0,1,0,1
                 material=GdDopedLS
                 wavelength=500
               )

    echo "$(join _ ${torch_config_sphere[@]})" 
}


tbool%(name)s-op()
{
    local msg="=== $FUNCNAME :"
    local cmdline=$*
    local testconfig=$TESTCONFIG

    op.sh  \
            $cmdline \
            --animtimemax 20 \
            --timemax 20 \
            --geocenter \
            --eye 1,0,0 \
            --dbganalytic \
            --test --testconfig "$testconfig" \
            --torch --torchconfig "$(tbool%(name)s-torchconfig)" \
            --torchdbg \
            --tag $(tbool%(name)s-tag) --cat $(tbool%(name)s-det) \
            --rendermode +global \
            --save 
}

tbool%(name)s(){ TESTCONFIG=$($FUNCNAME-) tbool%(name)s-op $* ; }
tbool%(name)s-(){ $FUNCNAME- | python $* ; } 
tbool%(name)s--(){ cat << EOP

import logging
import numpy as np
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
from opticks.analytic.sc import Sc

args = opticks_main(csgpath="$TMP/tbool/%(name)s")

CSG.boundary = args.testobject
CSG.kwa = dict(verbosity="0", poly="IM", resolution="20")
#CSG.kwa = dict(verbosity="0", poly="HY", level="5")
"""

template_tail = r"""
raw = %(root)s

raw.dump("raw")

maxcsgheight = 4
maxcsgheight2 = 5
obj = Sc.optimize_csg(raw, maxcsgheight, maxcsgheight2 ) 

obj.dump("optimized")

#uobjs = [raw]
uobjs = [obj]

con = CSG("box",  param=[0,0,0,10], container="1", containerscale="2", boundary=args.container , poly="IM", resolution="20" )
CSG.Serialize([con]+uobjs, args  )

EOP
}
"""

template_body = r"""
%(body)s
"""


test_body = r"""

a = CSG("sphere", param=[0,0,0,500] )


"""

from opticks.ana.base import now_


class TBoolBashFunction(dict):
     """
     This is used from opticks.analytic.csg with the "body" geometry 
     provided by the recursive as_code function::

        1335     def as_tbool(self, name="esr"):
        1336         tbf = TBoolBashFunction(name=name, root=self.alabel, body=self.as_code(lang="py")  )
        1337         return str(tbf)


     """
     def __init__(self, *args, **kwa):
         dict.__init__(self, *args, **kwa)

     def _get_stamp(self):
         return "# generated by tbool.py : %s " % ( now_() )
     stamp = property(_get_stamp)

     def _get_runline(self):
         return "\n".join([
                   "# opticks-;opticks-tbool %(name)s " % self,
                   "# opticks-;opticks-tbool-vi %(name)s " % self, ""])

     runline = property(_get_runline)


     head = property(lambda self:template_head % self)
     body = property(lambda self:template_body % self)
     tail = property(lambda self:template_tail % self)

     path = property(lambda self:os.path.expandvars("$TMP/tbool%(name)s.bash" % self))

     def save(self):
         log.info("saving to %s " % self.path) 
         file(self.path,"w").write(str(self))

     def test(self):
         print self.stamp
         print self.head
         print self.body
         print self.tail

     def __str__(self):
         return "\n".join([self.stamp,self.runline, self.head, self.stamp,self.runline,  self.body, self.tail])  


if __name__ == '__main__':

     logging.basicConfig(level=logging.INFO)

     tbf = TBoolBashFunction(name="0", root="a", body=test_body)
     tbf.test()

     print tbf 

     tbf.save()

     
