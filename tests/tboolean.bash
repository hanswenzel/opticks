tboolean-source(){   echo $(opticks-home)/tests/tboolean.bash ; }
tboolean-vi(){       vi $(tboolean-source) ; }
tboolean-usage(){ cat << \EOU

tboolean- 
======================================================


TODO
--------

* CSG geometry config using python that writes
  a serialization turns out to be really convenient...
  Howabout attaching emission of torch photons to pieces
  of geometry ?

* Also there is lots of duplication in torchconfig
  between all the tests... pull out the preparation of 
  torchconfig metadata into a python script ? that 
  all the tests can use ? 

  Hmm but the sources need to correspond to geometry ...
  this needs an overhaul.
  

NOTES
--------

tracetest option
~~~~~~~~~~~~~~~~~~~

When using tracetest option only a single intersect is
done using oxrap/cu/generate.cu:tracetest and a special 
format of the photon buffer is used, for analysis by 
ana/tboolean.py 

However in tracetest mode the record buffer filling 
is not implemented so the visualization 
of photon paths is not operational.


bash test geometry configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* CSG tree is defined in breadth first or level order

* parameters of boolean operations currently define adhoc box 
  intended to contain the geometry, TODO: calculate from bounds of the contained tree 

* offsets arg identifies which nodes belong to which primitives by pointing 
  at the nodes that start each primitive

::

     1  node=union        parameters=0,0,0,400           boundary=Vacuum///$material 
     2  node=difference   parameters=0,0,100,300         boundary=Vacuum///$material
     3  node=difference   parameters=0,0,-100,300        boundary=Vacuum///$material
     4  node=box          parameters=0,0,100,$inscribe   boundary=Vacuum///$material
     5  node=sphere       parameters=0,0,100,$radius     boundary=Vacuum///$material
     6  node=box          parameters=0,0,-100,$inscribe  boundary=Vacuum///$material
     7  node=sphere       parameters=0,0,-100,$radius    boundary=Vacuum///$material

Perfect tree with n=7 nodes is depth 2, dev/csg/node.py (root2)::
 
                 U1                
                  o                
         D2              D3        
          o               o        
     b4      s5      b6      s7    
      o       o       o       o         


* nodes identified with 1-based levelorder index, i
* left/right child of node i at l=2i, r=2i+1, so long as l,r < n + 1


python test geometry configuration 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* python config is much more flexible than bash, allowing 
  more natural tree construction and node reuse

Running functions such as tboolean-box-sphere-py- 
construct CSG node trees for each "solid" of the geometry.
Typically the containing volume is a single node tree 
and the contained volume is a multiple node CSG tree.

These trees are serialized into numpy arrays and written to 
files within directories named after the bash function eg 
"/tmp/blyth/opticks/tboolean-box-sphere-py-". 

The bash function emits to stdout only the name of 
this directory which is captured and used in the 
commandline testconfig csgpath slot.


testconfig modes
~~~~~~~~~~~~~~~~~~

PmtInBox

     * see tpmt- for this one

BoxInBox

     * CSG combinations not supported, union/intersection/difference nodes
       appear as placeholder boxes

     * raytrace superficially looks like a union, but on navigating inside 
       its apparent that its just overlapped individual primitives


PyCsgInBox

     * requires csgpath identifying directory containing serialized CSG trees
       and csg.txt file with corresponding boundary spec strings


CsgInBox

     * DECLARED DEAD, USE PyCsgInBox
     * requires "offsets" identifying node splits into primitives eg offsets=0,1 
     * nodes are specified in tree levelorder, trees must be perfect 
       with 1,3,7 or 15 nodes corresponding to trees of height 0,1,2,3

EOU
}

tboolean-env(){      olocal- ;  }
tboolean-dir(){ echo $(opticks-home)/tests ; }
tboolean-cd(){  cd $(tboolean-dir); }

join(){ local IFS="$1"; shift; echo "$*"; }

tboolean-tag(){  echo 1 ; }
tboolean-det(){  echo boolean ; }
tboolean-src(){  echo torch ; }
tboolean-args(){ echo  --det $(tboolean-det) --src $(tboolean-src) ; }

tboolean--(){

    tboolean-

    local msg="=== $FUNCNAME :"
    local cmdline=$*

    local testconfig
    if [ -n "$TESTCONFIG" ]; then
        testconfig=${TESTCONFIG}
    else
        testconfig=$(tboolean-testconfig)
    fi 

    #        --rendermode "-axis" \

    op.sh  \
            $cmdline \
            --animtimemax 20 \
            --timemax 20 \
            --geocenter \
            --eye 1,0,0 \
            --dbganalytic \
            --test --testconfig "$testconfig" \
            --torch --torchconfig "$(tboolean-torchconfig)" \
            --torchdbg \
            --tag $(tboolean-tag) --cat $(tboolean-det) \
            --save 
}

tboolean-tracetest()
{
    tboolean-- --tracetest $*
}

tboolean-enum(){
   local tmp=$TMP/$FUNCNAME.exe
   clang $OPTICKS_HOME/optixrap/cu/boolean-solid.cc -lstdc++ -I$OPTICKS_HOME/optickscore -o $tmp && $tmp $*
}






tboolean-torchconfig()
{

    local pol=${1:-s}
    local wavelength=500
    local identity=1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,1.000,0.000,0.000,0.000,0.000,1.000

    #local photons=1000000
    local photons=100000
    #local photons=1

    local torch_config_disc=(
                 type=disc
                 photons=$photons
                 mode=fixpol
                 polarization=1,1,0
                 frame=-1
                 transform=$identity
                 source=0,0,599
                 target=0,0,0
                 time=0.1
                 radius=300
                 distance=200
                 zenithazimuth=0,1,0,1
                 material=Vacuum
                 wavelength=$wavelength 
               )


    local discaxial_target=0,0,0
    local torch_config_discaxial=(
                 type=discaxial
                 photons=$photons
                 frame=-1
                 transform=$identity
                 source=$discaxial_target
                 target=0,0,0
                 time=0.1
                 radius=100
                 distance=400
                 zenithazimuth=0,1,0,1
                 material=Vacuum
                 wavelength=$wavelength 
               )



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
                 wavelength=$wavelength 
               )



    #echo "$(join _ ${torch_config_discaxial[@]})" 
    #echo "$(join _ ${torch_config_disc[@]})" 
    echo "$(join _ ${torch_config_sphere[@]})" 
}



#tboolean-material(){ echo MainH2OHale ; }
tboolean-material(){ echo GlassSchottF2 ; }
tboolean-container(){ echo Rock//perfectAbsorbSurface/Vacuum ; }
tboolean-testobject(){ echo Vacuum///GlassSchottF2 ; }


tboolean-bib-box()
{
    local test_config=(
                 mode=BoxInBox
                 name=$FUNCNAME
                 analytic=1

                 node=box      parameters=0,0,0,1000               boundary=$(tboolean-container)
                 node=box      parameters=0,0,0,100                boundary=$(tboolean-testobject)

                    )
     echo "$(join _ ${test_config[@]})" 
}


tboolean-bib-box-small-offset-sphere()
{
    local test_config=(
                 mode=BoxInBox
                 name=$FUNCNAME
                 analytic=1

                 node=sphere           parameters=0,0,0,1000          boundary=$(tboolean-container)
 
                 node=${1:-difference} parameters=0,0,0,300           boundary=$(tboolean-testobject)
                 node=box              parameters=0,0,0,200           boundary=$(tboolean-testobject)
                 node=sphere           parameters=0,0,200,100         boundary=$(tboolean-testobject)
               )
     echo "$(join _ ${test_config[@]})" 
}

tboolean-bib-box-sphere()
{
    local operation=${1:-difference}
    local inscribe=$(python -c "import math ; print 1.3*200/math.sqrt(3)")
    local test_config=(
                 mode=BoxInBox
                 name=$FUNCNAME
                 analytic=1

                 node=box          parameters=0,0,0,1000          boundary=$(tboolean-container)
 
                 node=$operation   parameters=0,0,0,300           boundary=$(tboolean-testobject)
                 node=box          parameters=0,0,0,$inscribe     boundary=$(tboolean-testobject)
                 node=sphere       parameters=0,0,0,200           boundary=$(tboolean-testobject)
               )

     echo "$(join _ ${test_config[@]})" 
}







tboolean-box(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null) tboolean-- ; } 
tboolean-box-(){  $FUNCNAME- | python $* ; }
tboolean-box--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.polyconfig import PolyConfig
from opticks.analytic.csg import CSG  

args = opticks_main()

container = CSG("box")
container.boundary = args.container
container.meta.update(PolyConfig("CONTAINER").meta)


im = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )

