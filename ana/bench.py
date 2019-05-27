#!/usr/bin/env python
"""
bench.py
============

Presents launchAVG times and prelaunch times for groups of Opticks runs
with filtering based on commandline arguments of the runs and the digest 
of the geocache used.

::

    bench.py --include xanalytic --digest f6cc352e44243f8fa536ab483ad390ce
    bench.py --include xanalytic --digest f6
        selecting analytic results for a particular geometry 

    bench.py --include xanalytic --digest 52e --since May22_1030
        selecting analytic results for a particular geometry after some time 

    bench.py --digest 52 --since 6pm

    bench.py --name geocache-bench360
         fullname of the results dir

    bench.py --name 360
         also works with just a tail string, so long as it selects 
         one of the results dirs 


    bench.py --name 360 --runlabel R1
          select runs with runlabel starting R1


"""
import os, re, logging, sys, argparse
from collections import OrderedDict as odict
import numpy as np
log = logging.getLogger(__name__)

from dateutil.parser import parse
from datetime import datetime
from opticks.ana.datedfolder import DatedFolder, dateparser
from opticks.ana.meta import Meta
from opticks.ana.geocache import keydir


def lol2l(lol):
    l = []
    if lol is not None:
        for ii in lol:
            assert len(ii) == 1, "expecting a list of lists of length 1"
            i = ii[0]
            l.append(i)
        pass
    pass
    return l 



