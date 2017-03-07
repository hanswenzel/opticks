#!/usr/bin/env python
"""
A slavish translation back to python from the CUDA/OptiX implementation 
of iterative CSG in intersect_boolean.h  in slavish_intersect and
a recursive riff on that in slavish_intersect_recursive.

Objectives:

* instrument code paths and make visual rep of them, especially 
  the non-bileaf looping ones that do the reiteration backtracking 

* investigate pop empty errors seen with the OptiX imp

* extend code to handle partBuffer with empty nodes ie 
  levelorder serializations of non-perfect trees 

* extend to handle op-node transforms 

* check csg tree serialization/persisting and use to 
  bring python trees to GPU and vice versa
  
"""

import numpy as np
import logging
log = logging.getLogger(__name__)

from intersectTest import T

from opticks.bin.ffs import ffs_, clz_
from intersect import Ray, intersect_primitive
from node import Node, root4, lrsph_u
from node import Q0, Q1, Q2, Q3, X, Y, Z, W

from opticks.optixrap.cu.boolean_solid_h import CTRL_RETURN_MISS, CTRL_RETURN_A, CTRL_RETURN_B, CTRL_RETURN_FLIP_B, CTRL_LOOP_A, CTRL_LOOP_B
from opticks.optixrap.cu.boolean_solid_h import ERROR_LHS_POP_EMPTY, ERROR_RHS_POP_EMPTY, ERROR_LHS_END_NONEMPTY, ERROR_RHS_END_EMPTY, ERROR_BAD_CTRL, ERROR_LHS_OVERFLOW, ERROR_RHS_OVERFLOW, ERROR_LHS_TRANCHE_OVERFLOW
from opticks.optixrap.cu.boolean_h import desc_state, Enter, Exit, Miss


def CSG_CLASSIFY(ise, dir_, tmin):
     assert ise.shape == (4,)
     if ise[W] > tmin:
         return Enter if np.dot( ise[:3], dir_ ) < 0 else Exit 
     else:
         return Miss


TRANCHE_STACK_SIZE = 4
CSG_STACK_SIZE = 4

class Error(Exception):
    def __init__(self, msg):
        super(Error, self).__init__(msg)


class CSG_(object):
    def __init__(self):
        self.data = np.zeros( (CSG_STACK_SIZE, 4,4), dtype=np.float32 )
        self.curr = -1

    def pop(self):
        if self.curr < 0:
            raise Error("CSG_ pop from empty")
        else:
            ret = self.data[self.curr] 
            self.curr -= 1 
        pass
        return ret

    def push(self, ise):
        if self.curr+1 >= CSG_STACK_SIZE:
            raise Error("CSG_ push overflow")
        else:
            self.curr += 1 
            self.data[self.curr] = ise        
        pass


def TRANCHE_PUSH0( _stacku, _stackf, stack, valu, valf ):
    stack += 1 
    _stacku[stack] = valu 
    _stackf[stack] = valf
    return stack 

def TRANCHE_PUSH( _stacku, _stackf, stack, valu, valf ):
    if stack+1 >= TRANCHE_STACK_SIZE:
        raise Error("Tranche push overflow")
    else:
        stack += 1 
        _stacku[stack] = valu 
        _stackf[stack] = valf
    pass
    return stack 


def TRANCHE_POP0( _stacku, _stackf, stack):
    valu = _stacku[stack] 
    valf = _stackf[stack] 
    stack -= 1 
    return stack, valu, valf


POSTORDER_SLICE = lambda begin, end:((((end) & 0xffff) << 16) | ((begin) & 0xffff)  )

POSTORDER_BEGIN = lambda tmp:( (tmp) & 0xffff )
POSTORDER_END = lambda tmp:( (tmp) >> 16 )
POSTORDER_NODE = lambda postorder, i: (((postorder) & (0xF << (i)*4 )) >> (i)*4 )



# generated from /Users/blyth/opticks/optixrap/cu by boolean_h.py on Sat Mar  4 20:37:03 2017 
packed_boolean_lut_ACloser = [ 0x22121141, 0x00014014, 0x00141141, 0x00000000 ]
packed_boolean_lut_BCloser = [ 0x22115122, 0x00022055, 0x00133155, 0x00000000 ]
             