#tr = dict(translate="0,0,100", rotate="1,1,1,45", scale="1,1,2")
#tr = dict(scale="2,2,2", rotate="1,1,1,45")

kwa = {}
kwa.update(im)
#kwa.update(tr)

box_param = [0,0,0,200]
box3_param = [300,300,200,0] 

box = CSG("box3", param=box3_param, boundary="$(tboolean-testobject)", **kwa )
box.dump()


CSG.Serialize([container, box], args.csgpath, outmeta=True )
EOP
}



tboolean-cone-scan(){ SCAN="0,0,100,1,0,0,0,300,10" NCSGScanTest $TMP/tboolean-cone--/1 ; }
tboolean-cone(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-cone-(){  $FUNCNAME- | python $* ; }
tboolean-cone--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.polyconfig import PolyConfig
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(verbosity="1", poly="HY", resolution="4" )

r2,r1 = 100,300
#r2,r1 = 300,300    ## with equal radii (a cylinder) polygonization and raytrace both yield nothing 
#r2,r1 = 300,100    ## radii swapped (upside-down cone) works

z2 = 200
z1 = 0

a = CSG("cone", param=[r1,z1,r2,z2] )
a.dump()

obj = a 
container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="HY", resolution="4", verbosity="0" )

CSG.Serialize([container, obj], args.csgpath )

EOP
}




tboolean-prism(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null) &&  tboolean-- $* ; } 
tboolean-prism-(){  $FUNCNAME- | python $* ; }
tboolean-prism--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.polyconfig import PolyConfig
from opticks.analytic.csg import CSG  
from opticks.analytic.prism import make_prism  

args = opticks_main()

container = CSG("box")
container.boundary = args.container
container.meta.update(PolyConfig("CONTAINER").meta)

im = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )

angle, height, depth = 45, 200, 300
planes, verts, bbox = make_prism(angle, height, depth)

obj1 = CSG("convexpolyhedron")
obj1.boundary = args.testobject
obj1.planes = planes
obj1.param2[:3] = bbox[0]
obj1.param3[:3] = bbox[1]

# unlike other solids, need to manually set bbox for solids stored as a 
# set of planes in NConvexPolyhedron as cannot easily derive the bbox 
# from the set of planes

obj1.meta.update(im)

obj1.dump()

#print obj1.as_python()


CSG.Serialize([container, obj1], "$TMP/$FUNCNAME", outmeta=True )


EOP

}















tboolean-trapezoid-deserialize(){ NCSGDeserializeTest $TMP/${FUNCNAME/-deserialize}-- ; }
tboolean-trapezoid(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-trapezoid-(){  $FUNCNAME- | python $* ; }
tboolean-trapezoid--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.polyconfig import PolyConfig
from opticks.analytic.prism import make_trapezoid, make_icosahedron
from opticks.analytic.csg import CSG  

args = opticks_main()

container = CSG("box")
container.boundary = args.container
container.meta.update(PolyConfig("CONTAINER").meta)

im = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )

obj = CSG("trapezoid")
obj.boundary = args.testobject

cube = False

if cube:
    planes = CSG.CubePlanes(200.)
    bbox = [[-201,-201,-201,0],[ 201, 201, 201,0]] 
else:
    #planes, verts, bbox, srcmeta = make_trapezoid(z=50.02, x1=100, y1=27, x2=237.2, y2=27 )
    planes, verts, bbox, srcmeta = make_trapezoid(z=2228.5, x1=160, y1=20, x2=691.02, y2=20 )
    #planes, verts, bbox, srcmeta = make_icosahedron()
pass

obj.planes = planes
obj.param2[:3] = bbox[0]
obj.param3[:3] = bbox[1]


#
# unlike other solids, need to manually set bbox for solids stored as a 
# set of planes in NConvexPolyhedron as cannot easily derive the bbox 
# from the set of planes
#

obj.meta.update(srcmeta)
obj.meta.update(im)

obj.dump()

CSG.Serialize([container, obj], "$TMP/$FUNCNAME", outmeta=True )
EOP
}




tboolean-uniontree(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null) &&  tboolean-- || echo $FUNCNAME : ERROR : investigate with : $FUNCNAME-  ; } 
tboolean-uniontree-(){  $FUNCNAME- | python $* ; }
tboolean-uniontree--(){ cat << EOP 

import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.polyconfig import PolyConfig
from opticks.analytic.csg import CSG  
from opticks.analytic.treebuilder import TreeBuilder 

args = opticks_main()

container = CSG("box")
container.boundary = args.container
container.meta.update(PolyConfig("CONTAINER").meta)

im = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )


sp = CSG("sphere", param=[0,0,-1,100] )   # zrange -100:100
sp2 = CSG("sphere", param=[0,0,0,200] )   # zrange -200:200

zs = CSG("zsphere", param=[0,0,0,500], param1=[100,200,0,0],param2=[0,0,0,0])  # zrange 100:200
zs.param2.view(np.uint32)[0] = 3 

co = CSG("cone", param=[300,0,100,200])   # zrange 0:200


#prim = [sp,zs,co]    
prim = [sp,co,zs]    

#prim = [sp2, zs, co]  # works as expected
#prim = [sp, co]       # works as expected
#prim = [sp, zs ]      #  sp just touches zp, so difficult to say 
#prim = [sp2, zs]      # expected behavior


ut = TreeBuilder.uniontree(prim, name="$FUNCNAME")
ut.boundary = args.container
ut.meta.update(im)
ut.dump()

CSG.Serialize([container, ut], "$TMP/$FUNCNAME", outmeta=True )


"""
prim = [sp,zs,co]    

            un    
     un          co
 sp      zs        


         __________
        /          \       zs
       +---+-----+--+  
          /_______\        co
             \_/           sp


Looks like improper "shadow" sphere surface inside the union,  
propagation intersects with improper surf between sphere and cone,
nudging sphere upwards makes a hole in the center of improper surface
 ... suspect issue with three way overlapping 
nudging downwards still get the improper surf


Can only see the shadow shape when positioned to look up at cone 
(ie looking into threeway region)

Changing order to a more natural (2-way overlapping) one, gets expected behavior

::

    prim = [sp,co,zs]   

                un    
         un          zs
     sp      co        


Does the order depency of a set of unions indicate a bug, 
or a limitation of the algorithm ... or is it just 
a result of having coincident z-faces ?

NON-CONCLUSION: 

* make sure uniontree primitives are in a sensible order 
* avoid three way overlapping where possible


Thinking about the pairwise CSG algorithm the behaviour is kinda
understandable... sp and zs are initially tangential and then after 
nudging the sp upwards creates a small intersection opening up the hole.

But then the union with the cone thru the middle should get rid of that 
surface, and open up a full cavity betweeb the zs and sp ?


TODO:

* see what happens when all coincidident z-planes are avoided

* automatic z-growing in polycone to avoid coincident surfaces 
  (this would be difficult in a general uniontree, but easy in 
  polycone as just needs 3*epsilon grow in z from the
  smaller radius part into the larger radius part)  
  ... analogous to joins in carpentry

"""

EOP
}