if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    argline = "bench.py %s" % " ".join(sys.argv[1:])
    print(argline)

    parser = argparse.ArgumentParser(__doc__)

    resultsprefix = "$OPTICKS_RESULTS_PREFIX" if os.environ.has_key("OPTICKS_RESULTS_PREFIX") else "$TMP"  ## equivalent to BOpticksResource::ResolveResultsPrefix
    parser.add_argument( "--resultsdir", default=os.path.join(resultsprefix, "results"), help="Directory path to results" )
    parser.add_argument( "--name", default="bench", help="String at the end of the directory names beneath resultsdir in which to look for results")
    parser.add_argument( "--digest", default=None, help="Select result groups using geocaches with digests that start with the option string")
    parser.add_argument( "--since", default=None, help="Select results from dated folders following the date string provided, eg May22_1030 or 20190522_173746")
    parser.add_argument( "--include", default=None, action='append', nargs='*', help="Select result groups with commandline containing the string provided. ALL the strings when repeated" )
    parser.add_argument( "--exclude", default=None, action='append', nargs='*', help="Select result groupd with commandline NOT containing the string. NOT containing ANY of the strings when repeated" )
    parser.add_argument( "--runlabel", default=None, help="Select result groups with runlabel starting with the string provided." )
    parser.add_argument( "--xrunlabel", default=None, help="Exclude result groups with runlabel starting with the string provided." )
    parser.add_argument( "--metric", default="launchAVG" );
    parser.add_argument( "--other", default="prelaunch000" );
    parser.add_argument( "--nodirs", dest="dirs", action="store_false", default=True );
    parser.add_argument( "--splay",  action="store_true", default=False, help="Display the example commandline in a more readable but space consuming form." );
    parser.add_argument( "--nosort",  action="store_true", default=False, help="Dont display time sorted." );
    args = parser.parse_args()
    print(args)


    ratios = odict()
    ratios["R0/1_TITAN_V"] = "R0_TITAN_V R1_TITAN_V".split()
    ratios["R0/1_TITAN_RTX"] = "R0_TITAN_RTX R1_TITAN_RTX".split()
    ratios["R1/0_TITAN_V"] = "R1_TITAN_V R0_TITAN_V".split()
    ratios["R1/0_TITAN_RTX"] = "R1_TITAN_RTX R0_TITAN_RTX".split()

    rnames = filter( lambda rname:rname.endswith(args.name),  os.listdir(os.path.expandvars(args.resultsdir)) )
    print(rnames)

    assert len(rnames) == 1, rnames
    rname = rnames[0]

    base = os.path.join( args.resultsdir, rname )
    base = os.path.expandvars(base)

    if args.since is not None:
        now = datetime.now()
        default = datetime(now.year, now.month, now.day)
        since = parse(args.since.replace("_"," "), default=default)  
        print("since : %s " % since )
    else:
        since = None
    pass 


    dirs, dfolds, dtimes = DatedFolder.find(base)
    metric = args.metric 
    other = args.other  


    ## arrange into groups of runs with the same runstamp/datedfolder
    assert len(dfolds) == len(dtimes) 
    order = sorted(range(len(dfolds)), key=lambda i:dtimes[i])   ## sorted by run datetimes

    for i in order:

        df = dfolds[i] 
        dt = dtimes[i] 

        udirs = filter(lambda _:_.endswith(df),dirs)
        #print("\n".join(udirs))
        if args.runlabel is not None:
            udirs = filter(lambda udir:os.path.dirname(udir).startswith(args.runlabel),  udirs)
        pass 
        if args.xrunlabel is not None:
            udirs = filter(lambda udir:not os.path.dirname(udir).startswith(args.xrunlabel),  udirs)
        pass 
        if len(udirs) == 0: continue


        mm = [Meta(p, base) for p in udirs]

        dtype = [ 
              ("index", np.int32),
              ("label", "|S30"),
              ("metric", np.float32),
              ("rfast", np.float32),
              ("rslow", np.float32),
              ("other", np.float32),
                ]

        a = np.recarray((len(mm),), dtype=dtype )

        labfmt_ = lambda lab:" %30s %10s %10s %10s      %10s " %  lab
        rowfmt_ = lambda row:" %30s %10.3f %10.3f %10.3f      %10.3f   " % ( row.label, row.metric, row.rfast, row.rslow, row.other )

        lab = ( df,metric,"rfast", "rslow", other)

        metric_ = lambda m:float(m.d["OTracerTimes"][metric])
        other_ = lambda m:float(m.d["OTracerTimes"][other])
        groupcommand_ = lambda m:m.d["parameters"].get("GROUPCOMMAND","-")
        geofunc_ = lambda m:m.d["parameters"].get("GEOFUNC","-")

        def key_(m):
            d = m.d["parameters"]
            k0 = d.get("OPTICKS_KEY",None)
            k1 = d.get("KEY",None)
            kk = list(set(filter(None, [k0,k1])))
            assert len(kk) == 1, d
            return kk[0]


        smm = sorted(mm, key=metric_)  

        ffast = metric_(smm[0])
        fslow = metric_(smm[-1])

        keys = map(key_, smm)
        mcmds = map(groupcommand_, smm)
        geofs = map(geofunc_, smm)

        assert len(set(mcmds)) == 1, "all OPTICKS_GROUPCOMMAND for a group of runs with same dated folder should be identical" 
        assert len(set(geofs)) == 1, "all OPTICKS_GEOFUNC for a group of runs with same dated folder should be identical" 
        assert len(set(keys)) == 1, "all OPTICKS_KEY for a group of runs with same dated folder should be identical " 
        mcmd = mcmds[0]
        geof = geofs[0]
        key = keys[0]

        digest = key.split(".")[-1]
        idpath = keydir(key) 


        includes = lol2l(args.include)
        excludes = lol2l(args.exclude)


        cmdline = smm[0].d["parameters"]["CMDLINE"]

        if len(includes)>0:
            found = list(set(map(lambda include:cmdline.find(include) > -1, includes)))
            #print(" found %s " % repr(found))
            if len(found) == 1 and found[0] == True:
                pass
            else:
                continue
            pass    
        elif len(excludes)>0:
            notfound = list(set(map(lambda exclude:cmdline.find(exclude) == -1, excludes)))
            #print(" notfound %s " % repr(notfound))
            if len(notfound) == 1 and notfound[0] == True:
                pass
            else:
                continue
            pass    
        elif args.digest is not None and not digest.startswith(args.digest):
            continue   
        elif since is not None and not dt > since:
            continue
        else:
            pass
        pass

        print("\n---  GROUPCOMMAND : %s  GEOFUNC : %s " % (mcmd, geof) )
        #print("includes %s " % repr(includes))
        #print("excludes %s " % repr(excludes))

        if args.splay:
            print("\\\n    --".join(cmdline.split("--")))
        else:
            print(cmdline)
        pass

        print(key)
        print(idpath)
        print(labfmt_(lab))

        umm = mm if args.nosort else smm

        d = odict() 
        for i, m in enumerate(umm):

            f = metric_(m)
            rfast = f/ffast
            rslow = f/fslow
            o = other_(m)
               
            a[i] = (i, m.parentfold, f, rfast, rslow, o )  

            d[m.parentfold] = f 

            print(" %s : %s  " % ( rowfmt_(a[i]), m.absdir if args.dirs else ""  )  )
        pass
        print("")
        r = odict()
        for k,v in ratios.items():
            assert len(v) == 2, v
            num,den = v
            if num in d and den in d:
                r[k] = d[num]/d[den]
            pass
        pass
        for k,v in r.items():
            print("  %30s %10.3f " % (k, v))
        pass

    pass 

    #print(args)
    print("")
    print(argline)



    