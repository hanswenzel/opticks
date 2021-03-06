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


import os, sys

class BPath(object):
    """
    Python version of brap/BPath for juicing the idpath
    allowing the srcpath, srcdigest and layout to be
    extracted 
    """
    def __init__(self, idpath):

        elem = idpath.split("/")
        last = elem[-1]
        bits = last.split(".") 

        is_triple = len(bits) == 3

        if is_triple:
            layout = 0 
            idfile = ".".join([bits[0], bits[2]])
            srcdigest = bits[1]
            assert len(srcdigest) == 32, srcdigest
            idname = elem[-2]
            geobase = os.path.sep + os.path.join( *elem[0:-2] )
            prefix = os.path.sep + os.path.join( *elem[0:-4] )
        else:
            layout = int(last)
            assert layout > 0, layout
            srcdigest = elem[-2]
            idfile = elem[-3]
            idname = elem[-4]
            geobase = os.path.sep + os.path.join( *elem[0:-4] )
            prefix = os.path.sep + os.path.join( *elem[0:-5] )
        pass
  
        self.idpath = idpath 
        self.layout = layout 
        self.srcpath = os.path.join( prefix, "opticksdata", "export", idname, idfile )
        self.srcdigest = srcdigest

    def __repr__(self):
        return "\n".join(["BPath layout %d" % self.layout, self.idpath, self.srcpath, self.srcdigest])  
      


if __name__ == '__main__':

    keys = filter(lambda k:k.startswith("IDPATH"), os.environ.keys() )
    for key in keys:
        idpath = os.environ.get(key, None)
        idp = BPath(idpath)
        print()
        print(key) 
        print(idp)

        if not os.path.isdir(idp.idpath):
           print(" missing idpath ")
        if not os.path.isfile(idp.srcpath):
           print(" missing srcpath ")