tboolean-parade(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-parade-(){ $FUNCNAME- | python $* ; } 
tboolean-parade--(){ cat << EOP 

import logging, numpy as np
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
        
args = opticks_main(csgpath="$TMP/$FUNCNAME")

container = CSG("box", param=[0,0,0,1200], boundary=args.container, poly="MC", nx="20" )

dcs = dict(poly="DCS", nominal="7", coarse="6", threshold="1", verbosity="0")
hy = dict(poly="HY", level="4", polycfg="contiguous=1,reversed=0,numsubdiv=0,offsave=1", verbosity="0" )


CSG.boundary = args.testobject
#CSG.kwa = dcs
CSG.kwa = hy

a = CSG("sphere", param=[0,0,0,100]) 
b = CSG("zsphere", param=[0,0,0,100], param1=[-50,60,0,0]) 
c = CSG("box3",param=[100,50,70,0]) 
d = CSG("box",param=[0,0,10,50])
e = CSG("cylinder",param=[0,0,0,100], param1=[-100,100,0,0])
f = CSG("disc",param=[0,0,0,100], param1=[-1,1,0,0])
g = CSG("cone", param=[100,-100,50,100])
h = CSG.MakeTrapezoid(z=100, x1=80, y1=100, x2=100, y2=80)
i = CSG.MakeSegment(phi0=0,phi1=45,sz=100,sr=100)
j = CSG.MakeIcosahedron(scale=100.)

prims = [a,b,c,d,e,f,g,h,i,j]

h.meta.update(poly="IM", resolution="40")
i.meta.update(poly="IM", resolution="40")
j.meta.update(poly="IM", resolution="40")


nprim = len(prims)
hprim = nprim/2
ygap = 200
ysize = hprim*ygap
yy = np.repeat(np.linspace(-ysize/2,ysize/2, hprim), 2)
xx = np.tile(np.linspace(-ygap,ygap,2),hprim)
for iprim, prim in enumerate(prims):
    prim.translate = "%s,%s,0" % (xx[iprim],yy[iprim])
pass

CSG.Serialize([container] + prims, args.csgpath )

EOP
}




tboolean-complement-deserialize(){ NCSGDeserializeTest $TMP/${FUNCNAME/-deserialize}-- ; }
tboolean-complement(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-complement-(){ $FUNCNAME- | python $* ; } 
tboolean-complement--(){ cat << EOP 

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50", verbosity="1", ctrl="0" )


al = CSG("sphere", param=[0,0,50,100])   # mid-right-Y, conventional difference(top-sphere,bottom-sphere)
ar = CSG("sphere", param=[0,0,-50,100])
a = CSG("difference", left=al, right=ar, translate="0,200,0" )

bl = CSG("sphere", param=[0,0,50,100])   # far-right-Y,     intersect(top-sphere, complement(bot-sphere) )      
br = CSG("sphere", param=[0,0,-50,100], complement=True)
b = CSG("intersection", left=bl, right=br, translate="0,400,0" )

cl = CSG("sphere", param=[0,0, 50,100])  # mid left Y,  conventional difference(bot-sphere,top-sphere) 
cr = CSG("sphere", param=[0,0,-50,100])
c = CSG("difference", left=cr, right=cl, translate="0,-200,0" )

dl = CSG("sphere", param=[0,0,50,100], complement=True)    #  far-left-Y  intersect( complement(top-sphere), bot-sphere )    #  bot-top
dr = CSG("sphere", param=[0,0,-50,100])
d = CSG("intersection", left=dl, right=dr, translate="0,-400,0" )

#b.analyse()
#b.dump_tbool("name")
#b.dump_NNodeTest("name")

CSG.Serialize([container, a, b, c, d ], args.csgpath )

"""
Getting expected:

+ve Y : top - bottom
-ve Y : bottom - top

"""

EOP
}

tboolean-zsphere(){ TESTCONFIG=$(tboolean-zsphere- 2>/dev/null)    tboolean-- ; } 
tboolean-zsphere-(){ $FUNCNAME- | python $* ; } 
tboolean-zsphere--(){ cat << EOP 

import numpy as np
from opticks.analytic.csg import CSG  

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20" )


im = dict(poly="IM", resolution="50", verbosity="3", ctrl="0" )
#tr = dict(scale="1,1,2")
tr = dict(translate="0,0,100", rotate="1,1,1,45", scale="1,1,2")

kwa = {}
kwa.update(im)
#kwa.update(tr)

#zsphere = CSG("zsphere", param=[0,0,0,500], param1=[-200,200,0,0],param2=[0,0,0,0],  boundary="$(tboolean-testobject)", **kwa )
zsphere = CSG("zsphere", param=[0,0,0,500], param1=[100,200,0,0],param2=[0,0,0,0],  boundary="$(tboolean-testobject)", **kwa )

# flag settings are now ignored
#ZSPHERE_QCAP = 0x1 << 1   # ZMAX
#ZSPHERE_PCAP = 0x1 << 0   # ZMIN
#flags = ZSPHERE_QCAP | ZSPHERE_PCAP
#flags = ZSPHERE_QCAP | ZSPHERE_PCAP
#flags = ZSPHERE_QCAP
#flags = ZSPHERE_PCAP
#flags = 0 
#zsphere.param2.view(np.uint32)[0] = flags 

CSG.Serialize([container, zsphere], "$TMP/$FUNCNAME" )

EOP
}




tboolean-union-zsphere(){ TESTCONFIG=$(tboolean-union-zsphere- 2>/dev/null)    tboolean-- ; } 
tboolean-union-zsphere-(){ $FUNCNAME- | python $* ; } 
tboolean-union-zsphere--(){ cat << EOP 

import numpy as np
from opticks.analytic.csg import CSG  

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20" )

im = dict(poly="IM", resolution="50", verbosity="3", ctrl="0" )

kwa = {}
kwa.update(im)


ZSPHERE_QCAP = 0x1 << 1   # ZMAX
ZSPHERE_PCAP = 0x1 << 0   # ZMIN
flags = ZSPHERE_QCAP | ZSPHERE_PCAP

lzs = CSG("zsphere", param=[0,0,0,500], param1=[-200,200,0,0],param2=[0,0,0,0] )
lzs.param2.view(np.uint32)[0] = flags   

rzs = CSG("zsphere", param=[0,0,0,500], param1=[300,400,0,0] ,param2=[0,0,0,0] )
rzs.param2.view(np.uint32)[0] = flags

uzs = CSG("union", left=lzs, right=rzs, boundary="$(tboolean-testobject)", **kwa )

CSG.Serialize([container, uzs], "$TMP/$FUNCNAME" )

"""
Observe wierdness when caps are off:

* wrong sub-object appears in front of other...
* hmm maybe fundamental closed-sub-object-limitation again 

"""

EOP
}






tboolean-difference-zsphere(){ TESTCONFIG=$(tboolean-difference-zsphere- 2>/dev/null)    tboolean-- ; } 
tboolean-difference-zsphere-(){ $FUNCNAME- | python $* ; } 
tboolean-difference-zsphere--(){ cat << EOP 

import numpy as np
from opticks.analytic.csg import CSG  

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20" )

im = dict(poly="IM", resolution="50", verbosity="3", ctrl="0", seeds="0,0,0,1,0,0")

kwa = {}
kwa.update(im)

ZSPHERE_QCAP = 0x1 << 1   # ZMAX
ZSPHERE_PCAP = 0x1 << 0   # ZMIN
both = ZSPHERE_QCAP | ZSPHERE_PCAP

lzs = CSG("zsphere", param=[0,0,0,500], param1=[-100,100,0,0],param2=[0,0,0,0] )
lzs.param2.view(np.uint32)[0] = both   

rzs = CSG("zsphere", param=[0,0,0,400], param1=[-101,101,0,0] ,param2=[0,0,0,0] )
rzs.param2.view(np.uint32)[0] = both

dzs = CSG("difference", left=lzs, right=rzs, boundary="$(tboolean-testobject)", **kwa )

CSG.Serialize([container, dzs], "$TMP/$FUNCNAME" )

"""

#. FIXED: Differencing two concentric zspheres with same zmin/zmax does not 
   produce the expected ring like shape, unless you slightly increase the 
   zmin/zmax of the one you are subtracting over the other

   * avoid common/coincident faces between the subtracted solids 

#. FIXED: IM poly: fails to find surface even radii 400 and 500, hmm NZSphere looking in +z, 
   but manual seeding doesnt find surface either, it does after fix  
   bug in the setting of manual seed directions in NImplicitMesher


"""

EOP
}








tboolean-hybrid(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-hybrid-combinetest(){ lldb NOpenMeshCombineTest -- $TMP/${FUNCNAME/-combinetest}--/1 ; }
tboolean-hybrid-(){ $FUNCNAME- | python $* ; } 
tboolean-hybrid--(){ cat << EOP
from opticks.analytic.csg import CSG  

container = CSG("box",   name="container",  param=[0,0,0,1000], boundary="$(tboolean-container)", poly="IM", resolution="10" )

box = CSG("box", param=[0,0,0,201], boundary="$(tboolean-testobject)", level="2" )
sph = CSG("sphere", param=[100,0,0,200], boundary="$(tboolean-testobject)", level="4"  )

polycfg="contiguous=1,reversed=0,numsubdiv=0,offsave=1"
obj = CSG("union", left=box, right=sph, boundary="$(tboolean-testobject)", poly="HY", level="4", verbosity="2", polycfg=polycfg  )

# only root node poly and polycfg are obeyed, and distributed to the entire tree

CSG.Serialize([container, obj ], "$TMP/$FUNCNAME" )
#CSG.Serialize([container, box ], "$TMP/$FUNCNAME" )
#CSG.Serialize([container, box, sph ], "$TMP/$FUNCNAME" )


"""
* initially with box size and sphere radius equal, the poles of the sphere
  were just touching the inside of the box : this is invisible in the ray trace but meant that 
  the polecaps tris where classified as frontier as the poles were regarded as being 
  outside the box (perhaps some epsiloning needed here) 
  ... how to handle such tangency issues without manual tweaks ? 


   (x - 100)^2 + y^2 + z^2 = 200^2       x = 201 

        y^2 + z^2 = 200^2 - 101^2

In [1]: np.sqrt( 200*200-101*101 ) 
Out[1]: 172.62386856978961
  

"""

EOP
}

tboolean-hyctrl(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-hyctrl-polytest-lldb(){ lldb NPolygonizerTest -- $TMP/${FUNCNAME/-polytest-lldb}--/1 ; }
tboolean-hyctrl-polytest(){           NPolygonizerTest    $TMP/${FUNCNAME/-polytest}--/1 ; }
tboolean-hyctrl-(){ $FUNCNAME- | python $* ; } 
tboolean-hyctrl--(){ cat << EOP
from opticks.analytic.csg import CSG  

container = CSG("box",   name="container",  param=[0,0,0,1000], boundary="$(tboolean-container)", poly="IM", resolution="1" )

#ctrl = "3"   # tripatch
# tripatch : works in phased and contiguous, contiguous-reversed missed edge(looks broken) 

#ctrl = "4"  # tetrahedron
#ctrl = "6"  # cube
#ctrl = "66"  
# hexpatch_inner : contiguous works, but not with reversed, showing face order sensitivity 
# hexpatch_inner : phased fails to do last flip, when reversed fails to do two flips

ctrl = "666"  # hexpatch 
# hexpatch : contiguous works until reversed=1 showing face order sensitivity
# hexpatch : phased is missing ~6 flips 

cfg0="phased=1,split=1,flip=1,numflip=0,reversed=0,maxflip=0,offsave=1"
cfg1="contiguous=1,reversed=1,numsubdiv=1,offsave=1"

box = CSG("box", param=[0,0,0,500], boundary="$(tboolean-testobject)", poly="HY", level="0", ctrl=ctrl, verbosity="4", polycfg=cfg1 )



CSG.Serialize([container, box  ], "$TMP/$FUNCNAME" )

EOP
}






tboolean-boxsphere(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-boxsphere-(){ $FUNCNAME- | python $* ; } 
tboolean-boxsphere--(){ cat << EOP
from opticks.analytic.csg import CSG  

container = CSG("sphere",           param=[0,0,0,1000], boundary="$(tboolean-container)", poly="IM", resolution="10" )

box = CSG("box",    param=[0,0,0,200], boundary="$(tboolean-testobject)", rotate="0,0,1,45" )
sph = CSG("sphere", param=[0,0,0,100], boundary="$(tboolean-testobject)", translate="0,0,200", scale="1,1,0.5" )

object = CSG("${1:-difference}", left=box, right=sph, boundary="$(tboolean-testobject)", poly="IM", resolution="50" )

CSG.Serialize([container, object], "$TMP/$FUNCNAME" )
EOP
}




tboolean-uncoincide-loadtest(){ ${FUNCNAME/-loadtest}- ; NCSGLoadTest $TMP/${FUNCNAME/-loadtest}--/1 ; }
tboolean-uncoincide(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-uncoincide-(){ $FUNCNAME- | python $* ; } 
tboolean-uncoincide--(){ cat << EOP

outdir = "$TMP/$FUNCNAME"

import logging
log = logging.getLogger(__name__)

from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  

args = opticks_main()

CSG.boundary = "$(tboolean-testobject)"
CSG.kwa = dict(verbosity="1", poly="IM", resolution="20" )

# container=1 metadata causes sphere/box to be auto sized to contain other trees
container = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary="$(tboolean-container)", poly="HY", level="5" )

log.info(r"""

$FUNCNAME
=========================

* FIXED : disabling uncoincide not causing speckles, its causing subtraction not to appear
* FIXED : uncoincide fails to work when there is a rotation applied to the composite obj
* FIXED : applying a non-uniform scale 0.5,1.5,1 causes speckles from some angles


SUSPECT the standard make_transformed is doing the nudge translation last
as translation last is the usual TRS order, but nudging 
needs it to be first ...

::
  
  -200,200                   200,200
        +-----------------------+
        |                150,150|
        |   +---------------+   |
        |   |               |   |
        |   |       Y       |   |
        |   |       |       |   |
        |   |       +--X    |   |
        |   |               |   |
        |   |               |   |
        |   |               |   |
        |   +---------------+   |
        |                       |
        +-----------------------+         
  -200,-200                  200,-200



     Z
     |

      50
        +---------------+--+--+--+--- <--- +Z face:1 at 50 
        |
        |
     25 +  +  +  +  +
        |
        |
        +               + + + + + +   <-- -Z face:0 at 0 
        |
        | 
    -25 +  +  +  +  +
        |
        |
        +----------------------------
     -50


Testing what obj transforms prevent uncoincide from working:

* obj.rotate = "0,0,1,45"  # works : about Z is OK
* obj.rotate = "1,0,0,180" # works
* obj.rotate = "1,0,0,360" # works
* obj.rotate = "1,0,0,45" # nope about X or Y 
* obj.rotate = "1,0,0,90" # nope

* obj.scale = "0.5,0.5,0.5"   # uniform scale works
* obj.scale = "0.5,0.5,2"   # works
* obj.scale = "1,1,1.5"     # works

ALL WORKING NOW

Fixed by doing coincidence detection and nudging 
entirely in FRAME_LOCAL of the CSG boolean sub-objects (usually difference) node.
Hmm have only tested right node having a transform.

Basically this means that only the left/right transforms are relevant, the
gtransforms are not used.

To enable this, added a frame argument to par_pos_ and sdf_ and bbox_

       obj
      /  \
     A    B 

Local coincidence testing and uncoincidence fixes 
needs to be done in obj frame, and thus be impervious to whatever
transforms are applied to obj... only the transforms
applied to A and B are relevant.

This means need an SDF route that can operate in such local
frames is needed.


""")

a = CSG("box3", param=[400,400,100,0] )
b = CSG("box3", param=[300,300,50,0], translate="0,0,25" )

obj = a - b
obj.meta.update(uncoincide="1")  # 0:disable uncoincidence nudging

obj.translate = "0,0,100"
obj.scale = "1,1,1.5"
obj.rotate = "1,0,0,45"

CSG.Serialize([container, obj], outdir )

EOP
}





tboolean-disc(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-disc-(){ $FUNCNAME- | python $* ; } 
tboolean-disc--(){ cat << EOP

outdir = "$TMP/$FUNCNAME"
obj_ = "$(tboolean-testobject)"
con_ = "$(tboolean-container)"

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

CSG.boundary = obj_
CSG.kwa = dict(verbosity="1",poly="IM")

delta = 1.0 

z1,z2 = -0.050,0.050
z1d,z2d = z1-delta,z2+delta


a = CSG("disc", param = [0.000,0.000,523.0,2223.000],param1 = [z1,z2,0.000,0.000])
b = CSG("disc", param = [1000.000,0.000,0.000,223.000],param1 = [z1d,z2d,0.000,0.000])

obj = a - b 

con = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary=con_ , poly="HY", level="5" )
CSG.Serialize([con, obj], outdir )

EOP
}






tboolean-esr(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-esr-(){ $FUNCNAME- | python $* ; } 
tboolean-esr--(){ cat << EOP

outdir = "$TMP/$FUNCNAME"
obj_ = "$(tboolean-testobject)"
con_ = "$(tboolean-container)"

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

CSG.boundary = obj_
CSG.kwa = dict(verbosity="1",poly="IM")

z1,z2 = -0.050,0.050

scale = 10. 

z1 *= scale
z2 *= scale

# Varying the delta that increases the thickness of the subtracted disc cylinder 
#
# delta observations for scale = 1
#
#delta = 20.    # NO pole artifact visible
#delta = 15.    # slight artifacting 
#delta = 10.    # pole artifact, disappears axially 
#delta = 1.     # pole artifact, disappears axially   (scale:10 tiny artifact)
#delta = 0.1    # pole artifact, does not disappear axially 
#delta = 0.01   # pole artifact, does not disappear axially 
#delta = 0.001  # pole artifact, does not disappear axially + RING SPECKLES, ~/opticks_refs/tboolean_esr_speckle_centered_on_pole_delta_10-3.png 
delta = 0.0001 # full metal speckle from all angles
#delta = 0.     # zero makes the subtraction appear not to work, just get a disc not an annulus 

z1d,z2d = z1-delta, z2+delta

a = CSG("cylinder", param = [0.000,0.000,0.000,2223.000],param1 = [z1 , z2,0.000,0.000])
b = CSG("cylinder", param = [0.000,0.000,0.000,2144.500],param1 = [z1d,z2d,0.000,0.000],complement = True)
ab = CSG("intersection", left=a, right=b)

obj = ab

check_3 = False
if check_3:
    c = CSG("cylinder", param = [0.000,0.000,0.000,2223.001],param1 = [-0.050,0.050,0.000,0.000])
    d = CSG("cylinder", param = [0.000,0.000,0.000,2144.500],param1 = [-20.051,20.051,0.000,0.000],complement = True)
    cd = CSG("intersection", left=c, right=d)
    cd.translate = "0,0,10"

    e = CSG("cylinder", param = [0.000,0.000,0.000,2223.001],param1 = [-0.050,0.050,0.000,0.000])
    f = CSG("cylinder", param = [0.000,0.000,0.000,2144.500],param1 = [-20.051,20.051,0.000,0.000],complement = True)
    ef = CSG("intersection", left=e, right=f)
    ef.translate = "0,0,20"

    abcd = CSG("union", left=ab, right=cd) 
    abcdef = CSG("union", left=abcd, right=ef) 

    obj = abcdef
pass



con = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary=con_ , poly="HY", level="5" )
CSG.Serialize([con, obj], outdir )


"""

Annulus shape, difference of two very thin discs : 
-----------------------------------------------------

Speckles like crazy in the hole, especially when viewed from high angle, end on 

* without increasing thickness of the subtracted, the hole doesnt show : just looks like complete disc

* speckles only appear for very small delta

* delta 0.001 shows the speckles have a circular disposition surrounding the pole

* substantially increasing *delta* thickness of the subtracted disk, gets rid of the speckle 

  * NB this is perfectly valid thing to do, IT DOES NOT CHANGE GEOMETRY, 
    are increasing the size of smth are about to subtract.. 
    there is no concern about bumping into other geometry.

  * DONE: verified the above statement by testing two and three close annuli whose
    subtracted cylinders would overlap, bizarrely this also makes the 
    pole artifact go away ???

* small bright split circle  "pole artifact" when viewed from high angles that 
  moves across the hole as the angle is changed, but disappears in the middle
  when very close to axis (algorithm special casing of axial rays ?)

* increasing *delta* yet more gets rid of the pole artifact, varying the 
  parameters suggests that the primary cause of the pole artifact 
  is the thinness of the "disc" cylinder resulting in near degeneracy of the endcaps 
  intersections

* Adopting robust quadratic did not change behavior... so presumably 
  the issue is from the endcap intersects when they are almost degenerate 

  * using a separate disc primitive or special casing cylinder intersection
    for near degenerate endcaps, seem like the next available thing to try


* THESE ISSUES MOTIVATED DEVELOPMENT OF CSG_DISC see tboolean-disc


"""

EOP
}



tboolean-rip(){ local fnpy="tboolean-${1:-sc}--" ; local py=$TMP/$fnpy.py ; $fnpy > $py ;  ipython --profile=g4opticks -i $py ; }
# jump into ipython session with the python streamed from a bash function

tboolean-sc-loadtest(){ ${FUNCNAME/-loadtest}- ; NCSGLoadTest $TMP/${FUNCNAME/-loadtest}--/1 ; }
tboolean-sc(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-sc-(){ $FUNCNAME- | python $* ; } 
tboolean-sc--(){ cat << EOP

outdir = "$TMP/$FUNCNAME"

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main

from opticks.analytic.csg import CSG  
from opticks.analytic.gdml import GDML
from opticks.analytic.sc import Sc
from opticks.analytic.treebase import Tree
from opticks.analytic.treebuilder import TreeBuilder

args = opticks_main()


CSG.boundary = "$(tboolean-testobject)"
CSG.kwa = dict(verbosity="1")

# container=1 metadata causes sphere/box to be auto sized to contain other trees
container = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary="$(tboolean-container)", poly="HY", level="5" )

gdml = GDML.parse()
tree = Tree(gdml.world)


#gsel = "/dd/Geometry/AdDetails/lvRadialShieldUnit0x"   # thin shell cy with 6 cy holes, poly mess
#gsel = "/dd/Geometry/AdDetails/lvTopReflector0x"      # flat plate with 5 holes, no poly
#gsel = "/dd/Geometry/AdDetails/lvTopRefGap0x"          # flat plate with 5 holes, no poly 
#gsel = "/dd/Geometry/AdDetails/lvTopESR0x"            # flat plate with 9 holes, no poly, center one has coincidence speckle FIXED with disc
#gsel = "/dd/Geometry/AdDetails/lvSstTopCirRibBase0x"       # ring with 4-T slots cut out, coincidence speckle at top of T  

#gsel = "/dd/Geometry/CalibrationSources/lvLedSourceAssy0x"  # three capsules connected with 2 wires , poly works!

gsel = "/dd/Geometry/OverflowTanks/lvGdsOflTnk0x"   # mixed deep tree (union of cy-cy) 

gidx = 0 

target = tree.findnode(gsel, gidx)


orig = Sc.translate_lv(target.lv, maxcsgheight=0)
orig.analyse()
orig.dump(msg="ORIG", detailed=True)

log.info("ORIGINAL:\n"+str(orig.txt))

orig.positivize()
log.info("POSITIVIZED:\n"+str(orig.txt))

obj = TreeBuilder.balance(orig)
log.info("BALANCED:\n"+str(obj.txt))

obj.meta.update(verbosity="1")
obj.dump(msg="BALANCED", detailed=True)
obj.dump_tboolean(name="esr")


CSG.Serialize([container, obj], outdir )

"""

* raytrace of balanced tree as expected
* polygonization yielding a blank 


~/opticks_refs/tboolean_esr_pole_artifact.png




"""

EOP
}






tboolean-positivize(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-positivize-(){ $FUNCNAME- | python $* ; } 
tboolean-positivize--(){ cat << EOP

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main

from opticks.analytic.csg import CSG  

args = opticks_main()

outdir = "$TMP/$FUNCNAME"
CSG.boundary = "$(tboolean-testobject)"
CSG.kwa = dict(poly="IM", resolution="40", verbosity="1" )

container = CSG("sphere",  param=[0,0,0,1000], boundary="$(tboolean-container)", poly="HY", level="4" )

lshape = "box"
#lshape = "sphere"
rshape = "sphere"

a_msg = "Positivized CSG difference, ie A - B ->  A*!B  "
a_left = CSG(lshape, param=[0,0,0,200], rotate="0,0,1,45"  )
a_right = CSG(rshape, param=[0,0,0,100],translate="0,0,200", scale="1,1,0.5" )
a = CSG.Difference(a_left, a_right, translate="-600,0,0" )
a.positivize()   
a.analyse()
log.info("A\n"+str(a.txt))


b_msg = "Standard CSG difference,  A - B "
b_left = CSG(lshape, param=[0,0,0,200], rotate="0,0,1,45" )
b_right = CSG(rshape, param=[0,0,0,100], translate="0,0,200", scale="1,1,0.5" )
b = CSG.Difference(b_left, b_right, translate="0,0,0" )
b.analyse()
log.info("B\n"+str(b.txt))


c_msg = "Standard CSG intersection with B complemented,  A*!B "
c_left = CSG(lshape, param=[0,0,0,200], rotate="0,0,1,45" )
c_right = CSG(rshape, param=[0,0,0,100], translate="0,0,200", scale="1,1,0.5" )
c_right.complement = True 
c = CSG.Intersection(c_left, c_right, translate="600,0,0" )
c.analyse()
log.info("C\n"+str(c.txt))


CSG.Serialize([container, a, b, c  ], outdir )
#CSG.Serialize([container, a ],  outdir )

log.info(r"""

Three CSG Differences, done in three different ways
=====================================================

Polygonizations and raytraces should all look the same.

A. %s
B. %s 
C. %s

* raytrace correctly looks the same with and without positivize, it is handling the complement

* FIXED : polygonization was ignoring the complement, got little intersection box not a difference,
          fixed by getting implicit function to honour the complement
           
* FIXED : positivize causing implicit mesher to complain of out-of-bounds, as the NNode::bbox
          was not honouring the complement

""" % (a_msg, b_msg, c_msg))

EOP
}









tboolean-bsu(){ TESTCONFIG=$(tboolean-boxsphere union)        tboolean-- ; }
tboolean-bsd(){ TESTCONFIG=$(tboolean-boxsphere difference)   tboolean-- ; }
tboolean-bsi(){ TESTCONFIG=$(tboolean-boxsphere intersection) tboolean-- ; }
tboolean-boxsphere-(){ $FUNCNAME- $* | python  ; } 
tboolean-boxsphere--(){ cat << EOP 
import math
from opticks.analytic.csg import CSG  

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20" )
  
radius = 200 
inscribe = 1.3*radius/math.sqrt(3)

box = CSG("box", param=[0,0,0,inscribe])


rtran = dict(translate="100,0,0")
sph = CSG("sphere", param=[0,0,0,radius], **rtran)

object = CSG("${1:-difference}", left=box, right=sph, boundary="$(tboolean-testobject)", poly="IM", resolution="50" )

CSG.Serialize([container, object], "$TMP/$FUNCNAME" )
EOP
}





tboolean-segment(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-segment-(){  $FUNCNAME- | python $* ; }
tboolean-segment--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.prism import make_segment
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )
container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )

phi0,phi1,sz,sr = 0,45,200,300 

if 0:
    planes, verts, bbox = make_segment(phi0,phi1,sz,sr)
    obj = CSG("segment")
    obj.planes = planes
    obj.param2[:3] = bbox[0]
    obj.param3[:3] = bbox[1]
else:
    obj = CSG.MakeSegment(phi0,phi1,sz,sr)
pass


obj.dump()

CSG.Serialize([container, obj], args.csgpath )
EOP
}


tboolean-cysegment(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-cysegment-(){  $FUNCNAME- | python $* ; }
tboolean-cysegment--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.prism import make_segment
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="40", verbosity="1", ctrl="0" )
container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )

phi0,phi1,sz,sr = 0,45,202,500*1.5

# sr needs to be significantly more than the rmax are cutting 
# to make a clean segment : otherwise the outside plane
# cuts into the tube ... 
# how much depends on the deltaphi 
 
planes, verts, bbox = make_segment(phi0,phi1,sz,sr)
seg = CSG("segment")
seg.planes = planes
seg.param2[:3] = bbox[0]
seg.param3[:3] = bbox[1]
  
ca = CSG("cylinder", param=[0,0,0,500], param1=[-100,100,0,0] )
cb = CSG("cylinder", param=[0,0,0,400], param1=[-101,101,0,0] )
cy = ca - cb 

obj = cy*seg

obj.dump()

CSG.Serialize([container, obj], args.csgpath )
EOP
}




tboolean-cyslab(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-cyslab-(){  $FUNCNAME- | python $* ; } 
tboolean-cyslab--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
from opticks.analytic.gdml import Primitive  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )
  
ca = CSG("cylinder", param=[0,0,0,500], param1=[-100,100,0,0] )
cb = CSG("cylinder", param=[0,0,0,400], param1=[-101,101,0,0] )
cy = ca - cb 


startphi = 90
deltaphi = 90 

phi0 = startphi
phi1 = startphi+deltaphi
dist = 500+1     # make it rmax + smth 

cyseg = Primitive.deltaphi_slab_segment( cy, phi0, phi1, dist)

if 0:
    xyzw_ = lambda phi:(np.cos(phi*np.pi/180.), np.sin(phi*np.pi/180.),0,0)
    sa = CSG("slab", param=xyzw_(phi0+90),param1=[0,501,0,0] )  # normalization done in NSlab.hpp/init_slab
    sb = CSG("slab", param=xyzw_(phi1-90),param1=[0,501,0,0] )  # normalization done in NSlab.hpp/init_slab
    # flipped +-90 as was chopping in opposite side of cylinder
    cysa = cy*sa 
    cysb = cy*sb 
    cyseg = cy*sa*sb 
pass

obj = cyseg

CSG.Serialize([container, obj], args.csgpath )

"""
         
         |
         |     .
         |     
         |  .
       \ | 
         +--------x
         |
         V


* slabs are defined by two planes with the same normal

* notice that phi slab slicing doesnt work when viewed precisely end on 
  from along the unbounded direction +-Z 

  Rather than the segment, see the whole cylinder.

  However its is difficult to get into that position without 
  using G/Composition/Home(H) and the axial view buttons(+Z/-Z)

  * unclear how this can be handled ?


"""


EOP
}




tboolean-undefined(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-undefined-(){  $FUNCNAME- | python $* ; } 
tboolean-undefined--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,4], boundary=args.container, poly="MC", nx="20" )
  
