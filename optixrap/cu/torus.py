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

   (x*x + y*y + z*z + R*R + r*r)^2 = 4R*R(x*x + y*y)  


::

    In [56]: ex.subs([(ox,0),(oy,0),(oz,0),(sx,1),(sy,0),(sz,0)])
    Out[56]: -4*R**2*t**2 + (R**2 - r**2 + t**2)**2

    In [57]: factor(ex.subs([(ox,0),(oy,0),(oz,0),(sx,1),(sy,0),(sz,0)]))
    Out[57]: (-R + r - t)*(-R + r + t)*(R + r - t)*(R + r + t)

    In [60]: solve(ex.subs([(ox,0),(oy,0),(oz,0),(sx,1),(sy,0),(sz,0)]), t )
    Out[60]: [-R - r, -R + r, R - r, R + r]

    In [62]: solve(ex.subs([(ox,0),(oy,0),(oz,0),(sx,0),(sy,0),(sz,1)]), t )
    Out[62]: [-sqrt(-R**2 + r**2), sqrt(-R**2 + r**2)]




Issue is very wide scale range in coeffs causing numerical issue...

Can rotation + quadrant symmetry be exploited ?  

::

    In [52]: circle.subs(ray)
    Out[52]: -B**2 + (oz + sz*t)**2 + (-A + ox + sx*t)**2

    In [54]: get_coeff( circle.subs(ray), t, 3 )
    Out[54]: 
    [A**2 - 2*A*ox - B**2 + ox**2 + oz**2,
     -2*(A*sx - ox*sx - oz*sz),
     sx**2 + sz**2]


    cir_ray = get_coeff( circle.subs(ray), t, 3 )

    discr = cir_ray[1]**2 -4*cir_ray[0]*cir_ray[2]


Contrast with sphere intersecton

 // Solving:  d t^2 + 2 b t +  c = 0 


 601     float c = dot(O, O)-radius*radius;
 600     float b = dot(O, D);
 602     float d = dot(D, D);

 603 
 604     float disc = b*b-d*c;


* torus axis (0,0,1) and center (0,0,0) torus plane z=0 ie xy 
* virtual ray cone axis (0,0,1) and apex (0,0,za) 

  * huh : need the ray to intersect with axis line at apex