def boolean_ctrl_packed_lookup(operation, stateA, stateB, ACloser ):
    lut = packed_boolean_lut_ACloser if ACloser else  packed_boolean_lut_BCloser 
    offset = 3*stateA + stateB ;
    return (lut[operation] >> (offset*4)) & 0xf if offset < 8 else CTRL_RETURN_MISS 

propagate_epsilon = 1e-3


def slavish_intersect_recursive(partBuffer, ray):
    """
    """
    numParts = len(partBuffer)  
    partOffset = 0 
    fullHeight = ffs_(numParts + 1) - 2
    height = fullHeight - 1
    numInternalNodes = (0x1 << (1+height)) - 1

    LHS, RHS = 0, 1
    MISS, LEFT, RIGHT, RFLIP = 0,1,2,3

    def slavish_intersect_r(nodeIdx, tmin):
        """  
        :param nodeIdx: 1-based levelorder tree index
        """
        leftIdx = nodeIdx*2 
        rightIdx = nodeIdx*2 + 1
        bileaf = leftIdx > numInternalNodes

        isect = np.zeros( [4, 4, 4], dtype=np.float32 )
        tX_min = np.zeros( 2, dtype=np.float32 )
        tX_min[LHS] = tmin
        tX_min[RHS] = tmin
        x_state = np.zeros( 2, dtype=np.uint32 )

        if bileaf:
            isect[LEFT,0,W] = 0.
            isect[RIGHT,0,W] = 0.
            isect[LEFT] = intersect_primitive( Node.fromPart(partBuffer[partOffset+leftIdx-1]), ray, tX_min[LHS])
            isect[RIGHT] = intersect_primitive( Node.fromPart(partBuffer[partOffset+rightIdx-1]), ray, tX_min[RHS])
        else:
            isect[LEFT]  = slavish_intersect_r( leftIdx, tX_min[LHS] )
            isect[RIGHT] = slavish_intersect_r( rightIdx, tX_min[RHS] )
        pass
        x_state[LHS] = CSG_CLASSIFY( isect[LEFT][0], ray.direction, tX_min[LHS] )
        x_state[RHS] = CSG_CLASSIFY( isect[RIGHT][0], ray.direction, tX_min[RHS] )

        operation = partBuffer[nodeIdx-1,Q1,W].view(np.uint32)
        ctrl = boolean_ctrl_packed_lookup( operation, x_state[LHS], x_state[RHS], isect[LEFT][0][W] <= isect[RIGHT][0][W] )
        side = ctrl - CTRL_LOOP_A 
 
        loop = -1
        while side > -1 and loop < 10:
            loop += 1
            assert side in [LHS, RHS], "ctrl:%d side:%d not in LHS:%d RHS:%d CTRL_LOOP_A:%d " % (ctrl, side, LHS, RHS, CTRL_LOOP_A)
            THIS = side + LEFT 
            assert THIS in [LEFT, RIGHT]
            tX_min[side] = isect[THIS][0][W] + propagate_epsilon 

            if bileaf:               
                isect[THIS] = intersect_primitive( Node.fromPart(partBuffer[partOffset+leftIdx+side-1]) , ray, tX_min[side])
            else:
                isect[THIS] = slavish_intersect_r( leftIdx+side, tX_min[side] )
            pass
 
            x_state[side] = CSG_CLASSIFY( isect[THIS][0], ray.direction, tX_min[side] )
            ctrl = boolean_ctrl_packed_lookup( operation, x_state[LHS], x_state[RHS], isect[LEFT][0][W] <= isect[RIGHT][0][W] )
            side = ctrl - CTRL_LOOP_A    ## NB possibly changed side            
        pass   # side loop

        isect[RFLIP] = isect[RIGHT]
        isect[RFLIP,0,X] = -isect[RFLIP,0,X]
        isect[RFLIP,0,Y] = -isect[RFLIP,0,Y]
        isect[RFLIP,0,Z] = -isect[RFLIP,0,Z]
 
        assert ctrl < CTRL_LOOP_A 
        return isect[ctrl]
    pass
    return slavish_intersect_r( 1, ray.tmin )