a = CSG.MakeUndefined(name="$FUNCNAME", src_type="hello")

CSG.Serialize([container, a], args.csgpath )


EOP
}





tboolean-torus(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-torus-(){  $FUNCNAME- | python $* ; } 
tboolean-torus--(){ cat << EOP 

from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
#CSG.kwa = dict(poly="MC", resolution="100")
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,400], boundary=args.container, poly="MC", nx="20" )
  
#a = CSG.MakeTorus(R=100, r=50)

a = CSG.MakeTorus(R=1, r=0.1)
a.scale = [100,100,100]

CSG.Serialize([container, a], args.csgpath )

"""

* ~/opticks_refs/tboolean_torus_orthographic_artifact.png 
* ~/opticks_refs/tboolean_torus_orthographic_artifact_extreme_near_axial.png


* perspective projection ring artifacts
* orthographic projection line artifacts

"""

EOP
}


tboolean-ellipsoid(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-ellipsoid-(){  $FUNCNAME- | python $* ; } 
tboolean-ellipsoid--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
from opticks.analytic.gdml import Primitive  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )
  
#a = CSG.MakeEllipsoid(axes=[100,200,100] )
#a = CSG.MakeEllipsoid(axes=[100,200,100], zcut1=-50, zcut2=50 )
#a = CSG.MakeEllipsoid(axes=[100,200,100], zcut1=-50  )
a = CSG.MakeEllipsoid(axes=[100,200,100], zcut1=-50  )