"""

from sympy import symbols, Pow, sqrt, Matrix, sin, cos
from coeff import get_coeff, subs_coeff, print_coeff, expr_coeff

isq3 = 1/sqrt(3) 



def torus_0():

    x,y,z,r,R = symbols("x y z r R")

    expr = (x*x + y*y + z*z + R*R - r*r)**2 - 4*R*R*(x*x + y*y)

    ox,oy,oz,t,sx,sy,sz,SS,OO,OS = symbols("ox oy oz t sx sy sz SS OO OS")

    ray = [(x,ox+t*sx),(y,oy+t*sy),(z,oz+t*sz)]
    simp = [(sx**2+sy**2+sz**2,SS),(ox**2+oy**2+oz**2,OO),(ox*sx+oy*sy+oz*sz,OS)]

    expr2 = expr.subs(ray+simp)

    x_expr2 = expand(expr2)
    t_expr2 = collect(x_expr2, t )

    c = get_coeff( expr2, t, 5 )

    subs_coeff(c, simp)
    print_coeff(c) 

    return expr2, c



def torus_1():
    """
    http://www.cosinekitty.com/raytrace/chapter13_torus.html

    Symbolic manipulations matching cosinekitty example

    * simplifying coeffs separately then putting 
      back together seems an effective approach

    """
    x,y,z,A,B = symbols("x y z A B")

    _sq_lhs = (x*x + y*y + z*z + A*A - B*B) 
    _rhs = 4*A*A*(x*x + y*y)
   
    ox,oy,oz,t,sx,sy,sz,SS,OO,OS = symbols("ox oy oz t sx sy sz SS OO OS")

    ray = [(x,ox+t*sx),(y,oy+t*sy),(z,oz+t*sz)]
    simp0 = [(sx**2+sy**2+sz**2,SS),(ox**2+oy**2+oz**2,OO),(ox*sx+oy*sy+oz*sz,OS)]


    G,H,I,J,K,L = symbols("G H I J K L")


    exG = 4*A*A*(sx*sx+sy*sy) 
    exH = 8*A*A*(ox*sx+oy*sy)
    exI = 4*A*A*(ox*ox+oy*oy)
    exJ = sx*sx+sy*sy+sz*sz
    exK = 2*(ox*sx+oy*sy+oz*sz)
    exL = ox*ox+oy*oy+oz*oz+A*A-B*B
  
    simp1 = [
        (exG, G),
        (exH, H),
        (exI, I),
        (exJ, J),
        (exK/2, K/2),   # fails to sub with 2 on other side
        (exL, L)] 

    sq_lhs = _sq_lhs.subs(ray + simp0)
    rhs = _rhs.subs(ray + simp0)


    cl = get_coeff(sq_lhs, t, 3)
    cr = get_coeff(rhs, t, 3)

    subs_coeff(cl, simp1)
    subs_coeff(cr, simp1)

    print_coeff(cl, "cl")
    print_coeff(cr, "cr")

    SQ_LHS = expr_coeff(cl, t)
    RHS = expr_coeff(cr, t)

    ex = Pow(SQ_LHS, 2) - RHS

    c = get_coeff(ex, t, 5)

    print_coeff(c, "c")


    exc = range(5)
    exc[4] = exJ**2
    exc[3] = 2*exJ*exK
    exc[2] = -exG + 2*exJ*exL + exK**2
    exc[1] = -exH + 2*exK*exL 
    exc[0] = -exI + exL**2

    subs = {}
    subs["O->X+"] = [(ox,0),(oy,0),(oz,0),(sx,1),(sy,0),(sz,0)]
    subs["zenith->-Z"] = [(ox,0),(oy,0),(oz,A),(sx,0),(sy,0),(sz,-1)]
    subs["corner"] = [(ox,A),(oy,A),(oz,A),(sx,-isq3),(sy,-isq3),(sz,-isq3)]


    radii = [(A, 500), (B, 50)]

    for key in subs:
        print "\n\n", key
        for j, exc_ in enumerate(exc):
            print "\n", j, exc_
            print exc_.subs(subs[key])
            print exc_.subs(subs[key]).subs(radii)
    



    return ex, cl


if __name__ == '__main__':
    pass
    #ex, c = torus_0()
    #ex, c = torus_1()

    """
    Torus rotational symmetry -> its a sphere swept around the axis.
    Does that mean can do a bounding intersection test of a rotated 
    ray and a sphere ? 

    Can consider intersection of 
    a ray rotated around torus axis (0,0,1) to put it into y=0 plane.

    * oz -> oz_rot   (stays same)   
    * ox^2 + oy^2 -> ox_rot^2 + oy_rot^2   (radial invariant + by design oy_rot = 0)            
    * ox_rot = sqrt(ox^2+oy^2)  ...  ie rotated x is at the xy radius

    Hmm can easily rotate to put ray origin into the y=0 plane, but  
    ray direction will not generally be in that plane ? 
    It will only be in that plane if the ray happens to intersect with the 
    torus axis.   

    Normal to the test y=0 plane is [1,0,0], 

    It is the ray direction that needs to be rotated to 
    be within the test plane 

    * [sx,sy,sz].[1,0,0] = cos(phi)


    * https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

    * vrot = v*cos(phi) + (k x v)*sin(phi) + k(k.v)(1-cos(phi))

    *  

    * u X v = ( u2 v3 - u3 v2, u3 v1 - u1 v3 , u1 v2 - u2 v1 )
    
    * u = k = [0,0,1]    u1=u2=0, u3=1   
    * u X k = ( -v2, v1, 0 )

    * Rotate [sx,sy,sz] and [ox,oy,oz] by phi about axis [0,0,1]
    
    * srot = [sx cos(phi) - sy sin(phi), sy cos(phi) + sx sin(phi) , sz ] 
    * orot = [ox cos(phi) - oy sin(phi), oy cos(phi) + ox sin(phi) , oz ] 

    
    * srot.[1,0,0] = 1  ->   sx/sy = tan(phi)    phi=atan2(sx,sy)  



    """ 
    t,phi = symbols("t,phi")


    zrot = Matrix([[cos(phi),-sin(phi),0,0],[sin(phi),cos(phi),0,0],[0,0,1,0],[0,0,0,1]])
    sx,sy,sz = symbols("sx,sy,sz")
    ox,oy,oz = symbols("ox,oy,oz")
    o = Matrix([ox,oy,oz,1])
    s = Matrix([sx,sy,sz,0])

    
    
    srot = s*cos(phi) +  

    r = o+t*s