def slavish_intersect(partBuffer, ray):
    postorder_sequence = [ 0x1, 0x132, 0x1376254, 0x137fe6dc25ba498 ] 
    ierr = 0
    abort_ = False

    partOffset = 0 
    numParts = len(partBuffer)
    primIdx = -1

    fullHeight = ffs_(numParts + 1) - 2 
    height = fullHeight - 1 

    postorder = postorder_sequence[height] 
    numInternalNodes = (0x1 << (1+height)) - 1       

    _tmin = np.zeros( (TRANCHE_STACK_SIZE), dtype=np.float32 )
    _tranche = np.zeros( (TRANCHE_STACK_SIZE), dtype=np.uint32 )
    tranche = -1

    LHS, RHS = 0, 1
    csg_ = [CSG_(), CSG_()] 
    lhs = csg_[LHS] 
    rhs = csg_[RHS] 
    lhs.curr = -1
    rhs.curr = -1

    MISS, LEFT, RIGHT, RFLIP = 0,1,2,3
    isect = np.zeros( [4, 4, 4], dtype=np.float32 )

    tranche = TRANCHE_PUSH0( _tranche, _tmin, tranche, POSTORDER_SLICE(0, numInternalNodes), ray.tmin )

    while tranche >= 0:
        tranche, tmp, tmin = TRANCHE_POP0( _tranche, _tmin,  tranche )
        begin = POSTORDER_BEGIN(tmp)
        end = POSTORDER_END(tmp)

        i = begin
        while i < end:

            nodeIdx = POSTORDER_NODE(postorder, i)
            leftIdx = nodeIdx*2 
            rightIdx = nodeIdx*2 + 1
            depth = 32 - clz_(nodeIdx)-1
            subNodes = (0x1 << (1+height-depth)) - 1
            halfNodes = (subNodes - 1)/2 
            bileaf = leftIdx > numInternalNodes

            operation = partBuffer[nodeIdx-1,Q1,W].view(np.uint32)

            tX_min = np.zeros( 2, dtype=np.float32 )
            tX_min[LHS] = tmin
            tX_min[RHS] = tmin

            x_state = np.zeros( 2, dtype=np.uint32 )

            if bileaf:
                isect[LEFT,0,W] = 0.
                isect[RIGHT,0,W] = 0.
                isect[LEFT] = intersect_primitive( Node.fromPart(partBuffer[partOffset+leftIdx-1]), ray, tX_min[LHS])
                isect[RIGHT] = intersect_primitive( Node.fromPart(partBuffer[partOffset+rightIdx-1]), ray, tX_min[RHS])
            else:
                try:
                    isect[LEFT] = lhs.pop()
                except Error:
                    ierr |= ERROR_LHS_POP_EMPTY
                    abort_ = True
                    break
                pass
                try:
                    isect[RIGHT] = rhs.pop()
                except Error:
                    ierr |= ERROR_RHS_POP_EMPTY
                    abort_ = True
                    break
                pass
            pass

            x_state[LHS] = CSG_CLASSIFY( isect[LEFT][0], ray.direction, tX_min[LHS] )
            x_state[RHS] = CSG_CLASSIFY( isect[RIGHT][0], ray.direction, tX_min[RHS] )

            ctrl = boolean_ctrl_packed_lookup( operation, x_state[LHS], x_state[RHS], isect[LEFT][0][W] <= isect[RIGHT][0][W] )
            side = ctrl - CTRL_LOOP_A 
            reiterate = False
            loop = -1

            while side > -1 and loop < 10:
                loop += 1
                assert side in [LHS, RHS], "ctrl:%d side:%d not in LHS:%d RHS:%d CTRL_LOOP_A:%d " % (ctrl, side, LHS, RHS, CTRL_LOOP_A)
                THIS = side + LEFT 
                assert THIS in [LEFT, RIGHT]
                tX_min[side] = isect[THIS][0][W] + propagate_epsilon 

                if bileaf:               
                    isect[THIS] = intersect_primitive( Node.fromPart(partBuffer[partOffset+leftIdx+side-1]) , ray, tX_min[side])
                else:
                    try:
                        isect[THIS] = csg_[side].pop()
                    except Error:
                        ierr |= ERROR_POP_EMPTY
                        abort_ = True
                        break
                    pass
                pass
                x_state[side] = CSG_CLASSIFY( isect[THIS][0], ray.direction, tX_min[side] )
                ctrl = boolean_ctrl_packed_lookup( operation, x_state[LHS], x_state[RHS], isect[LEFT][0][W] <= isect[RIGHT][0][W] )
                side = ctrl - CTRL_LOOP_A    ## NB changed side            

                if side > -1 and not bileaf:
                    other = 1 - side
                    tX_min[side] = isect[THIS][0][W] + propagate_epsilon 
                    csg_[other].push( isect[other+LEFT] ) 
                    subtree = POSTORDER_SLICE(i-2*halfNodes, i-halfNodes) if side == LHS else POSTORDER_SLICE(i-halfNodes, i) 
     
                    tranche = TRANCHE_PUSH( _tranche, _tmin, tranche, POSTORDER_SLICE(i, numInternalNodes), tmin )
                    tranche = TRANCHE_PUSH( _tranche, _tmin, tranche, subtree, tX_min[side] )
                    reiterate = True
                    break
                pass
            pass   # side loop

            if reiterate or abort_:
                break
            pass
       
            isect[RFLIP,0,X] = -isect[RIGHT,0,X]
            isect[RFLIP,0,Y] = -isect[RIGHT,0,Y]
            isect[RFLIP,0,Z] = -isect[RIGHT,0,Z]
            isect[RFLIP,0,W] = isect[RIGHT,0,W]
 
            assert ctrl < CTRL_LOOP_A 
            result = isect[ctrl] 
            nside = LHS if nodeIdx % 2 == 0 else RHS  # even in left

            csg_[nside].push(result)

            i += 1   # next postorder node in the tranche
        pass         # end traversal loop
        # if abort_:break  ## ???
    pass             # end tranch loop


    LHS_curr = csg_[LHS].curr
    RHS_curr = csg_[RHS].curr 

    ierr |=  ERROR_LHS_END_NONEMPTY if LHS_curr != -1 else 0
    ierr |=  ERROR_RHS_END_EMPTY    if RHS_curr != 0 else 0


    if RHS_curr == 0 and ierr == 0:
        ret = csg_[RHS].data[RHS_curr]
    else:
        ret = None ## hmm need some kind error holding ret
    pass
    if ierr is not 0:
        print "ierr: %x " % ierr
    pass
    return ret 



if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    from nodeRenderer import Renderer
    import matplotlib.pyplot as plt

    plt.rcParams['figure.figsize'] = 18,10.2 
    plt.ion()
    plt.close("all")

    root = root4
    #root = lrsph_u

    tst = T(root, source="aringlight")

    ray = Ray()
    ray.tmin = 0 

    partBuffer = Node.serialize( root )
    iterative_ = lambda ray:slavish_intersect(partBuffer, ray) 
    recursive_ = lambda ray:slavish_intersect_recursive(partBuffer, ray) 

    rr = [0,1]

    tst.run( [iterative_, recursive_], rr )

    fig = plt.figure()
    ax1 = fig.add_subplot(1,2,1, aspect='equal')
    ax2 = fig.add_subplot(1,2,2, aspect='equal')
    axs = [ax1,ax2]

    tst.plot_intersects( axs=axs, rr=rr, normal=False, origin=False, rayline=False)

    for ax in axs:
        rdr = Renderer(ax)
        rdr.render(tst.root)
        #rdr.limits(400,400)
        ax.axis('auto')
    pass

    fig.suptitle(tst.suptitle, horizontalalignment='left', family='monospace', fontsize=10, x=0.1, y=0.99) 
    fig.show()






 