a.translate = [300,300,0]


#print a
#print "scale:", a.scale

CSG.Serialize([container, a], args.csgpath )

"""

* currently applying a scale to the ellipsoid will stomp on base scaling 
  of the sphere 


"""

EOP
}






tboolean-spseg(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-spseg-(){  $FUNCNAME- | python $* ; } 
tboolean-spseg--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
from opticks.analytic.gdml import Primitive  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )
  
a = CSG("sphere", param=[0,0,0,500] )
b = CSG("sphere", param=[0,0,0,490] )
d = a - b


phi0 =  0
phi1 = 90
dist = 500 + 1

s = Primitive.deltaphi_slab_segment( d, phi0, phi1, dist)


CSG.Serialize([container, s], args.csgpath )

"""
"""
EOP
}






tboolean-sphereslab(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; } 
tboolean-sphereslab-(){  $FUNCNAME- | python $* ; } 
tboolean-sphereslab--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(poly="IM", resolution="50")

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20" )
  
slab   = CSG("slab", param=[1,1,1,0],param1=[-500,100,0,0] )  # normalization done in NSlab.hpp/init_slab
sphere = CSG("sphere", param=[0,0,0,500] )

object = CSG("intersection", left=sphere, right=slab )

CSG.Serialize([container, object], args.csgpath )

