#!/usr/bin/env python

import sys
from collections import defaultdict, OrderedDict
FMT = "// %80s : %s " 
COUNT = defaultdict(lambda:0)



class Loc(object):
    """
    Identifies a calling python function from its pframe
    """

    @classmethod
    def Tag(cls, func, name):
        if func is None:return None
        pass
        identity = "%s.%s" % (name, func  )
        global COUNT 
        idx = COUNT[identity] 
        tag = "%s.[%.2d]" % ( func, idx)
        COUNT[identity] += 1 
        return tag, idx

    def __init__(self, pframe, name):
        """
        :param pframe: python frame of caller 
        :param name: module __name__ of caller
        """
        self.name = name 
        if pframe is not None:
            func = pframe.f_code.co_name
            doc = pframe.f_code.co_consts[0]
            doclines = filter(None, doc.split("\n")) if doc is not None else []
            label = doclines[0].lstrip() if len(doclines) > 0 else "no-docstring-label"  # 1st line of docstring
            tag, idx = self.Tag(func, name)
            hdr = FMT % (tag, label) 
        else:
            func = None
            label = "-"
            tag = None
            idx = None
            hdr = None
        pass
        self.func = func
        self.label = label
        self.tag = tag
        self.idx = idx
        self.hdr = hdr

    def __repr__(self):
        disp_ = lambda k:" %10s : %s " % ( k, getattr(self, k, None)) 
        return "\n".join(map(disp_, "name func label tag idx hdr".split()))



def test_Loc():
    """
    First Line of docstring becomes label
    """

    loc = Loc(sys._getframe(), __name__)
    print loc



def test_Introspect_(pframe):
    func = pframe.f_code.co_name
    doc = pframe.f_code.co_consts[0]

    doclines = filter(None, doc.split("\n"))
    label = doclines[0].lstrip() if len(doclines) > 0 else "-"

    print "doc:[%s]" % doc
    print "func:[%s]" % func
    print "label:[%s]" % label

def test_Introspect():
    test_Introspect_(sys._getframe())
 



if __name__ == '__main__':

    test_Loc(); 

     
 