"""

0. Works 

Why tboolean-sphere-slab raytrace is OK but tboolean-sphere-plane has directional visibility issues ?

* suspect due to "sub-objects must be closed" limitation of the  algorithm that 
  my CSG implementation is based upon: "Kensler:Ray Tracing CSG Objects Using Single Hit Intersections"

* http://xrt.wikidot.com/doc:csg

    "The [algorithm] computes intersections with binary CSG objects using the
    [nearest] intersection. Though it may need to do several of these per
    sub-object, the usual number needed is quite low. The only limitation of this
    algorithm is that the sub-objects must be closed, non-self-intersecting and
    have consistently oriented normals."

It appears can get away with infinite slab, which isnt bounded also, 
as only unbounded in "one" direction whereas half-space is much more
unbounded : in half the directions.


* note that without the caps enabled see nothing, because of this
  the user setting of endcap flags is now diabled : they are always 
  set to ON in NSlab.hpp


#SLAB_ACAP = 0x1 << 0
#SLAB_BCAP = 0x1 << 1
#flags = SLAB_ACAP | SLAB_BCAP
#flags = 0
#slab.param.view(np.uint32)[3] = flags 



"""
EOP
}


tboolean-sphereplane(){ TESTCONFIG=$($FUNCNAME-) tboolean-- ; }
tboolean-sphereplane-(){  $FUNCNAME- | python $* ; } 
tboolean-sphereplane--(){ cat << EOP 
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject 
CSG.kwa = dict(poly="IM", resolution="50", verbosity="1" )


container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="MC", nx="20", verbosity="0" )
  
plane  = CSG("plane",  param=[0,0,1,100], complement=False )
sphere = CSG("sphere", param=[0,0,0,500] )

object = CSG("intersection", left=sphere, right=plane )

CSG.Serialize([container, object], args.csgpath )

"""

With or without complement on the place get visbility wierdness, 
unbounded sub-objects such as planes are not valid CSG sub-objects within OpticksCSG 

0. Polygonization looks correct
1. only see the sphere surface from beneath the plane (ie beneath z=100)
2. only see the plane surface in shape of disc from above the plane 

"""
EOP
}

tboolean-boxplane(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; }
tboolean-boxplane-(){  $FUNCNAME- | python $* ; } 
tboolean-boxplane--(){ cat << EOP 
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20", verbosity="0" )

plane  = CSG("plane",  param=[0,0,1,100] )
box    = CSG("box", param=[0,0,0,200]  )
object = CSG("intersection", left=plane, right=box, boundary="$(tboolean-testobject)", poly="IM", resolution="50", verbosity="1" )

CSG.Serialize([container, object], "$TMP/$FUNCNAME" )

"""
#. Analogous issue to tboolean-sphere-plane
"""
EOP
}



tboolean-plane(){ TESTCONFIG=$($FUNCNAME- 2>/dev/null)    tboolean-- ; }
tboolean-plane-(){ $FUNCNAME- | python $* ; } 
tboolean-plane--(){ cat << EOP 
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20", verbosity="0" )

bigbox = CSG("box", param=[0,0,0,999] )
plane  = CSG("plane",  param=[0,0,1,100] )
object = CSG("intersection", left=plane, right=bigbox, boundary="$(tboolean-testobject)", poly="IM", resolution="50", verbosity="1" )

CSG.Serialize([container, object], "$TMP/$FUNCNAME" )

"""


#. An odd one, it appears OK in polygonization and raytrace : but it is breaking the rules,
   are using an unbounded sub-object (the plane) in intersection with the bigbox.

#. Actually the wierdness is there, just you there is no viewpoint from which you can see it. 
   Reducing the size of the bigbox to 500 allows it to manifest.

#. intersecting the plane with the container, leads to coincident surfaces and a flickery mess when 
   view from beneath the plane, avoided issue by intersecting instead with a bigbox slightly 
   smaller than the container

"""

EOP
}




tboolean-cy(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-cy-(){  $FUNCNAME- | python $* ; } 
tboolean-cy--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
CSG.kwa = dict(verbosity="1", poly="HY", resolution="4" )

container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="HY", resolution="4", verbosity="0" )

ra = 200 
z1 = -100
z2 = 100

#a = CSG("cylinder", param=[0,0,0,ra], param1=[z1,z2,0,0] )
#a = CSG("disc", param=[0,0,0,ra], param1=[-0.01,0.01,0,0] )
a = CSG("zsphere", param=[0,0,0,ra], param1=[z1,z2,0,0] )

ZSPHERE_QCAP = 0x1 << 1   # ZMAX
ZSPHERE_PCAP = 0x1 << 0   # ZMIN
flags = ZSPHERE_QCAP | ZSPHERE_PCAP
a.param2.view(np.uint32)[0] = flags 



obj = a 


CSG.Serialize([container, obj], args.csgpath )

"""

Currently need to kludge ncylinder::_par_pos_body to not join endcap and body to avoid:: 

    2017-06-28 12:36:37.651 INFO  [1576409] [>::add_parametric_primitive@181] NOpenMeshBuild<T>::add_parametric_primitive verbosity 1 ns 3 nu 32 nv 32 num_vert(raw) 3267 cfg.epsilon 1e-05 ctrl 0
    PolyMeshT::add_face: complex edge
    Assertion failed: (mesh.is_valid_handle(f)), function add_face_, file /Users/blyth/opticks/opticksnpy/NOpenMeshBuild.cpp, line 96.

"""


EOP
}







tboolean-cylinder(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-cylinder-(){  $FUNCNAME- | python $* ; } 
tboolean-cylinder--(){ cat << EOP 
import numpy as np
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()


outdir = "$TMP/$FUNCNAME"

CSG.boundary = "$(tboolean-testobject)"
CSG.kwa = dict(verbosity="1", poly="IM", resolution="30" )

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="MC", nx="20", verbosity="0" )

z1 = -100
z2 = 100
delta = 0.001 

z1d = z1 - delta
z2d = z2 + delta

a = CSG("cylinder", param=[0,0,0,400], param1=[z1,z2,0,0] )
b = CSG("cylinder", param=[0,0,0,100], param1=[z1d,z2d,0,0] )
c = CSG("cylinder", param=[200,200,0,100], param1=[z1d,z2d,0,0] )
d = CSG("cylinder", param=[-200,-200,0,100], param1=[z1d,z2d,0,0] )

obj = a - b - c - d 


#obj.rotate = "1,1,1,45"


CSG.Serialize([container, obj], outdir )

"""
Failed to reproduce the ESR speckles, from tboolean-sc

"""
EOP
}






tboolean-fromstring(){  TESTCONFIG=$($FUNCNAME-) tboolean-- ; }
tboolean-fromstring-(){ $FUNCNAME- | python ; }
tboolean-fromstring--(){ cat << EOP

from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
from opticks.analytic.gdml import Primitive

args = opticks_main()


so = Primitive.fromstring(r"""<tube aunit="deg" deltaphi="360" lunit="mm" name="AdPmtCollar0xc2c5260" rmax="106" rmin="105" startphi="0" z="12.7"/>""")

obj = so.as_ncsg() 
obj.boundary = "$(tboolean-testobject)"

container = CSG("box", param=[0,0,0,200], boundary="$(tboolean-container)", poly="IM", resolution="20" )

CSG.Serialize([container, obj], "$TMP/$FUNCNAME" )

EOP
}






tboolean-unbalanced(){  TESTCONFIG=$($FUNCNAME-)  tboolean-- ; }
tboolean-unbalanced-(){ $FUNCNAME- | python $*  ; }
tboolean-unbalanced--()
{
    local material=$(tboolean-material)
    local base=$TMP/$FUNCNAME 
    cat << EOP 
import math, logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

 
radius = 200 
inscribe = 1.3*radius/math.sqrt(3)

lbox = CSG("box",    param=[100,100,-100,inscribe])
lsph = CSG("sphere", param=[100,100,-100,radius])
left  = CSG("difference", left=lbox, right=lsph, boundary="$(tboolean-testobject)" )

right = CSG("sphere", param=[0,0,100,radius])

object = CSG("union", left=left, right=right, boundary="$(tboolean-testobject)", poly="IM", resolution="60" )
object.dump()

container = CSG("box", param=[0,0,0,1000], boundary="$(tboolean-container)", poly="IM", resolution="20")

CSG.Serialize([container, object], "$base" )


EOP
}





tboolean-deep(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-deep-(){ local n=14 ; tboolean-gdml- $TMP/${FUNCNAME}$n --gsel $($FUNCNAME- $n) --gmaxdepth 1 ; }
tboolean-deep--(){  $FUNCNAME- | sed -n ${1:-1}p ; }
tboolean-deep---(){ cat << EOD
/dd/Geometry/PoolDetails/lvNearTopCover0x
/dd/Geometry/AdDetails/lvRadialShieldUnit0x
/dd/Geometry/AdDetails/lvTopESR0x
/dd/Geometry/AdDetails/lvTopRefGap0x
/dd/Geometry/AdDetails/lvTopReflector0x
/dd/Geometry/AdDetails/lvBotESR0x
/dd/Geometry/AdDetails/lvBotRefGap0x
/dd/Geometry/AdDetails/lvBotReflector0x
/dd/Geometry/AdDetails/lvSstTopCirRibBase0x
/dd/Geometry/CalibrationSources/lvLedSourceAssy0x
/dd/Geometry/CalibrationSources/lvGe68SourceAssy0x
/dd/Geometry/CalibrationSources/lvAmCCo60SourceAssy0x
/dd/Geometry/OverflowTanks/lvLsoOflTnk0x
/dd/Geometry/OverflowTanks/lvGdsOflTnk0x
/dd/Geometry/OverflowTanks/lvOflTnkContainer0x
/dd/Geometry/PoolDetails/lvTablePanel0x
/dd/Geometry/Pool/lvNearPoolIWS0x
/dd/Geometry/Pool/lvNearPoolCurtain0x
/dd/Geometry/Pool/lvNearPoolOWS0x
/dd/Geometry/Pool/lvNearPoolLiner0x
/dd/Geometry/Pool/lvNearPoolDead0x
/dd/Geometry/RadSlabs/lvNearRadSlab90x
EOD
}

tboolean-deep-notes(){ cat << EON

n = {}  
n["/dd/Geometry/PoolDetails/lvNearTopCover0x"]="1:flat lozenge"
n["/dd/Geometry/AdDetails/lvRadialShieldUnit0x"]="2:tambourine with 6 holes, potential wierdness : from inside dont see the caps, coincidence perhaps"
n["/dd/Geometry/AdDetails/lvTopESR0x"]="3:evaluative_csg tranOffset 0 numParts 1023 perfect tree height 9 exceeds current limit"
n["/dd/Geometry/AdDetails/lvBotReflector0x"]="8:  disc with 4 slots, but thats partial: evaluative_csg tranOffset 0 numParts 511 perfect tree height 8 exceeds current limit "
n["/dd/Geometry/AdDetails/lvSstTopCirRibBase0x"]="9:  cross cut cylinder, obvious coincidence speckling in the cuts"


EON
}


tboolean-0(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-0-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel 0 ; }
tboolean-0-deserialize(){ VERBOSITY=0 lldb NCSGDeserializeTest -- $TMP/tboolean-0- ; }
tboolean-0-polygonize(){  VERBOSITY=0 lldb NCSGPolygonizeTest  -- $TMP/tboolean-0- ; }

tboolean-gds0(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-gds0-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AD/lvGDS0x ; }

tboolean-oav(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-oav-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AD/lvOAV0x ; }

tboolean-iav(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-iav-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AD/lvIAV0x ; }

tboolean-sst(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-sst-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AD/lvSST0x --gmaxdepth 3 ; }








tboolean-gds(){ TESTCONFIG=$($FUNCNAME-) tboolean-- $* ; }
tboolean-gds-(){ $FUNCNAME- | python $* ; } 
tboolean-gds--(){ cat << EOP


# In [15]: c.mesh.csg.dump_tboolean("gds")

outdir = "$TMP/$FUNCNAME"
obj_ = "Acrylic//perfectAbsorbSurface/GdDopedLS"


con_ = "$(tboolean-container)"

import logging
log = logging.getLogger(__name__)
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  
args = opticks_main()

CSG.boundary = obj_
CSG.kwa = dict(verbosity="0", poly="IM", resolution="20")


a = CSG("cylinder", param = [0.000,0.000,0.000,1550.000],param1 = [-1535.000,1535.000,0.000,0.000])
b = CSG("cone", param = [1520.000,3070.000,75.000,3145.729],param1 = [0.000,0.000,0.000,0.000])
c = CSG("cylinder", param = [0.000,0.000,0.000,75.000],param1 = [3145.729,3159.440,0.000,0.000])
bc = CSG("union", left=b, right=c)
bc.transform = [[1.000,0.000,0.000,0.000],[0.000,1.000,0.000,0.000],[0.000,0.000,1.000,0.000],[0.000,0.000,-1535.000,1.000]]

abc = CSG("union", left=a, right=bc)

# photons formed maltese cross, until upped timemax from 10ns to 20ns

#abc.transform = [[0.543,-0.840,0.000,0.000],[0.840,0.543,0.000,0.000],[0.000,0.000,1.000,0.000],[-18079.453,-799699.438,-7100.000,1.000]]
abc.transform = [[0.543,-0.840,0.000,0.000],[0.840,0.543,0.000,0.000],[0.000,0.000,1.000,0.000],[0,0,1000.,1.000]]

obj = abc


con = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary=con_ , poly="HY", level="5" )
CSG.Serialize([con, obj], outdir )


EOP
}









tboolean-pmt(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-pmt-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/PMT/lvPmtHemi0x ; }




### trapezoid examples

tboolean-sstt(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-sstt-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AdDetails/lvSstTopRadiusRib0x ; }
# contains a trapezoid as part of, thats the real skinny one 

tboolean-sstt2(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-sstt2-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/AdDetails/lvSstInnVerRibBase0x ; }


## ntc: flat lozenge shape, a deep CSG tree

tboolean-ntc(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-ntc-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel /dd/Geometry/PoolDetails/lvNearTopCover0x ; }


tboolean-p0(){  TESTCONFIG=$($FUNCNAME- 2>/dev/null) && tboolean--  ; }
tboolean-p0-(){ tboolean-gdml- $TMP/$FUNCNAME --gsel  /dd/Geometry/AdDetails/lvOcrGdsInIav0x ; }







tboolean-gdml-()
{      
    local csgpath=$1
    shift
    python $(tboolean-gdml-translator) \
          --csgpath $csgpath \
          --container $(tboolean-container)  \
          --testobject $(tboolean-testobject) \
          $*
}
tboolean-gdml-translator(){ echo $(opticks-home)/analytic/translate_gdml.py ; }

tboolean-gdml-check(){ tboolean-gdml- 2> /dev/null ; }
tboolean-gdml-edit(){ vi $(tboolean-gdml-translator)   ; }
tboolean-gdml-scan(){ SCAN="0,0,127.9,0,0,1,0,0.1,0.01" NCSGScanTest $TMP/tboolean-gdml-/1 ; }
tboolean-gdml-ip(){  tboolean-cd ; ipython tboolean_gdml.py -i ; }



tboolean-dd(){          TESTCONFIG=$(tboolean-dd- 2>/dev/null)     tboolean-- $* ; }
tboolean-dd-()
{       
    python $(tboolean-dir)/tboolean_dd.py \
          --csgpath $TMP/$FUNCNAME \
          --container $(tboolean-container)  \
          --testobject $(tboolean-testobject)  

    # got too long for here-string  so broke out into script
}
tboolean-dd-check(){ tboolean-dd- 2> /dev/null ; }
tboolean-dd-edit(){ vi $(tboolean-dir)/tboolean_dd.py  ; }
tboolean-dd-scan(){ SCAN="0,0,127.9,0,0,1,0,0.1,0.01" NCSGScanTest $TMP/tboolean-dd-/1 ; }




tboolean-interlocked(){  TESTCONFIG=$($FUNCNAME-) tboolean-- ; }
tboolean-interlocked-(){ $FUNCNAME- | python $* ; }
tboolean-interlocked--()
{
    cat << EOP 
import math
from opticks.ana.base import opticks_main
from opticks.analytic.csg import CSG  

args = opticks_main(csgpath="$TMP/$FUNCNAME")

CSG.boundary = args.testobject
  
radius = 200 
inscribe = 1.3*radius/math.sqrt(3)

lbox = CSG("box",    param=[100,100,-100,inscribe])
lsph = CSG("sphere", param=[100,100,-100,radius])
left  = CSG("difference", left=lbox, right=lsph )

rbox = CSG("box",    param=[0,0,100,inscribe])
rsph = CSG("sphere", param=[0,0,100,radius])

tran = dict(translate="0,0,100", rotate="1,1,1,45", scale="1,1,1.5" )
right = CSG("difference", left=rbox, right=rsph, **tran)

#dcs = dict(poly="DCS", nominal="7", coarse="6", threshold="1", verbosity="0")
im = dict(poly="IM", resolution="64", verbosity="0", ctrl="0" ) #seeds = "100,100,-100,0,0,300"
obj = CSG("union", left=left, right=right, **im )
#obj.translate = "0,-300,0"
obj.meta.update(gpuoffset="0,600,0")


#mc = dict(poly="MC", nx="20")
container = CSG("box", param=[0,0,0,1000], boundary=args.container, poly="IM", resolution="20" )

CSG.Serialize([container, obj], args.csgpath )

EOP
}



tboolean-testconfig()
{
    # this is the default TESTCONFIG
    tboolean-boxsphere-
}


