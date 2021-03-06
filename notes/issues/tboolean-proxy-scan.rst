tboolean-proxy-scan
======================

Context
-----------

Have lots of deviation comparison machinery, need to 
get back into context with it.

* :doc:`deviation-comparison-review`




TODO : extend random alignment checking beyond history match, also do a u match at tail ?
-------------------------------------------------------------------------------------------

Currently random mis-alignment judgement is based entirely on seqhis history of photons.  
Extending to also do a umatch, eg by recording a random at the tail 
and storing into p.weight slot. 

* Actually used p.flags.y to avoid tripping ox deviation errors weights

Objective:

* decouple probable coupling between "maligned" and "deviant" issues

* :doc:`ts-box-utaildebug-decouple-maligned-from-deviant`


::

    ts box --utaildebug



::

    In [3]: a.ox[:,1,3]
    Out[3]: A([0.5475, 0.8245, 0.8068, ..., 0.8001, 0.0093, 0.7396], dtype=float32)

    In [4]: b.ox[:,1,3]
    Out[4]: A([0.5475, 0.8245, 0.8068, ..., 0.8001, 0.0093, 0.7396], dtype=float32)

    In [5]: np.all( a.ox[:,1,3] == b.ox[:,1,3] )
    Out[5]: A(False)

    In [6]: np.where( a.ox[:,1,3] != b.ox[:,1,3] )
    Out[6]: 
    (array([ 2084,  4074, 14747, 15299, 19361, 20870, 25113, 25748, 26317, 26635, 30272, 33262, 36621, 43525, 45629, 51563, 53964, 57355, 60803, 61602, 65894, 71978, 73241, 77062, 78744, 79117, 86814,
            94891]),)

    In [7]: a.ox.shape
    Out[7]: (100000, 4, 4)

    In [8]: len(np.where( a.ox[:,1,3] != b.ox[:,1,3] ))
    Out[8]: 1

    In [9]: len(np.where( a.ox[:,1,3] != b.ox[:,1,3] )[0])
    Out[9]: 28




DONE : ab.smry : summarize results into a table of 40 lines, one line for each solid
---------------------------------------------------------------------------------------

* lvidx, lvname, binary-return-code, max-deviation-in-different-categories, fraction-maligned, num-photons-used-for-test
* make the smry persistable as ab_smry.json for collection and presentation by a smry.py 


* observations : solids with more malignment also have more deviation, suspect some mixage between these issues 

::

    [blyth@localhost ana]$ absmry.py
    args: /home/blyth/opticks/ana/absmry.py
    [2019-07-14 22:03:57,217] p116563 {__init__            :absmry.py :34} INFO     - base /home/blyth/local/opticks/tmp 
    ABSmryTab
        LV   level   RC npho    fmal(%)  nmal                 rpost_dv.max              rpol_dv.max                ox_dv.max      solid
         0 WARNING 0x00   1M      0.002     2       WARNING     0.1603     0      INFO     0.0000     0   WARNING     0.1215     0      Upper_LS_tube0x5b2e9f0
         1 WARNING 0x00   1M      0.001     1       WARNING     0.1603     0      INFO     0.0000     0      INFO     0.0750     0      Upper_Steel_tube0x5b2eb10
         2 WARNING 0x00   1M      0.000     0       WARNING     0.1603     0      INFO     0.0000     0      INFO     0.0591     0      Upper_Tyvek_tube0x5b2ec30
         3 WARNING 0x00   1M      0.002     2       WARNING     0.1603     0      INFO     0.0000     0      INFO     0.0276     0      Upper_Chimney0x5b2e8e0
         4 WARNING 0x00   1M      0.000     0       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0132     0      sBar0x5b34ab0
         5 WARNING 0x00   1M      0.000     0       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0132     0      sBar0x5b34920
         6 WARNING 0x00   1M      0.004     4       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0132     0      sModuleTape0x5b34790
         7 WARNING 0x00   1M      0.000     0       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0137     0      sModule0x5b34600
         8 WARNING 0x00   1M      0.000     0       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0137     0      sPlane0x5b34470
         9 WARNING 0x00   1M      0.000     0       WARNING     0.3141     0      INFO     0.0000     0      INFO     0.0132     0      sWall0x5b342e0
        10   ERROR 0x04   1M      0.031    31       WARNING     2.1974     0      INFO     0.0000     0     ERROR     0.3281     1      sAirTT0x5b34000
        11 WARNING 0x00   1M      0.239   239       WARNING     2.1974     0      INFO     0.0000     0   WARNING     0.1567     0      sExpHall0x4bcd390
        12 WARNING 0x00   1M      0.294   294       WARNING     2.4720     0      INFO     0.0000     0   WARNING     0.1807     0      sTopRock0x4bccfc0
        13 _FATAL_ 0x06   1M      0.296   296       WARNING     1.6261     0   _FATAL_     1.9764     3   _FATAL_     1.9908     8      sTarget0x4bd4340
        14 _FATAL_ 0x07   1M      0.233   233       _FATAL_    11.4209     1   _FATAL_     1.9764     2   _FATAL_    11.3033     9      sAcrylic0x4bd3cd0
        15 WARNING 0x00   1M      0.001     1       WARNING     0.0550     0      INFO     0.0000     0      INFO     0.0493     0      sStrut0x4bd4b80
        16 WARNING 0x00   1M      0.000     0       WARNING     0.0138     0      INFO     0.0000     0      INFO     0.0005     0      sFasteners0x4c01080
        17 WARNING 0x00   1M      0.000     0       WARNING     0.0252     0      INFO     0.0000     0      INFO     0.0051     0      sMask0x4ca38d0
        18 _FATAL_ 0x06   1M      0.000     0       WARNING     0.0228     0   _FATAL_     1.9764     2   _FATAL_     1.9779     2      PMT_20inch_inner1_solid0x4cb3610
        19 _FATAL_ 0x05   1M      0.020    20       _FATAL_   429.2548    52   WARNING     0.0079     0   _FATAL_   429.2452     6      PMT_20inch_inner2_solid0x4cb3870
        20 _FATAL_ 0x07   1M      0.011    11       _FATAL_   611.2571     1   _FATAL_     1.9764     2   _FATAL_   611.2682     2      PMT_20inch_body_solid0x4c90e50
        21 _FATAL_ 0x07   1M      0.011    11       _FATAL_   611.2594     1   _FATAL_     1.9764     2   _FATAL_   611.2719     2      PMT_20inch_pmt_solid0x4c81b40
        22 WARNING 0x00   1M      0.000     0       WARNING     0.0252     0      INFO     0.0000     0      INFO     0.0014     0      sMask_virtual0x4c36e10
        23 _FATAL_ 0x07   1M      0.000     0       _FATAL_     0.0491     1   _FATAL_     1.9764     2   _FATAL_     1.9779     1      PMT_3inch_inner1_solid_ell_helper0x510ae30
        24 WARNING 0x00   1M      0.000     0       WARNING     0.0035     0      INFO     0.0000     0      INFO     0.0001     0      PMT_3inch_inner2_solid_ell_helper0x510af10
        25 _FATAL_ 0x06   1M      0.000     0       WARNING     0.0037     0   _FATAL_     1.9764     2   _FATAL_     1.9779     1      PMT_3inch_body_solid_ell_ell_helper0x510ada0
        26 WARNING 0x00   1M      0.000     0       WARNING     0.0028     0      INFO     0.0000     0      INFO     0.0000     0      PMT_3inch_cntr_solid0x510afa0
        27 WARNING 0x00   1M      0.001     1       WARNING     0.0053     0   WARNING     0.0079     0      INFO     0.0062     0      PMT_3inch_pmt_solid0x510aae0
        28 WARNING 0x00   1M      0.000     0       WARNING     0.0476     0      INFO     0.0000     0      INFO     0.0038     0      sChimneyAcrylic0x5b310c0
        29 WARNING 0x00   1M      0.000     0       WARNING     0.1799     0      INFO     0.0000     0      INFO     0.0256     0      sChimneyLS0x5b312e0
        30 WARNING 0x00   1M      0.000     0       WARNING     0.1525     0      INFO     0.0000     0   WARNING     0.1437     0      sChimneySteel0x5b314f0
        31 WARNING 0x00   1M      0.001     1       WARNING     0.1799     0      INFO     0.0000     0      INFO     0.0225     0      sWaterTube0x5b30eb0
        32 WARNING 0x00   1M      0.000     0       WARNING     0.0004     0      INFO     0.0000     0      INFO     0.0000     0      svacSurftube0x5b3bf50
        33    INFO 0x00   1M      0.000     0          INFO     0.0000     0      INFO     0.0000     0      INFO     0.0000     0      sSurftube0x5b3ab80
        34 _FATAL_ 0x06   1M      0.414   414       WARNING     1.9135     0   _FATAL_     1.9764     2   _FATAL_     1.9779    47      sInnerWater0x4bd3660
        35 _FATAL_ 0x06   1M      0.424   424       WARNING     1.9136     0   _FATAL_     1.9764     2   _FATAL_     1.9779    35      sReflectorInCD0x4bd3040
        36 WARNING 0x00   1M      0.462   462       WARNING     1.9914     0      INFO     0.0000     0   WARNING     0.1367     0      sOuterWaterPool0x4bd2960
        37 WARNING 0x00   1M      0.462   462       WARNING     1.9916     0      INFO     0.0000     0   WARNING     0.2188     0      sPoolLining0x4bd1eb0
        38 WARNING 0x00   1M      0.531   531       WARNING     2.2660     0      INFO     0.0000     0   WARNING     0.1406     0      sBottomRock0x4bcd770
        39 _FATAL_ 0x04   1M      2.939  2939       WARNING     5.4934     0      INFO     0.0000     0   _FATAL_     2.9766    78      sWorld0x4bc2350





rejigged shortcuts moving the detailed options within tboolean-lv
------------------------------------------------------------------

::

    [blyth@localhost ana]$ t opticks-tboolean-shortcuts

    opticks-tboolean-shortcuts () 
    { 
        : **simulate** : aligned bi-simulation creating OK+G4 events;
        function ts () 
        { 
            LV=$1 tboolean.sh ${@:2}
        };
        : **visualize** : load events and visualize the propagation;
        function tv () 
        { 
            LV=$1 tboolean.sh --load ${@:2}
        };
        : **visualize** the geant4 propagation;
        function tv4 () 
        { 
            LV=$1 tboolean.sh --load --vizg4 ${@:2}
        };
        : **analyse** : load events and analyse the propagation;
        function ta () 
        { 
            LV=$1 tboolean.sh --ip ${@:2}
        }
    }


TODO
----------


* see if can confirm the leverarm explanation using g4lldb.py -DD option  

* make analysis selection more lazy, its getting slow making 
  devation comparisons for tables with many categories


DONE
--------

* use 8-bits of RC more meaningfully
* split c2max rdvmax pdvmax cuts into three floats for : warning, error, fatal   
  where exceeding error level yields non-zero RC for that comparison
* report these levels in the output
* ansi colors are good interactively but need to show that in pure text way too
* have an RC for every line,  doing it as WARN/ERROR/FATAL for each line, that gets combined into max RC for each section 


DONE : Deviation comparisons with history misaligned excluded ?
------------------------------------------------------------------

::

    In [5]: ab.a.psel = ab.align

    In [6]: ab.b.psel = ab.align

    In [7]: ab.his
    Out[7]: 
    ab.his
    .                seqhis_ana  1:tboolean-proxy-11:tboolean-proxy-11   -1:tboolean-proxy-11:tboolean-proxy-11        c2        ab        ba 
    .                               9975      9975         0.00/15 =  0.00  (pval:1.000 prob:0.000)  
    0000             8ccd      7506      7506      0             0.00        1.000 +- 0.012        1.000 +- 0.012  [4 ] TO BT BT SA
    0001              8bd       514       514      0             0.00        1.000 +- 0.044        1.000 +- 0.044  [3 ] TO BR SA
    0002            8cbcd       471       471      0             0.00        1.000 +- 0.046        1.000 +- 0.046  [5 ] TO BT BR BT SA
    0003              86d       415       415      0             0.00        1.000 +- 0.049        1.000 +- 0.049  [3 ] TO SC SA
    0004            86ccd       400       400      0             0.00        1.000 +- 0.050        1.000 +- 0.050  [5 ] TO BT BT SC SA
    0005              4cd       156       156      0             0.00        1.000 +- 0.080        1.000 +- 0.080  [3 ] TO BT AB
    0006            8cc6d        65        65      0             0.00        1.000 +- 0.124        1.000 +- 0.124  [5 ] TO SC BT BT SA
    0007            8c6cd        61        61      0             0.00        1.000 +- 0.128        1.000 +- 0.128  [5 ] TO BT SC BT SA
    0008          8cc6ccd        52        52      0             0.00        1.000 +- 0.139        1.000 +- 0.139  [7 ] TO BT BT SC BT BT SA
    0009       bbbbbbb6cd        36        36      0             0.00        1.000 +- 0.167        1.000 +- 0.167  [10] TO BT SC BR BR BR BR BR BR BR
    0010           866ccd        30        30      0             0.00        1.000 +- 0.183        1.000 +- 0.183  [6 ] TO BT BT SC SC SA
    0011           8cbbcd        25        25      0             0.00        1.000 +- 0.200        1.000 +- 0.200  [6 ] TO BT BR BR BT SA
    0012             866d        23        23      0             0.00        1.000 +- 0.209        1.000 +- 0.209  [4 ] TO SC SC SA
    0013             86bd        22        22      0             0.00        1.000 +- 0.213        1.000 +- 0.213  [4 ] TO BR SC SA
    0014           8cbc6d        20        20      0             0.00        1.000 +- 0.224        1.000 +- 0.224  [6 ] TO SC BT BR BT SA
    0015           86cbcd        19        19      0             0.00        1.000 +- 0.229        1.000 +- 0.229  [6 ] TO BT BR BT SC SA
    0016           8cb6cd        14        14      0             0.00        1.000 +- 0.267        1.000 +- 0.267  [6 ] TO BT SC BR BT SA
    0017             4bcd        14        14      0             0.00        1.000 +- 0.267        1.000 +- 0.267  [4 ] TO BT BR AB
    0018             8b6d        12        12      0             0.00        1.000 +- 0.289        1.000 +- 0.289  [4 ] TO SC BR SA
    0019           8b6ccd        11        11      0             0.00        1.000 +- 0.302        1.000 +- 0.302  [6 ] TO BT BT SC BR SA
    .                               9975      9975         0.00/15 =  0.00  (pval:1.000 prob:0.000)  

Hmm deviation machiney works line by line, so need to combine selections



DONE : visualization needs auto time domain : to be of any use for large geometry
---------------------------------------------------------------------------------------

* :doc:`large-extent-geometry-sparse-photon-visualization`














LV 0-9 inclusive, chisq 0, no serious deviations : extents all less than 3.5m
-------------------------------------------------------------------------------------

* aligned scan with the new RC,  RC offset rpost_dv:0, rpol_dv:1, ox_dv:2 
* chisq 0 means the simulation histories stayed perfectly aligned 

::

    [blyth@localhost ana]$ scan--
               scan-- :       tboolean.py --pfx tboolean-proxy-0 ======= RC   4  RC 0x04       chisq:0    ab.ox_dv  maxdvmax:0.0354

               scan-- :       tboolean.py --pfx tboolean-proxy-1 ======= RC   0  RC 0x00       chisq:0

               scan-- :       tboolean.py --pfx tboolean-proxy-2 ======= RC   4  RC 0x04       chisq:0     ab.ox_dv  maxdvmax:0.0206
               scan-- :       tboolean.py --pfx tboolean-proxy-3 ======= RC   4  RC 0x04       chisq:0     ab.ox_dv  maxdvmax:0.0225

               scan-- :       tboolean.py --pfx tboolean-proxy-4 ======= RC   0  RC 0x00       chisq:0     ab.ox_dv  maxdvmax:0.0068 
               scan-- :       tboolean.py --pfx tboolean-proxy-5 ======= RC   0  RC 0x00                             maxdvmax:0.0068  
               scan-- :       tboolean.py --pfx tboolean-proxy-6 ======= RC   0  RC 0x00 
               scan-- :       tboolean.py --pfx tboolean-proxy-7 ======= RC   0  RC 0x00 
               scan-- :       tboolean.py --pfx tboolean-proxy-8 ======= RC   0  RC 0x00 
               scan-- :       tboolean.py --pfx tboolean-proxy-9 ======= RC   0  RC 0x00 



     GMeshLibTest 

     0                       Upper_LS_tube0x5b2e9f0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  0
     1                    Upper_Steel_tube0x5b2eb10 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  1
     2                    Upper_Tyvek_tube0x5b2ec30 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  2
     3                       Upper_Chimney0x5b2e8e0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  3
     4                                sBar0x5b34ab0 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  4
     5                                sBar0x5b34920 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  5
     6                         sModuleTape0x5b34790 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  6
     7                             sModule0x5b34600 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  7
     8                              sPlane0x5b34470 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  8
     9                               sWall0x5b342e0 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  9



LV 10-14 inclusive, very big extents 17-24m : 10:dropout-zero, 11+12+13+14:truncation difference 
-----------------------------------------------------------------------------------------------------

Two different issues

1. LV 10 : speckle in the hole coincidence problem : FIXED
2. LV 11,12,13,14 : truncated big bouncers loosing alignment  


::

    .
               scan-- :      tboolean.py --pfx tboolean-proxy-10 ======= RC   5  RC 0x05          
               scan-- :      tboolean.py --pfx tboolean-proxy-11 ======= RC   4  RC 0x04 
               scan-- :      tboolean.py --pfx tboolean-proxy-12 ======= RC   4  RC 0x04 
               scan-- :      tboolean.py --pfx tboolean-proxy-13 ======= RC   6  RC 0x06 
               scan-- :      tboolean.py --pfx tboolean-proxy-14 ======= RC   4  RC 0x04 

    10                              sAirTT0x5b34000 ce0          0.0000,0.0000,0.0000,24000.0000 ce1          0.0000,0.0000,0.0000,24000.0000 10
    11                            sExpHall0x4bcd390 ce0          0.0000,0.0000,0.0000,24000.0000 ce1          0.0000,0.0000,0.0000,24000.0000 11
    12                            sTopRock0x4bccfc0 ce0          0.0000,0.0000,0.0000,27000.0000 ce1          0.0000,0.0000,0.0000,27000.0000 12
    13                             sTarget0x4bd4340 ce0         0.0000,0.0000,60.0000,17760.0000 ce1          0.0000,0.0000,0.0000,17760.0000 13
    14                            sAcrylic0x4bd3cd0 ce0          0.0000,0.0000,0.0000,17820.0000 ce1          0.0000,0.0000,0.0000,17820.0000 14



Excluding non-aligned from deviation comparisons::

                   scan-- :      tboolean.py --pfx tboolean-proxy-10 ======= RC   4  RC 0x04     2/10k photons >= ox_dv.error maxdvmax:0.7034 

                   scan-- :      tboolean.py --pfx tboolean-proxy-11 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-12 ======= RC   0  RC 0x00 

                   scan-- :      tboolean.py --pfx tboolean-proxy-13 ======= RC   6  RC 0x06     POLZ issue for 2 multi-not-quite-normal "sphere-pole" photons  TO BT BR BT SA
                   scan-- :      tboolean.py --pfx tboolean-proxy-14 ======= RC   4  RC 0x04     3 ph deviations above cut 0.25 



LV:10 sAirTT COINCIDENCE/SPECKLE + HISTORY ALIGNMENT LOSSES
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* large deviations from a few photons failing to stay in history alignment

* :doc:`tboolean-proxy-scan-LV10-coincidence-speckle`  FIXED
* :doc:`tboolean-proxy-scan-negative-rpost-times` FIXED

* :doc:`tboolean-proxy-scan-LV10-history-misaligned-big-bouncer`


LV:11 sExpHall0x4bcd390 : maxdvmax:0.1265  THIS ONE IS A BIG EXTENT SOLIDS THATS CLOSE TO BEING OK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ta 11, chisq aligned : apart from one that looks like a truncation difference::

      0009       bbbbbbb6cd        43        39             0.20        1.103 +- 0.168        0.907 +- 0.145  [10] TO BT SC BR BR BR BR BR BR BR


* excluding 25/10000 maligned, makes this go away 

LV:12 sTopRock0x4bccfc0  maxdvmax:0.1836  LOOKS LIKE SAME TRUNCATION ISSUE TO LV:11 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     
ta 12, maxdvmax:0.1836, same as LV:11::

      0009       bbbbbbb6cd        47        42             0.28        1.119 +- 0.163        0.894 +- 0.138  [10] TO BT SC BR BR BR BR BR BR BR

* excluding 12/10000 maligned, makes this go away 


LV:13 sTarget0x4bd4340
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* polz problem + 4 other lines 

* :doc:`tboolean-proxy-scan-polarization`



LV:14 sAcrylic0x4bd3cd0  
~~~~~~~~~~~~~~~~~~~~~~~~~~

ta 14, maxdvmax:0.5522

* After exclude the maligned : three lines in error 


LV 15,16 
-------------

::

    .          scan-- :      tboolean.py --pfx tboolean-proxy-15 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-16 ======= RC   0  RC 0x00 

    15                              sStrut0x4bd4b80 ce0            0.0000,0.0000,0.0000,600.0000 ce1            0.0000,0.0000,0.0000,600.0000 15
    16                          sFasteners0x4c01080 ce0          0.0000,0.0000,-92.5000,150.0000 ce1            0.0000,0.0000,0.0000,150.0000 16



LV 17-22 
-----------------

* LV:18 TODO: POSSIBLE POLZ BUG TO CHASE
* LV:19/20/21 TODO: UNDERSTAND WHY ALIGNMENT LOST FOR HANDFUL OF PHOTONS OUT OF 10k 


::

    .          scan-- :      tboolean.py --pfx tboolean-proxy-17 ======= RC   4  RC 0x04     observatory dome,  chisq 0, maxdvmax:0.0241 just beneath cut
               scan-- :      tboolean.py --pfx tboolean-proxy-18 ======= RC   6  RC 0x06     cathode cap,       chisq 0, maxdvmax:1.0000   POLZ WRONG ?
               scan-- :      tboolean.py --pfx tboolean-proxy-19 ======= RC   5  RC 0x05     cap chopped PMT,   chisq != 0, maxdvmax:0.1598   
               scan-- :      tboolean.py --pfx tboolean-proxy-20 ======= RC   5  RC 0x05     full PMT,          chisq != 0, maxdvmax:0.0479 
               scan-- :      tboolean.py --pfx tboolean-proxy-21 ======= RC   5  RC 0x05     full PMT           chisq != 0, maxdvmax:0.0670
               scan-- :      tboolean.py --pfx tboolean-proxy-22 ======= RC   0  RC 0x00     cylinder           chisq 0, no warnings even


    17                               sMask0x4ca38d0 ce0          0.0000,0.0000,-78.9500,274.9500 ce1            0.0000,0.0000,0.0000,274.9500 17
    18             PMT_20inch_inner1_solid0x4cb3610 ce0           0.0000,0.0000,89.5000,249.0000 ce1            0.0000,0.0000,0.0000,249.0000 18
    19             PMT_20inch_inner2_solid0x4cb3870 ce0         0.0000,0.0000,-167.0050,249.0000 ce1            0.0000,0.0000,0.0000,249.0000 19
    20               PMT_20inch_body_solid0x4c90e50 ce0          0.0000,0.0000,-77.5050,261.5050 ce1            0.0000,0.0000,0.0000,261.5050 20
    21                PMT_20inch_pmt_solid0x4c81b40 ce0          0.0000,0.0000,-77.5050,261.5060 ce1           0.0000,0.0000,-0.0000,261.5060 21
    22                       sMask_virtual0x4c36e10 ce0          0.0000,0.0000,-79.0000,275.0500 ce1            0.0000,0.0000,0.0000,275.0500 22


Excluding maligned from deviation comparison::

                   scan-- :      tboolean.py --pfx tboolean-proxy-15 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-16 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-17 ======= RC   0  RC 0x00 

                   scan-- :      tboolean.py --pfx tboolean-proxy-18 ======= RC   6  RC 0x06      ## polz ???

                   scan-- :      tboolean.py --pfx tboolean-proxy-19 ======= RC   1  RC 0x01      ## leverarm, plausible explanation for 2 photons rpost deviations
                   scan-- :      tboolean.py --pfx tboolean-proxy-20 ======= RC   1  RC 0x01      ## one more plausible leverarm  
                   scan-- :      tboolean.py --pfx tboolean-proxy-21 ======= RC   1  RC 0x01 
                   scan-- :      tboolean.py --pfx tboolean-proxy-22 ======= RC   0  RC 0x00 



LV 18 : polarization wrong ? for "TO BT BR BR BR BT SA"  0x8cbbbcd
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* :doc:`tboolean-proxy-scan-polarization`  issue with matching transition to normal incidence handling ?



LV 19 : after exclude maligned
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    ab.a.metadata:/tmp/tboolean-proxy-19/evt/tboolean-proxy-19/torch/1         ox:b5458c3dfdb22d752d3e6acfa43683f3 rx:5b9da7ec4b0d939ec2545b0493a6b377 np:  10000 pr:    0.0059 COMPUTE_MODE compute_requested 
    ab.b.metadata:/tmp/tboolean-proxy-19/evt/tboolean-proxy-19/torch/-1        ox:98867d00e936532ab32df7389e1c3ae7 rx:6de9c9b280aa5b3dd362f254fdbedcd9 np:  10000 pr:    2.9355 COMPUTE_MODE compute_requested 
    WITH_SEED_BUFFER WITH_RECORD WITH_SOURCE WITH_ALIGN_DEV WITH_ALIGN_DEV_DEBUG WITH_LOGDOUBLE 
    []
    .
    ab.rpost_dv
    maxdvmax:0.1598  level:FATAL  RC:1       skip:
                     :                                :                   :                       :                   : 0.0228 0.0342 0.0457 :                                    
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem :  nwar  nerr  nfat :   fwar   ferr   ffat :        mx        mn       avg      
     0000            :                    TO BT BT SA :    8668     8668  :        8668    138688 :  1033     0     0 : 0.0074 0.0000 0.0000 :    0.0228    0.0000    0.0002   :              WARNING :   > dvmax[0] 0.0228  
     0001            :                       TO BR SA :     710      710  :         710      8520 :    77     0     0 : 0.0090 0.0000 0.0000 :    0.0228    0.0000    0.0002   :              WARNING :   > dvmax[0] 0.0228  
     0002            :                 TO BT BR BT SA :     503      503  :         503     10060 :    46     0     0 : 0.0046 0.0000 0.0000 :    0.0228    0.0000    0.0001   :              WARNING :   > dvmax[0] 0.0228  
     0003            :              TO BT BR BR BT SA :      74       73  :          73      1752 :    17     0     0 : 0.0097 0.0000 0.0000 :    0.0228    0.0000    0.0002   :              WARNING :   > dvmax[0] 0.0228  
     0004            :           TO BT BR BR BR BT SA :      10       11  :          10       280 :     4     0     0 : 0.0143 0.0000 0.0000 :    0.0228    0.0000    0.0003   :              WARNING :   > dvmax[0] 0.0228  
     0005            :        TO BT BR BR BR BR BT SA :      10       10  :          10       320 :    17     6     6 : 0.0531 0.0187 0.0187 :    0.1598    0.0000    0.0020   :                FATAL :   > dvmax[2] 0.0457  
     0006            :                       TO SC SA :       8        8  :           8        96 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
     0007            :                 TO BT BT SC SA :       7        7  :           7       140 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
     0009            :                       TO BT AB :       2        2  :           2        24 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
     0010            :                 TO BT SC BT SA :       2        2  :           2        40 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
     0011            :  TO BT BR BR BR BR BR BR BR BR :       1        1  :           1        40 :     1     0     0 : 0.0250 0.0000 0.0000 :    0.0228    0.0000    0.0006   :              WARNING :   > dvmax[0] 0.0228  
     0012            :  TO BT BR BR BR BR BR BR BT SA :       1        1  :           1        40 :     3     1     1 : 0.0750 0.0250 0.0250 :    0.0457    0.0000    0.0023   :                FATAL :   > dvmax[2] 0.0457  
     0013            :        TO BT BT SC BT BR BT SA :       1        1  :           1        32 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
     0014            :           TO BR SC BT BR BT SA :       1        1  :           1        28 :     1     0     0 : 0.0357 0.0000 0.0000 :    0.0228    0.0000    0.0008   :              WARNING :   > dvmax[0] 0.0228  
    .


    In [1]: ab.aselhis = "TO BT BR BR BR BR BT SA"

    In [7]: a.rpost()[1]
    Out[7]: 
    A()sliced
    A([[  23.741 ,   70.5152, -746.9043,    0.    ],
       [  23.741 ,   70.5152,  -13.925 ,    2.4449],
       [  -1.7806,   -5.2504,  167.0085,    3.6436],
       [ -26.0009,  -77.2038,   -4.7938,    4.7817],
       [  54.102 ,  160.6624,   35.9082,    6.3224],
       [  79.4637,  235.9486,  164.4975,    7.2385],
       [  79.0072,  234.6246,  167.0085,    7.2558],
       [-183.125 , -543.7364,  746.9956,   -7.4802]])

    In [8]: b.rpost()[1]
    Out[8]: 
    A()sliced
    A([[  23.741 ,   70.5152, -746.9043,    0.    ],
       [  23.741 ,   70.5152,  -13.925 ,    2.4449],
       [  -1.7806,   -5.2504,  167.0085,    3.6436],
       [ -26.0009,  -77.2038,   -4.7938,    4.7817],
       [  54.102 ,  160.6624,   35.8854,    6.3224],
       [  79.4637,  235.9486,  164.4975,    7.2383],
       [  79.0072,  234.6246,  167.0085,    7.2558],
       [-183.0793, -543.5766,  746.9956,   -7.4802]])                   ## -ve times, not enough time domain

    In [9]: a.rpost()[1] - b.rpost()[1]
    Out[9]: 
    A()sliced
    A([[ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.0228,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.0002],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [-0.0457, -0.1598,  0.    ,  0.    ]])


    In [17]: a.where
    Out[17]: array([ 100,  360,  944, 2100, 2111, 3040, 3979, 5674, 6018, 6238])

::

    In [24]: (a.rpost()[1] - b.rpost()[1])*1e4
    Out[24]: 
    A()sliced
    A([[    0.    ,     0.    ,     0.    ,     0.    ],
       [    0.    ,     0.    ,     0.    ,     0.    ],
       [    0.    ,     0.    ,     0.    ,     0.    ],
       [    0.    ,     0.    ,     0.    ,     0.    ],
       [    0.    ,     0.    ,   228.2785,     0.    ],
       [    0.    ,     0.    ,     0.    ,     2.2828],
       [    0.    ,     0.    ,     0.    ,     0.    ],
       [ -456.5569, -1597.9492,     0.    ,     0.    ]])


* look like no deviation the point before, but remember this is 
  heavily domain compressed : so cannot see deviations below compression bin size  


Curious, how can the SA absorption y position be so different::

    In [12]: a.where
    Out[12]: array([ 100,  360,  944, 2100, 2111, 3040, 3979, 5674, 6018, 6238])


    In [15]: a.ox[1]
    Out[15]: 
    A()sliced
    A([[-183.1187, -543.7328,  747.    ,   10.6096],
       [  -0.2607,   -0.7741,    0.5768,    1.    ],
       [   0.963 ,   -0.1659,    0.2125,  380.    ],
       [   0.    ,    0.    ,    0.    ,    0.    ]], dtype=float32)

    In [16]: b.ox[1]
    Out[16]: 
    A()sliced
    A([[-183.0687, -543.584 ,  747.    ,   10.6092],
       [  -0.2607,   -0.7741,    0.5769,    1.    ],
       [   0.963 ,   -0.1659,    0.2125,  380.    ],
       [   0.    ,    0.    ,    0.    ,    0.    ]], dtype=float32)


tv 19: with "TO BT BR BR BR BR BT SA" selected, suspect leverarm effect, 
very small deviations from bouncing around the inside PMT gets amplified
by the long trip up to the container where SA happens.

* hmm this a situation where visualizing both at once would be useful


Revisit with rposta
~~~~~~~~~~~~~~~~~~~~~~~~

* looks like leverarm 


::

    ab.rpost_dv
    maxdvmax:429.2548  ndvp:  52  level:FATAL  RC:1       skip:
                     :                                :                   :                       :    52    52    21 : 0.0251 0.0365 0.0479 :                                    
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem :  nwar  nerr  nfat :   fwar   ferr   ffat :        mx        mn       avg      
     0000            :                    TO BT BT SA :   86046    86046  :       86046   1376736 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0228    0.0000    0.0002   :                 INFO :  
     0001            :                       TO BR SA :    7841     7841  :        7841     94092 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0228    0.0000    0.0002   :                 INFO :  
     0002            :                 TO BT BR BT SA :    4991     4990  :        4990     99800 :     6     6     6 : 0.0001 0.0001 0.0001 :  429.2548    0.0000    0.0245   :                FATAL :   > dvmax[2] 0.0479  
     0003            :              TO BT BR BR BT SA :     723      722  :         722     17328 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0228    0.0000    0.0002   :                 INFO :  
     0004            :        TO BT BR BR BR BR BT SA :     104      104  :         104      3328 :    40    40    14 : 0.0120 0.0120 0.0042 :    0.1598    0.0000    0.0012   :                FATAL :   > dvmax[2] 0.0479  
     0005            :           TO BT BR BR BR BT SA :      80       81  :          80      2240 :     6     6     1 : 0.0027 0.0027 0.0004 :    0.0685    0.0000    0.0004   :                FATAL :   > dvmax[2] 0.0479  
     0006            :                       TO SC SA :      57       57  :          57       684 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0005    0.0000    0.0000   :                 INFO :  
     0007            :                 TO BT BT SC SA :      51       51  :          51      1020 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0228    0.0000    0.0002   :                 INFO :  
     0008            :                       TO BT AB :      22       22  :          22       264 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0228    0.0000    0.0002   :                 INFO :  
    .


Chase the 6 

::


    In [1]: ab.aselhis = "TO BT BR BT SA"

    In [5]: np.where( np.abs(ab.a.rposta - ab.b.rposta).max(axis=(1,2)) > 1 )         ## finds the 6 deviants 
    Out[5]: (array([ 670, 4209, 4435, 4894, 4943, 4950]),)

    In [6]: wh = np.where( np.abs(ab.a.rposta - ab.b.rposta).max(axis=(1,2)) > 1 )

    In [7]: ab.a.rposta[wh]
    Out[7]: 
    A([[[ -70.9946,   11.4824, -746.9043,    0.    ],
        [ -70.9946,   11.4824,  -17.1209,    2.4344],
        [   9.3138,   -1.5066,  167.0085,    3.6543],
        [  82.8194,  -13.3771,   -1.5295,    4.7706],
        [ 510.6817,  -82.5227, -746.9956,    7.6469],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ]],

    In [8]: ab.b.rposta[wh]
    Out[8]: 
    A([[[ -70.9946,   11.4824, -746.9043,    0.    ],
        [ -70.9946,   11.4824,  -17.1209,    2.4344],
        [   9.3138,   -1.5066,  167.0085,    3.6543],
        [  82.8422,  -13.3771,   -1.5979,    4.771 ],
        [  82.8422,  -13.3771, -746.9956,    7.2574],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ]],


    In [9]: ab.a.rposta[wh] - ab.b.rposta[wh]
    Out[9]: 
    A([[[   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [  -0.0228,    0.    ,    0.0685,   -0.0005],
        [ 427.8395,  -69.1455,    0.    ,    0.3894],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ]],

       [[   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.0228,   -0.0005],
        [ 429.2548,   58.9415,    0.    ,    0.3894],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ],
        [   0.    ,    0.    ,    0.    ,    0.    ]],




Now chase the 40

::

    In [1]: ab.aselhis = "TO BT BR BR BR BR BT SA"

    In [2]: ab.a.rposta.shape
    Out[2]: (104, 10, 4)

    In [3]: ab.b.rposta.shape
    Out[3]: (104, 10, 4)




How to formalize leverarm ?

::

    In [6]: ab.a.rposta[0] 
    Out[6]: 
    A([[ -70.15  ,   35.2234, -746.9043,    0.    ],
       [ -70.15  ,   35.2234,   -8.6061,    2.4627],
       [  -0.799 ,    0.4109,  167.0085,    3.626 ],
       [  69.1227,  -34.6983,  -10.1127,    4.7993],
       [-139.9347,   70.2413,   27.8271,    6.2352],
       [-219.718 ,  110.3041,  138.5878,    7.0972],
       [-213.5545,  107.1996,  167.0085,    7.2748],
       [   7.031 ,   -3.5383,  746.9956,    9.3772],
       [   0.    ,    0.    ,    0.    ,    0.    ],
       [   0.    ,    0.    ,    0.    ,    0.    ]])

    In [7]: ab.b.rposta[0] 
    Out[7]: 
    A([[ -70.15  ,   35.2234, -746.9043,    0.    ],
       [ -70.15  ,   35.2234,   -8.6061,    2.4627],
       [  -0.799 ,    0.4109,  167.0085,    3.626 ],
       [  69.1227,  -34.6983,  -10.1127,    4.7993],
       [-139.9347,   70.2413,   27.8271,    6.2352],
       [-219.718 ,  110.3041,  138.5878,    7.0972],
       [-213.5545,  107.1996,  167.0085,    7.2748],
       [   7.0081,   -3.5155,  746.9956,    9.3772],
       [   0.    ,    0.    ,    0.    ,    0.    ],
       [   0.    ,    0.    ,    0.    ,    0.    ]])

    In [8]: ab.a.rposta[0] - ab.b.rposta[0]
    Out[8]: 
    A([[ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.0228, -0.0228,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ]])






Masked running with "ts 19" photon 360
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
::

   ts 19 --mask 360 --pindex 0 --pindexlog 
   

::

    WITH_ALIGN_DEV_DEBUG photon_id:0 bounce:0
    propagate_to_boundary  u_OpBoundary:0.151521131 speed:299.79245
    propagate_to_boundary  u_OpRayleigh:0.567659318   scattering_length(s.material1.z):1000000 scattering_distance:566233.812
    propagate_to_boundary  u_OpAbsorption:0.145306677   absorption_length(s.material1.y):1e+09 absorption_distance:1.9289088e+09
    propagate_at_surface   u_OpBoundary_DiDiReflectOrTransmit:        0.270948857
    propagate_at_surface   u_OpBoundary_DoAbsorption:   0.620978355
     WITH_ALIGN_DEV_DEBUG psave (-183.118683 -543.732788 747 10.6095982) ( 1, 0, 67305987, 7296 )


After masked running, subsequent "tv 19" and "ta 19" operate with just the single photon, allowing 
to visualize and analyse just the one.

ta 19::

    In [4]: ab.aselhis = "TO BT BR BR BR BR BT SA"

    In [5]: ab.rpost()           ## time domain still not enough due to all the BR, this messes up the viz of final step 
    Out[5]: 
    (A()sliced
     A([[[  23.741 ,   70.5152, -746.9043,    0.    ],
         [  23.741 ,   70.5152,  -13.925 ,    2.445 ],
         [  -1.7806,   -5.2504,  167.0085,    3.6436],
         [ -26.0009,  -77.2038,   -4.7938,    4.7817],
         [  54.102 ,  160.6624,   35.9082,    6.3224],
         [  79.4637,  235.9486,  164.4975,    7.2383],
         [  79.0072,  234.6246,  167.0085,    7.2559],
         [-183.125 , -543.7364,  746.9956,   -9.9736]]]), A()sliced
     A([[[  23.741 ,   70.5152, -746.9043,    0.    ],
         [  23.741 ,   70.5152,  -13.925 ,    2.445 ],
         [  -1.7806,   -5.2504,  167.0085,    3.6436],
         [ -26.0009,  -77.2038,   -4.7938,    4.7817],
         [  54.102 ,  160.6624,   35.8854,    6.3224],
         [  79.4637,  235.9486,  164.4975,    7.2383],
         [  79.0072,  234.6246,  167.0085,    7.2559],
         [-183.0793, -543.5766,  746.9956,   -9.9736]]]))

    In [6]: ab.fdom
    Out[6]: 
    A(torch,1,tboolean-proxy-19)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[  0.    ,   0.    ,   0.    , 748.    ]],

       [[  0.    ,   9.9733,   9.9733,   0.    ]],

       [[ 60.    , 820.    ,  20.    , 760.    ]]], dtype=float32)




    In [1]: ab.aselhis = "TO BT BR BR BR BR BT SA"

    In [2]: ab.rpost()                  ## after increasing timedomain by introducing timemaxthumb with default of 6.0, 
                                        ## viz now as expected : leverarm effect looks a plausible explanation 
    Out[2]: 
    (A()sliced
     A([[[  23.741 ,   70.5152, -746.9043,    0.    ],
         [  23.741 ,   70.5152,  -13.925 ,    2.4449],
         [  -1.7806,   -5.2504,  167.0085,    3.6438],
         [ -26.0009,  -77.2038,   -4.7938,    4.7815],
         [  54.102 ,  160.6624,   35.9082,    6.3224],
         [  79.4637,  235.9486,  164.4975,    7.2383],
         [  79.0072,  234.6246,  167.0085,    7.2556],
         [-183.125 , -543.7364,  746.9956,   10.6095]]]), A()sliced
     A([[[  23.741 ,   70.5152, -746.9043,    0.    ],
         [  23.741 ,   70.5152,  -13.925 ,    2.4449],
         [  -1.7806,   -5.2504,  167.0085,    3.6438],
         [ -26.0009,  -77.2038,   -4.7938,    4.7815],
         [  54.102 ,  160.6624,   35.8854,    6.3224],
         [  79.4637,  235.9486,  164.4975,    7.2383],
         [  79.0072,  234.6246,  167.0085,    7.2556],
         [-183.0793, -543.5766,  746.9956,   10.609 ]]]))



    In [3]: ab.fdom
    Out[3]: 
    A(torch,1,tboolean-proxy-19)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[  0.  ,   0.  ,   0.  , 748.  ]],

       [[  0.  ,  14.96,  14.96,   0.  ]],

       [[ 60.  , 820.  ,  20.  , 760.  ]]], dtype=float32)

    In [4]: 748/50.
    Out[4]: 14.96




ts 19 : back to full
~~~~~~~~~~~~~~~~~~~~~~

::

    In [1]: ab.aselhis = "TO BT BR BR BR BR BR BR BT SA"

    In [5]: a.rpost()
    Out[5]: 
    A()sliced
    A([[[ -66.1551,   42.8935, -746.9043,    0.    ],
        [ -66.1551,   42.8935,   -8.1724,    2.464 ],
        [  -1.2099,    0.7761,  167.0085,    3.6246],
        [  64.6256,  -41.9119,  -10.5465,    4.8007],
        [-130.3698,   84.5543,   27.1423,    6.2274],
        [-205.8159,  133.4744,  136.2594,    7.0844],
        [-200.223 ,  129.8448,  167.0085,    7.2748],
        [-185.1567,  120.0745,   84.0978,    7.7889],
        [  37.7573,  -24.4943,  -70.4696,    9.6516],
        [ 412.3165, -267.3825, -746.9956,   12.3553]]])

    In [6]: b.rpost()
    Out[6]: 
    A()sliced
    A([[[ -66.1551,   42.8935, -746.9043,    0.    ],
        [ -66.1551,   42.8935,   -8.1724,    2.464 ],
        [  -1.2099,    0.7761,  167.0085,    3.6246],
        [  64.6256,  -41.9119,  -10.5465,    4.8007],
        [-130.3698,   84.5543,   27.1423,    6.2274],
        [-205.8159,  133.4744,  136.2594,    7.0844],
        [-200.223 ,  129.8448,  167.0085,    7.2748],
        [-185.1567,  120.0745,   84.0978,    7.7889],
        [  37.7573,  -24.4943,  -70.4924,    9.6516],
        [ 412.2709, -267.3597, -746.9956,   12.3549]]])

    In [7]: a.rpost() - b.rpost()
    Out[7]: 
    A()sliced
    A([[[ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.0228,  0.    ],
        [ 0.0457, -0.0228,  0.    ,  0.0005]]])


    In [8]: a.where
    Out[8]: array([2180])

    In [9]: b.where
    Out[9]: array([2180])


::

   ts 19 --mask 2180 --pindex 0 --pindexlog 
 
   ## viz again very plausible leverarm 





LV 20 : excluding maligned : down to one plausible leverarm 2301
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


ta 20::

    ab.mal
    aligned     9997/  10000 : 0.9997 : 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24 
    maligned       3/  10000 : 0.0003 : 2879,3404,6673 
          0   2879 : * :                               TO BT BR BT BR BT SA                                     TO BT BR BT SA 
          1   3404 : * :                                     TO BT BT BT SA                                  TO BT BT BT BT SA 
          2   6673 : * :                                     TO BT BT BT SA                                  TO BT BT BT BT SA 



        nph:   10000 A:    0.0059 B:    3.0332 B/A:     517.7 COMPUTE_MODE compute_requested  ALIGN non-reflectcheat 
        ab.a.metadata:/tmp/tboolean-proxy-20/evt/tboolean-proxy-20/torch/1         ox:c1d21b7240e566a9951eaed7eaea2117 rx:612caa1ae4777fa3682b6b8be58d09bd np:  10000 pr:    0.0059 COMPUTE_MODE compute_requested 
        ab.b.metadata:/tmp/tboolean-proxy-20/evt/tboolean-proxy-20/torch/-1        ox:b6a2d9b838d5ea971a8bf58ac136a9ab rx:dca93d3040fa2ade890bd1f9d9688fa7 np:  10000 pr:    3.0332 COMPUTE_MODE compute_requested 
        WITH_SEED_BUFFER WITH_RECORD WITH_SOURCE WITH_ALIGN_DEV WITH_ALIGN_DEV_DEBUG WITH_LOGDOUBLE 
        []
        .
        ab.rpost_dv
        maxdvmax:15.7105  level:FATAL  RC:1       skip:
                         :                                :                   :                       :                   : 0.0240 0.0360 0.0479 :                                    
          idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem :  nwar  nerr  nfat :   fwar   ferr   ffat :        mx        mn       avg      
         0000            :                    TO BT BT SA :    8681     8681  :        8681    138896 :   119     0     0 : 0.0009 0.0000 0.0000 :    0.0240    0.0000    0.0000   :              WARNING :   > dvmax[0] 0.0240  
         0001            :                       TO BR SA :     696      696  :         696      8352 :     9     0     0 : 0.0011 0.0000 0.0000 :    0.0240    0.0000    0.0000   :              WARNING :   > dvmax[0] 0.0240  
         0002            :                 TO BT BR BT SA :     345      346  :         345      6900 :    15     1     1 : 0.0022 0.0001 0.0001 :   15.7105    0.0000    0.0023   :                FATAL :   > dvmax[2] 0.0479  
         0003            :              TO BT BR BR BT SA :     174      174  :         174      4176 :    22     0     0 : 0.0053 0.0000 0.0000 :    0.0240    0.0000    0.0001   :              WARNING :   > dvmax[0] 0.0240  
         0004            :           TO BT BR BR BR BT SA :      54       54  :          54      1512 :     9     0     0 : 0.0060 0.0000 0.0000 :    0.0240    0.0000    0.0001   :              WARNING :   > dvmax[0] 0.0240  
         0005            :           TO BT BR BT BT BT SA :      10       10  :          10       280 :     2     0     0 : 0.0071 0.0000 0.0000 :    0.0240    0.0000    0.0002   :              WARNING :   > dvmax[0] 0.0240  
         0006            :        TO BT BR BR BT BT BT SA :       8        8  :           8       256 :     4     0     0 : 0.0156 0.0000 0.0000 :    0.0240    0.0000    0.0004   :              WARNING :   > dvmax[0] 0.0240  
         0007            :                       TO SC SA :       7        7  :           7        84 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0008            :                 TO BT BT SC SA :       5        5  :           5       100 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0009            :           TO BT BR BR BT BR SA :       3        3  :           3        84 :     3     1     1 : 0.0357 0.0119 0.0119 :    0.0479    0.0000    0.0011   :                FATAL :   > dvmax[2] 0.0479  
         0010            :                       TO BT AB :       3        3  :           3        36 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0011            :        TO BT BR BR BR BR BT SA :       3        3  :           3        96 :     1     0     0 : 0.0104 0.0000 0.0000 :    0.0240    0.0000    0.0002   :              WARNING :   > dvmax[0] 0.0240  
         0012            :                 TO BT SC BT SA :       3        3  :           3        60 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0013            :           TO BT BT SC BT BT SA :       2        2  :           2        56 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0015            :              TO BR SC BT BT SA :       1        1  :           1        24 :     0     0     0 : 0.0000 0.0000 0.0000 :    0.0000    0.0000    0.0000   :                 INFO :  
         0016            :     TO BT BR BR BR BR BR BT SA :       1        1  :           1        36 :     1     0     0 : 0.0278 0.0000 0.0000 :    0.0240    0.0000    0.0007   :              WARNING :   > dvmax[0] 0.0240  
         0018            :  TO BT SC BR BR BR BR BR BR BR :       1        1  :           1        40 :     1     0     0 : 0.0250 0.0000 0.0000 :    0.0240    0.0000    0.0006   :              WARNING :   > dvmax[0] 0.0240  
        .



Find the deviant::

    In [7]: aa = a.rpost()

    In [8]: bb = b.rpost()

    In [27]: aa.shape
    Out[27]: (345, 5, 4)

    In [28]: bb.shape
    Out[28]: (345, 5, 4)

    In [30]: dd = np.abs(aa - bb)

    In [31]: dd.max(axis=(1,2)).shape
    Out[31]: (345,)


    In [32]: dd.max(axis=(1,2))
    Out[32]: 
    A()sliced
    A([ 0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,
       ...
        0.    ,  0.    ,  0.    , 15.7105,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.024 ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,
        0.    ,  0.    ,  0.    ,  0.0002,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,
        0.    ,  0.024 ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.024 ,  0.    ,  0.    ,  0.024 ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,
        0.024 ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ,  0.    ])

    In [33]: a.where
    Out[33]: 
    array([  62,  246,  268,  327,  355,  370,  387,  433,  491,  536,  554,  575,  598,  615,  634,  662,  666,  684,  722,  731,  744,  756,  771,  805,  806,  837,  838,  902,  906,  940,  944,  975,
           1017, 1048, 1049, 1064, 1095, 1102, 1247, 1276, 1353, 1363, 1419, 1442, 1498, 1500, 1509, 1513, 1516, 1584, 1586, 1645, 1688, 1689, 1691, 1702, 1750, 1819, 1820, 1863, 1882, 1907, 1942, 1943,


    In [37]: a.where[np.where( dd.max(axis=(1,2)) > 1. )]
    Out[37]: array([8021])

    In [38]: a.where[np.where( dd.max(axis=(1,2)) > 0.01 )]
    Out[38]: array([ 837, 1750, 1943, 1994, 2729, 2950, 3090, 6203, 7413, 8021, 8260, 9111, 9223, 9311, 9785])


::

   ts 20 --mask 8021 --pindex 0 --pindexlog 



huh the deviant 8021 isnt deviating any more ?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    In [3]: ab.aselhis = None

    In [4]: a.rposti(8021)
    Out[4]: 
    A()sliced
    A([[ -13.3049,  -50.3907, -784.4123,    0.    ],
       [ -13.3049,  -50.3907, -138.7303,    2.1537],
       [  29.6063,  112.0726,  241.2138,    4.6713],
       [  -9.4213,  -35.6954, -261.4947,    7.8554],
       [ -83.0656, -314.4505, -784.5082,    9.8475]])

    In [5]: b.rposti(8021)
    Out[5]: 
    A()sliced
    A([[ -13.3049,  -50.3907, -784.4123,    0.    ],
       [ -13.3049,  -50.3907, -138.7303,    2.1537],
       [  29.6063,  112.0726,  241.2138,    4.6713],
       [  -9.4213,  -35.6954, -261.4947,    7.8554],
       [ -83.0656, -314.4505, -784.5082,    9.8475]])

    In [6]: a.rposti(8021) - b.rposti(8021)
    Out[6]: 
    A()sliced
    A([[0., 0., 0., 0.],
       [0., 0., 0., 0.],
       [0., 0., 0., 0.],
       [0., 0., 0., 0.],
       [0., 0., 0., 0.]])

    In [7]: a.fdom
    Out[7]: 
    A(torch,1,tboolean-proxy-20)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[  0.    ,   0.    ,   0.    , 785.515 ]],

       [[  0.    ,  15.7103,  15.7103,   0.    ]],

       [[ 60.    , 820.    ,  20.    , 760.    ]]], dtype=float32)


Probably previously over time domain, but not after increasing --timemaxthumb from 3 to 6::

    In [8]: 785.515/50.
    Out[8]: 15.7103

    In [9]: 785.515/100.
    Out[9]: 7.85515


Yep, reproduces old issue with::

    ts 20 --timemaxthumb 3


::

    In [1]: a.rposti(8021)
    Out[1]: 
    A()sliced
    A([[ -13.3049,  -50.3907, -784.4123,    0.    ],
       [ -13.3049,  -50.3907, -138.7303,    2.1537],
       [  29.6063,  112.0726,  241.2138,    4.6713],
       [  -9.4213,  -35.6954, -261.4947,    7.8551],
       [ -83.0656, -314.4505, -784.5082,   -7.8554]])

    In [2]: b.rposti(8021)
    Out[2]: 
    A()sliced
    A([[ -13.3049,  -50.3907, -784.4123,    0.    ],
       [ -13.3049,  -50.3907, -138.7303,    2.1537],
       [  29.6063,  112.0726,  241.2138,    4.6713],
       [  -9.4213,  -35.6954, -261.4947,   -7.8554],
       [ -83.0656, -314.4505, -784.5082,   -7.8554]])

    In [3]: a.rposti(8021) - b.rposti(8021)
    Out[3]: 
    A()sliced
    A([[ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    ,  0.    ],
       [ 0.    ,  0.    ,  0.    , 15.7105],
       [ 0.    ,  0.    ,  0.    ,  0.    ]])



That leaves one for LV 20, ta 20::

    In [1]: ab.aselhis = "TO BT BR BR BT BR SA"

    In [2]: a.rpost()
    Out[2]: 
    A()sliced
    A([[[  26.0344,   33.0824, -784.4123,    0.    ],
        [  26.0344,   33.0824, -261.4947,    1.7443],
        [  26.0344,   33.0824,  258.9536,    4.8981],
        [ -34.1612,  -43.3907, -134.7268,    7.3553],
        [  33.418 ,   42.4318, -136.2851,    8.0174],
        [  58.5894,   74.4354,  -93.23  ,    8.215 ],
        [ 617.6099,  784.5082, -320.7314,   11.3238]],
        ... 

    In [3]: b.rpost()
    Out[3]: 
    A()sliced
    A([[[  26.0344,   33.0824, -784.4123,    0.    ],
        [  26.0344,   33.0824, -261.4947,    1.7443],
        [  26.0344,   33.0824,  258.9536,    4.8981],
        [ -34.1612,  -43.3907, -134.7268,    7.3553],
        [  33.418 ,   42.4318, -136.2851,    8.0174],
        [  58.5894,   74.4354,  -93.23  ,    8.215 ],
        [ 617.6099,  784.5082, -320.7793,   11.3238]],
        ...

    In [4]: a.rpost() - b.rpost()
    Out[4]: 
    A()sliced
    A([[[ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.0479,  0.    ]],
        ...

    In [5]: ab.rpost_dv_where(0.025)
    Out[5]: array([2301])


Have a look at that one::

   ts 20 --mask 2301 --pindex 0 --pindexlog 


* it transmits out and bounces off the bulb, leverarm again looks plausible 




LV 21,  BT difference ?  maxdvmax:0.0719
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* this looks the same as 20, with same time domain issue



LV 23-27 : small extent, chisq 0
------------------------------------------

::

    23   PMT_3inch_inner1_solid_ell_helper0x510ae30 ce0            0.0000,0.0000,14.5216,38.0000 ce1             0.0000,0.0000,0.0000,38.0000 23
    24   PMT_3inch_inner2_solid_ell_helper0x510af10 ce0            0.0000,0.0000,-4.4157,38.0000 ce1             0.0000,0.0000,0.0000,38.0000 24
    25 PMT_3inch_body_solid_ell_ell_helper0x510ada0 ce0             0.0000,0.0000,4.0627,40.0000 ce1             0.0000,0.0000,0.0000,40.0000 25
    26                PMT_3inch_cntr_solid0x510afa0 ce0           0.0000,0.0000,-45.8740,29.9995 ce1             0.0000,0.0000,0.0000,29.9995 26
    27                 PMT_3inch_pmt_solid0x510aae0 ce0           0.0000,0.0000,-17.9373,57.9383 ce1             0.0000,0.0000,0.0000,57.9383 27



               scan-- :      tboolean.py --pfx tboolean-proxy-23 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-24 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-25 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-26 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-27 ======= RC   0  RC 0x00 




LV 28-31 
------------------------------------

* LV:28 not a fair test, the photons almost entirely missed 
* LV:29 perfect chisq zero, maxdvmax:0.0550 : ERROR CUT 0.0200  TOO TIGHT ? 


::

    28                     sChimneyAcrylic0x5b310c0 ce0            0.0000,0.0000,0.0000,520.0000 ce1            0.0000,0.0000,0.0000,520.0000 28
    29                          sChimneyLS0x5b312e0 ce0           0.0000,0.0000,0.0000,1965.0000 ce1           0.0000,0.0000,0.0000,1965.0000 29
    30                       sChimneySteel0x5b314f0 ce0           0.0000,0.0000,0.0000,1665.0000 ce1           0.0000,0.0000,0.0000,1665.0000 30
    31                          sWaterTube0x5b30eb0 ce0           0.0000,0.0000,0.0000,1965.0000 ce1           0.0000,0.0000,0.0000,1965.0000 31



               scan-- :      tboolean.py --pfx tboolean-proxy-28 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-29 ======= RC   4  RC 0x04      perfect chisq zero, maxdvmax:0.0550  
               scan-- :      tboolean.py --pfx tboolean-proxy-30 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-31 ======= RC   0  RC 0x00 

Excluding maligned from deviation check::

                   scan-- :      tboolean.py --pfx tboolean-proxy-28 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-29 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-30 ======= RC   0  RC 0x00 
                   scan-- :      tboolean.py --pfx tboolean-proxy-31 ======= RC   1  RC 0x01       ab.rpost_dv  single photon way off ??  TO BR SC BT BR BT SA : SEE BELOW FIXED



ta 31 : funny one RC 0x01 : zero maligned (its a small tubs)  : FIXED BY CHANGING TIME DOMAIN RULE OF THUMB IN Opticks::setupTimeDomain
--------------------------------------------------------------------------------------------------------------------------------------------


* fails ab.rpost_dv but not ab.ox_dv : very unusual : EXPLAINED BY BEING ON EDGE OF TIMEDOMAIN 

ab.rpost_dv::

   0020            :           TO BR SC BT BR BT SA :       1        1  :           1        28 :     1     1     1 : 0.0357 0.0357 0.0357 :  117.9218    0.0000    4.2115   :                FATAL :   > dvmax[2] 0.3599  



* B has -ve last time, so the timedomain is just too short such that B goes overdomain but A doesnt 
* rule of thumb timedomain needs tweaking ?

Opticks::setupTimeDomain::

    2004     //float rule_of_thumb_timemax = 3.f*extent/speed_of_light ;
    2005     float rule_of_thumb_timemax = 4.f*extent/speed_of_light ;


* checking in general with rpostn(10) (9) (8) .. see quite a few -ve times


::

    In [4]: a.rpost()
    Out[4]: 
    A()sliced
    A([[[ -496.2666,   154.9259, -5894.9204,     0.    ],
        [ -496.2666,   154.9259, -1964.9135,    13.1084],
        [ -496.2666,   154.9259, -2118.2199,    13.6194],
        [ -360.594 ,    90.3284, -1964.9135,    14.3356],
        [  433.1087,  -287.7195,   -75.3937,    26.9636],
        [ -275.3038,   344.0398,  1964.9135,    40.601 ],
        [-3151.239 ,  2908.6837,  5894.9204,    58.96  ]]])

    In [5]: b.rpost()
    Out[5]: 
    A()sliced
    A([[[ -496.2666,   154.9259, -5894.9204,     0.    ],
        [ -496.2666,   154.9259, -1964.9135,    13.1084],
        [ -496.2666,   154.9259, -2118.2199,    13.6194],
        [ -360.594 ,    90.3284, -1964.9135,    14.3356],
        [  433.1087,  -287.7195,   -75.3937,    26.9636],
        [ -275.3038,   344.0398,  1964.9135,    40.601 ],
        [-3151.239 ,  2908.6837,  5894.9204,   -58.9618]]])


    In [6]: a.fdom
    Out[6]: 
    A(torch,1,tboolean-proxy-31)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[   0.  ,    0.  ,    0.  , 5896.  ]],

       [[   0.  ,   58.96,   58.96,    0.  ]],

       [[  60.  ,  820.  ,   20.  ,  760.  ]]], dtype=float32)

    In [7]: b.fdom
    Out[7]: 
    A(torch,-1,tboolean-proxy-31)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[   0.  ,    0.  ,    0.  , 5896.  ]],

       [[   0.  ,   58.96,   58.96,    0.  ]],

       [[  60.  ,  820.  ,   20.  ,  760.  ]]], dtype=float32)

    In [8]: np.all( a.fdom == b.fdom )
    Out[8]: 
    A()sliced
    A(True)








LV 32-33, torus placeholder small boxes
------------------------------------------

::

    32                        svacSurftube0x5b3bf50 ce0              0.0000,0.0000,0.0000,4.0000 ce1              0.0000,0.0000,0.0000,4.0000 32
    33                           sSurftube0x5b3ab80 ce0              0.0000,0.0000,0.0000,5.0000 ce1              0.0000,0.0000,0.0000,5.0000 33

               scan-- :      tboolean.py --pfx tboolean-proxy-32 ======= RC   0  RC 0x00 
               scan-- :      tboolean.py --pfx tboolean-proxy-33 ======= RC   0  RC 0x00 



    

LV 34-39, big extent 
--------------------------

* looks like similar issues to LV 10-14


::

    34                         sInnerWater0x4bd3660 ce0        0.0000,0.0000,850.0000,20900.0000 ce1          0.0000,0.0000,0.0000,20900.0000 34
    35                      sReflectorInCD0x4bd3040 ce0        0.0000,0.0000,849.0000,20901.0000 ce1          0.0000,0.0000,0.0000,20901.0000 35
    36                     sOuterWaterPool0x4bd2960 ce0          0.0000,0.0000,0.0000,21750.0000 ce1          0.0000,0.0000,0.0000,21750.0000 36
    37                         sPoolLining0x4bd1eb0 ce0         0.0000,0.0000,-1.5000,21753.0000 ce1          0.0000,0.0000,0.0000,21753.0000 37
    38                         sBottomRock0x4bcd770 ce0      0.0000,0.0000,-1500.0000,24750.0000 ce1          0.0000,0.0000,0.0000,24750.0000 38
    39                              sWorld0x4bc2350 ce0          0.0000,0.0000,0.0000,60000.0000 ce1          0.0000,0.0000,0.0000,60000.0000 39


               scan-- :      tboolean.py --pfx tboolean-proxy-34 ======= RC   5  RC 0x05    sphere with protrusion, non zero chisq, big bouncers again
               scan-- :      tboolean.py --pfx tboolean-proxy-35 ======= RC   5  RC 0x05    ditto geometry 
               scan-- :      tboolean.py --pfx tboolean-proxy-36 ======= RC   4  RC 0x04    big cylinder 
               scan-- :      tboolean.py --pfx tboolean-proxy-37 ======= RC   4  RC 0x04    big polycone cylinder
               scan-- :      tboolean.py --pfx tboolean-proxy-38 ======= RC   4  RC 0x04    another big polycone
               scan-- :      tboolean.py --pfx tboolean-proxy-39 ======= RC   5  RC 0x05    worldbox 

                              tp 39 : handful of photons are way out, failed to stay aligned ?



Excluding maligned from deviation comparison::

                   scan-- :      tboolean.py --pfx tboolean-proxy-34 ======= RC   5  RC 0x05      single truncated photon way off, handful with deviations above cut 0.25
                   scan-- :      tboolean.py --pfx tboolean-proxy-35 ======= RC   5  RC 0x05      single truncated photon 13460.4918 off  "TO BT BT BT BR BR BR BR BR AB"
                   scan-- :      tboolean.py --pfx tboolean-proxy-36 ======= RC   4  RC 0x04      two lines just above cut 0.25 
                   scan-- :      tboolean.py --pfx tboolean-proxy-37 ======= RC   4  RC 0x04      one line just above cut 0.25
                   scan-- :      tboolean.py --pfx tboolean-proxy-38 ======= RC   4  RC 0x04      two lines just above cut 
                   scan-- :      tboolean.py --pfx tboolean-proxy-39 ======= RC   5  RC 0x05      around 25 lines in error : ~8 of them the otherside of universe
 

* hmm, I am wrapping this huge geometry like the worldBox in a container with a x3 scaling to make an enormous domain
  and then shooting photons from one side to the other...  thats far too much rope for the photons to play with 

* clearly see more problems the larger the domain


180m extent::

    In [1]: ab.fdom
    Out[1]: 
    A(torch,1,tboolean-proxy-39)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[     0.  ,      0.  ,      0.  , 180001.  ]],

       [[     0.  ,   1800.01,   1800.01,      0.  ]],

       [[    60.  ,    820.  ,     20.  ,    760.  ]]], dtype=float32





34 : although same history the AB happens in different place
-------------------------------------------------------------------

* -ve times show timedomain not sufficient with all that reflecting 

ta 34::

    In [1]: ab.aselhis = "TO BT BT BT BR BR BR BR BR AB"

    In [3]: a.rpost()
    Out[3]: 
    A()sliced
    A([[[ -3817.5144,   -924.2403, -62699.0865,      0.    ],
        [ -3817.5144,   -924.2403, -20511.2467,    140.7218],
        [  -769.2435,   -185.6135,  19185.1627,    382.0193],
        [  -514.7425,   -124.3802,  20823.1538,    387.5495],
        [  -413.3249,    -99.5041,  20899.6955,    388.334 ],
        [   514.7425,    124.3802,  20191.6853,    395.5481],
        [  -514.7425,   -124.3802,  19407.1335,    403.5275],
        [ 18706.7774,   4527.4382,   4766.6308,    552.6498],
        [ 10275.7155,   2487.6034, -17885.8683,   -627.0291],
        [  6454.374 ,   1561.4495, -17514.6413,   -627.0291]]])

    In [4]: b.rpost()
    Out[4]: 
    A()sliced
    A([[[ -3817.5144,   -924.2403, -62699.0865,      0.    ],
        [ -3817.5144,   -924.2403, -20511.2467,    140.7218],
        [  -769.2435,   -185.6135,  19185.1627,    382.0193],
        [  -514.7425,   -124.3802,  20823.1538,    387.5495],
        [  -413.3249,    -99.5041,  20899.6955,    388.334 ],
        [   514.7425,    124.3802,  20191.6853,    395.5481],
        [  -514.7425,   -124.3802,  19407.1335,    403.5275],
        [ 18706.7774,   4527.4382,   4766.6308,    552.6498],
        [ 10275.7155,   2487.6034, -17885.8683,   -627.0291],
        [ -7005.4738,  -1695.3974, -16205.7793,   -627.0291]]])

    In [6]: a.rpost() - b.rpost()
    Out[6]: 
    A()sliced
    A([[[    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [13459.8478,  3256.8469, -1308.8621,     0.    ]]])



    In [5]: a.fdom
    Out[5]: 
    A(torch,1,tboolean-proxy-34)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[    0.  ,     0.  ,     0.  , 62701.  ]],

       [[    0.  ,   627.01,   627.01,     0.  ]],

       [[   60.  ,   820.  ,    20.  ,   760.  ]]], dtype=float32)



35 : same geometry as 34 just slightly different size
-------------------------------------------------------------

ta 35, same thing happens::

    In [1]: ab.aselhis = "TO BT BT BT BR BR BR BR BR AB"

    In [2]: a.rpost()
    Out[2]: 
    A()sliced
    A([[[ -3817.6971,   -924.2846, -62702.0864,      0.    ],
        [ -3817.6971,   -924.2846, -20512.228 ,    140.7285],
        [  -769.2803,   -185.6224,  19187.9943,    382.0376],
        [  -516.6808,   -124.3861,  20812.6684,    387.5297],
        [  -401.8628,    -97.5953,  20900.6955,    388.4291],
        [   516.6808,    124.3861,  20200.3059,    395.567 ],
        [  -516.6808,   -124.3861,  19413.8029,    403.5851],
        [ 18709.5861,   4527.6548,   4768.7725,    552.7145],
        [ 10280.0344,   2487.7224, -17884.8104,   -627.0591],
        [  6458.5101,   1563.4378, -17513.5657,   -627.0591]]])

    In [3]: b.rpost()
    Out[3]: 
    A()sliced
    A([[[ -3817.6971,   -924.2846, -62702.0864,      0.    ],
        [ -3817.6971,   -924.2846, -20512.228 ,    140.7285],
        [  -769.2803,   -185.6224,  19187.9943,    382.0376],
        [  -516.6808,   -124.3861,  20812.6684,    387.5297],
        [  -401.8628,    -97.5953,  20900.6955,    388.4291],
        [   516.6808,    124.3861,  20200.3059,    395.567 ],
        [  -516.6808,   -124.3861,  19413.8029,    403.5851],
        [ 18709.5861,   4527.6548,   4768.7725,    552.7145],
        [ 10280.0344,   2487.7224, -17884.8104,   -627.0591],
        [ -7001.9817,  -1693.5649, -16210.3819,   -627.0591]]])

    In [4]: a.rpost() - b.rpost()
    Out[4]: 
    A()sliced
    A([[[    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [    0.    ,     0.    ,     0.    ,     0.    ],
        [13460.4918,  3257.0027, -1303.1838,     0.    ]]])







tp 0/1/2/3
----------------------

With large extent geometries suspect some errors just from rpost domain compression bin edges.

* NOW CONFIRMED 

Note same deviation number appearing 0.1603 for the first four which have same extent 
which gets scaled to make the domain.

::

    GMeshLibTest

    2019-06-20 17:12:06.694 INFO  [374159] [test_dump1@103]  num_mesh 41
     0                       Upper_LS_tube0x5b2e9f0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  0
     1                    Upper_Steel_tube0x5b2eb10 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  1
     2                    Upper_Tyvek_tube0x5b2ec30 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  2
     3                       Upper_Chimney0x5b2e8e0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  3



* TODO: get domain extent into the report 
* DONE: automate the rdvmax cuts based on the compression bin sizes for the fdomain in use


::

    ab.b.metadata:/tmp/tboolean-proxy-0/evt/tboolean-proxy-0/torch/-1          ox:7a76a0edf3bfc0ae98538fd2bff8e027 rx:04b5725eed5ebda2b1b7a828df5aa5ed np:  10000 pr:    2.2949 COMPUTE_MODE compute_requested 
    WITH_SEED_BUFFER WITH_RECORD WITH_SOURCE WITH_ALIGN_DEV WITH_LOGDOUBLE 
    []
    .
    ab.rpost_dv
    maxdvmax:0.1603 maxdv:0.1603 0.1603 0.0000 0.0000 0.1603 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.1603 0.0000  skip:
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem      nerr:  ferr          mx        mn       avg      
     0000            :                          TO SA :    5411     5411  :        5411     43288         2: 0.000      0.1603    0.0000    0.0000   :       ERROR :   > dvmax[1] 0.1000  
     0001            :                    TO BT BT SA :    3950     3950  :        3950     63200         4: 0.000      0.1603    0.0000    0.0000   :       ERROR :   > dvmax[1] 0.1000  
     0002            :                 TO BT BR BT SA :     260      260  :         260      5200         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0003            :                       TO BR SA :     253      253  :         253      3036         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0004            :                       TO SC SA :      55       55  :          55       660         1: 0.002      0.1603    0.0000    0.0002   :       ERROR :   > dvmax[1] 0.1000  
     0005            :                 TO BT BT SC SA :      22       22  :          22       440         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0006            :                       TO BT AB :      14       14  :          14       168         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0007            :              TO BT BR BR BT SA :      13       13  :          13       312         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0008            :                 TO SC BT BT SA :       6        6  :           6       120         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0009            :  TO BT SC BR BR BR BR BR BR BR :       4        4  :           4       160         0: 0.000      0.0000    0.0000    0.0000   :             :  

::

    ab.b.metadata:/tmp/tboolean-proxy-1/evt/tboolean-proxy-1/torch/-1          ox:fbb6fe2129d4bc18c43684ea75e2e7de rx:447d0281e37832dc4901f81393b5e2da np:  10000 pr:    2.0586 COMPUTE_MODE compute_requested 
    WITH_SEED_BUFFER WITH_RECORD WITH_SOURCE WITH_ALIGN_DEV WITH_LOGDOUBLE 
    []
    .
    ab.rpost_dv
    maxdvmax:0.1603 maxdv:0.1603 0.0000 0.1603 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000  skip:
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem      nerr:  ferr          mx        mn       avg      
     0000            :                          TO SA :    9776     9776  :        9776     78208         4: 0.000      0.1603    0.0000    0.0000   :       ERROR :   > dvmax[1] 0.1000  
     0001            :                    TO BT BT SA :     115      115  :         115      1840         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0002            :                       TO SC SA :      68       68  :          68       816         1: 0.001      0.1603    0.0000    0.0002   :       ERROR :   > dvmax[1] 0.1000  
     0003            :                       TO BR SA :      10       10  :          10       120         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0004            :                 TO SC BT BT SA :      10       10  :          10       200         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0005            :           TO SC BT BT BT BT SA :       6        6  :           6       168         0: 0.000      0.0000    0.0000    0.0000   :             :  


    ab.b.metadata:/tmp/tboolean-proxy-2/evt/tboolean-proxy-2/torch/-1          ox:e60cbb075eed2952304dbf187fd3aabf rx:c911af1562f91d2ca6ad17990b99e6ad np:  10000 pr:    2.0293 COMPUTE_MODE compute_requested 
    WITH_SEED_BUFFER WITH_RECORD WITH_SOURCE WITH_ALIGN_DEV WITH_LOGDOUBLE 
    []
    .
    ab.rpost_dv
    maxdvmax:0.1603 maxdv:0.1603 0.1603 0.0000 0.1603 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000  skip:
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem      nerr:  ferr          mx        mn       avg      
     0000            :                          TO SA :    9861     9861  :        9861     78888         4: 0.000      0.1603    0.0000    0.0000   :       ERROR :   > dvmax[1] 0.1000  
     0001            :                       TO SC SA :      70       70  :          70       840         1: 0.001      0.1603    0.0000    0.0002   :       ERROR :   > dvmax[1] 0.1000  
     0002            :                    TO BT BT SA :      41       41  :          41       656         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0003            :                 TO SC BT BT SA :      10       10  :          10       200         1: 0.005      0.1603    0.0000    0.0008   :       ERROR :   > dvmax[1] 0.1000  
     0004            :           TO SC BT BT BT BT SA :       6        6  :           6       168         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0005            :                 TO BT BR BT SA :       4        4  :           4        80         0: 0.000      0.0000    0.0000    0.0000   :             :  
     0006            :                    TO SC BR SA :       4        4  :           4        64         0: 0.000      0.0000    0.0000    0.0000   :             :  


* also tp 3, 

::

    In [1]: ab.a.fdom
    Out[1]: 
    A(torch,1,tboolean-proxy-1)(metadata) 3*float4 domains of position, time, wavelength (used for compression)
    A([[[   0.,    0.,    0., 5251.]],



    In [1]: 1750*3    ## factor 3 from container scale
    Out[1]: 5250

    In [2]: 1750*3*2     ## 2 as extent is [-extent, extent]
    Out[2]: 10500

    In [3]: 1750*3*2./0.1603
    Out[3]: 65502.18340611354

    In [5]: 0x1 << 16
    Out[5]: 65536



Hmm this gives a hint of what is the appropriate deviation cut for rpost checking 
based on the domain of the geometry.

::

    In [4]: ab.a.fdom[0,0,3]
    Out[4]: 5251.0

    In [5]: ab.a.fdom[0,0,3]*2.0/(0x1 << 16)
    Out[5]: 0.160247802734375


Was using a fixed triplet::

    In [7]: ab.ok.rdvmax
    Out[7]: [0.01, 0.1, 1.0]


Can instead can now use a more motivated cut.  DONE using [eps, 1.5*eps, 2.0*eps] as warn/error/fatal levels where eps is compression bin size 

::

    In [10]: np.float(ab.rpost_dv.dvs[0].dv.max())
    Out[10]: 0.16025269325837144

    In [16]: 2.*5251./(65536.-1.)
    Out[16]: 0.16025024795910583




::

     84 __device__ short shortnorm( float v, float center, float extent )
     85 {
     86     // range of short is -32768 to 32767
     87     // Expect no positions out of range, as constrained by the geometry are bouncing on,
     88     // but getting times beyond the range eg 0.:100 ns is expected
     89     //
     90     int inorm = __float2int_rn(32767.0f * (v - center)/extent ) ;    // linear scaling into -1.f:1.f * float(SHRT_MAX)
     91     return fitsInShort(inorm) ? short(inorm) : SHRT_MIN  ;
     92 }
     93 




tp 4-9
---------

::

     4                                sBar0x5b34ab0 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  4
     5                                sBar0x5b34920 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  5
     6                         sModuleTape0x5b34790 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  6
     7                             sModule0x5b34600 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  7
     8                              sPlane0x5b34470 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  8
     9                               sWall0x5b342e0 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  9








aligned scan
---------------

* hmm generally the number of photons with discreps is small, could return that number as the RC ?



::

    [blyth@localhost issues]$ scan--
    scan-- : tboolean.py --pfx tboolean-proxy-0 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-1 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-2 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-3 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-4 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-5 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-6 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-7 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-8 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-9 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-10 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-11 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-12 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-13 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-14 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-15 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-16 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-17 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-18 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-19 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-20 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-21 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-22 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-23 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-24 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-25 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-26 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-27 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-28 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-29 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-30 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-31 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-32 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-33 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-34 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-35 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-36 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-37 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-38 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-39 ========== RC 99
    [blyth@localhost issues]$ 



analysis check
-----------------

::





PROXYLV 21 : PMT shape : aligned running
-------------------------------------------

* the photons are incident from the back side and bounce off the neck, making pretty patterns 
  so its challenging 


tp::

    .
    ab.ox_dv
    maxdvmax:0.0670 maxdv:0.0013 0.0048 0.0045 0.0149 0.0078 0.0087 0.0068 0.0002 0.0002 0.0670 0.0001 0.0099 0.0005 0.0020 0.0005 0.0004 0.0020  skip:
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem      nerr:  ferr          mx        mn       avg      
     0000            :                    TO BT BT SA :    8681     8681  :        8681    104172         0: 0.000      0.0013    0.0000    0.0000   :     WARNING :   > dvmax[0] 0.0010  
     0001            :                       TO BR SA :     696      696  :         696      8352         0: 0.000      0.0048    0.0000    0.0001   :     WARNING :   > dvmax[0] 0.0010  
     0002            :                 TO BT BR BT SA :     346      346  :         346      4152         0: 0.000      0.0045    0.0000    0.0001   :     WARNING :   > dvmax[0] 0.0010  
     0003            :              TO BT BR BR BT SA :     174      174  :         174      2088         0: 0.000      0.0149    0.0000    0.0002   :     WARNING :   > dvmax[0] 0.0010  
     0004            :           TO BT BR BR BR BT SA :      54       54  :          54       648         0: 0.000      0.0078    0.0000    0.0002   :     WARNING :   > dvmax[0] 0.0010  
     0005            :           TO BT BR BT BT BT SA :      10       10  :          10       120         0: 0.000      0.0087    0.0000    0.0004   :     WARNING :   > dvmax[0] 0.0010  
     0006            :        TO BT BR BR BT BT BT SA :       8        8  :           8        96         0: 0.000      0.0068    0.0000    0.0004   :     WARNING :   > dvmax[0] 0.0010  
     0007            :                       TO SC SA :       7        7  :           7        84         0: 0.000      0.0002    0.0000    0.0000   :             :  
     0008            :                 TO BT BT SC SA :       5        5  :           5        60         0: 0.000      0.0002    0.0000    0.0000   :             :  
     0009            :           TO BT BR BR BT BR SA :       3        3  :           3        36         3: 0.083      0.0670    0.0000    0.0037   :       ERROR :   > dvmax[1] 0.0200  
     0010            :                       TO BT AB :       3        3  :           3        36         0: 0.000      0.0001    0.0000    0.0000   :             :  
     0011            :        TO BT BR BR BR BR BT SA :       3        3  :           3        36         0: 0.000      0.0099    0.0000    0.0008   :     WARNING :   > dvmax[0] 0.0010  
     0012            :                 TO BT SC BT SA :       3        3  :           3        36         0: 0.000      0.0005    0.0000    0.0000   :             :  
     0013            :           TO BT BT SC BT BT SA :       2        2  :           2        24         0: 0.000      0.0020    0.0000    0.0001   :     WARNING :   > dvmax[0] 0.0010  
     0015            :              TO BR SC BT BT SA :       1        1  :           1        12         0: 0.000      0.0005    0.0000    0.0000   :             :  
     0016            :     TO BT BR BR BR BR BR BT SA :       1        1  :           1        12         0: 0.000      0.0004    0.0000    0.0000   :             :  
     0017            :  TO BT SC BR BR BR BR BR BR BR :       1        1  :           1        12         0: 0.000      0.0020    0.0000    0.0002   :     WARNING :   > dvmax[0] 0.0010  
    .
    ab.rc     .rc  99      [0, 0, 99] 
    ab.rc.c2p .rc   0  .mx   0.001 .cut   1.500/  2.000/  2.500   seqmat_ana :  0.00143  pflags_ana :        0  seqhis_ana :        0   
    ab.rc.rdv .rc   0  .mx   0.072 .cut   0.010/  0.100/  1.000      rpol_dv :  0.00787    rpost_dv :   0.0719   
    ab.rc.pdv .rc  99  .mx   0.067 .cut   0.001/  0.020/  0.100        ox_dv :    0.067   
    .
    [2019-06-20 16:14:24,490] p266034 {tboolean.py:71} CRITICAL -  RC 99 


This multi reflectors "TO BT BR BR BT BR SA" ending up with z-position off::

    In [4]: a.oxa
    Out[4]: 
    A()sliced
    A([[[ 617.6166,  784.5179, -320.8895,   11.3237],
        [   0.5998,    0.7619,   -0.2443,    1.    ],
        [   0.7802,   -0.6247,   -0.0325,  380.    ]],

       [[-784.5179,  160.4182, -181.011 ,   10.5866],
        [  -0.9716,    0.1987,   -0.1286,    1.    ],
        [  -0.1947,   -0.9799,   -0.0428,  380.    ]],

       [[-784.5179, -222.7686,  -54.3965,   10.6167],
        [  -0.961 ,   -0.2729,    0.0441,    1.    ],
        [   0.2656,   -0.9558,   -0.1262,  380.    ]]], dtype=float32)

    In [5]: b.oxa
    Out[5]: 
    A()sliced
    A([[[ 617.6168,  784.5179, -320.8486,   11.3237],
        [   0.5998,    0.7619,   -0.2442,    1.    ],
        [   0.7802,   -0.6247,   -0.0326,  380.    ]],

       [[-784.5179,  160.4182, -180.9848,   10.5866],
        [  -0.9716,    0.1987,   -0.1286,    1.    ],
        [  -0.1947,   -0.9799,   -0.0428,  380.    ]],

       [[-784.5179, -222.7686,  -54.3296,   10.6167],
        [  -0.961 ,   -0.2729,    0.0442,    1.    ],
        [   0.2656,   -0.9558,   -0.1263,  380.    ]]], dtype=float32)

    In [6]: a.oxa - b.oxa
    Out[6]: 
    A()sliced
    A([[[-0.0001, -0.0001, -0.0408,  0.    ],
        [-0.    , -0.    , -0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.0001,  0.    ]],

       [[ 0.    ,  0.    , -0.0262,  0.    ],
        [ 0.    , -0.    , -0.    ,  0.    ],
        [ 0.    , -0.    ,  0.    ,  0.    ]],

       [[ 0.    ,  0.    , -0.067 , -0.    ],               ###### 
        [-0.    , -0.    , -0.0001,  0.    ],
        [ 0.    , -0.    ,  0.    ,  0.    ]]], dtype=float32)


Looking in the visualization *tv* see that those three photons manage to 
bounce around inside the polycone neck. 

::

    In [1]: ab.aselhis = "TO BT BT SA"          # set history selection, in aligned mode

    In [2]: a.oxa.shape
    Out[2]: (8681, 3, 4)

    In [3]: b.oxa.shape
    Out[3]: (8681, 3, 4)

    In [4]: 8681*12
    Out[4]: 104172          ## nelem 


    In [8]: d = a.oxa - b.oxa ; d
    Out[8]: 
    A()sliced
    A([[[-0.0001,  0.0004,  0.    ,  0.    ],
        [-0.    ,  0.    , -0.    ,  0.    ],
        [-0.    ,  0.    ,  0.    ,  0.    ]],

       [[ 0.    ,  0.    ,  0.0001,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ],
        [ 0.    ,  0.    , -0.    ,  0.    ]],

       [[-0.0002,  0.0002,  0.    ,  0.    ],
        [-0.    ,  0.    , -0.    ,  0.    ],
        [ 0.    ,  0.    ,  0.    ,  0.    ]],

The larger numbers of position and time give the deviations rather than 
direction and polarization with values in 0. to 1.:: 


    In [10]: a.oxa[:2]
    Out[10]: 
    A()sliced
    A([[[-120.5809,  369.9754,  784.5179,    6.6946],
        [  -0.1466,    0.4497,    0.8811,    1.    ],
        [  -0.0092,   -0.8913,    0.4534,  380.    ]],

       [[   0.8022,    0.0085,  784.518 ,    6.6567],
        [   0.0896,    0.0009,    0.996 ,    1.    ],
        [   0.    ,   -1.    ,    0.001 ,  380.    ]]], dtype=float32)

    In [11]: b.oxa[:2]
    Out[11]: 
    A()sliced
    A([[[-120.5807,  369.975 ,  784.5179,    6.6946],
        [  -0.1466,    0.4497,    0.8811,    1.    ],
        [  -0.0092,   -0.8913,    0.4534,  380.    ]],

       [[   0.8021,    0.0085,  784.5179,    6.6567],
        [   0.0896,    0.0009,    0.996 ,    1.    ],
        [  -0.    ,   -1.    ,    0.001 ,  380.    ]]], dtype=float32)



Largest deviations in direction and polarization less than 2e-6 level:: 

    In [15]: (d[:,1:].max(), d[:,1:].min())
    Out[15]: 
    (A()sliced
     A(0., dtype=float32), A()sliced
     A(-0., dtype=float32))

    In [16]: np.set_printoptions(suppress=False)

    In [17]: (d[:,1:].max(), d[:,1:].min())
    Out[17]: 
    (A()sliced
     A(1.3439e-06, dtype=float32), A()sliced
     A(-1.5851e-06, dtype=float32))



Hmm need different deviation cuts for the position and time than 
for direction and polarization OR do relative cuts ? Relative to domain
extent perhaps. Going relative to each value is a recipe for problems.

Its simpler to explain and understand fixed absolute cuts ? 
Hmm need to try with some big geometry. 







Issue : many ana fails from deviations
-----------------------------------------

* this is non-aligned comparison : 
  so it relies on accidental history aligment 
  making it suffer from poor stats  

  * also was not using "--reflectcheat" so it really has little hope 


::

    [blyth@localhost okg4]$ scan--
    scan-- : env PROXYLV=0 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=1 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=2 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=3 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=4 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=5 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=6 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=7 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=8 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=9 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=10 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=11 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=12 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=13 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=14 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=15 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=16 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=17 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=18 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=19 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=20 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=21 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=22 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=23 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=24 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=25 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=26 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=27 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=28 tboolean.sh --compute ========== RC 77
    scan-- : env PROXYLV=29 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=30 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=31 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=32 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=33 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=34 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=35 tboolean.sh --compute ========== RC 99
    scan-- : env PROXYLV=36 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=37 tboolean.sh --compute ========== RC 88
    scan-- : env PROXYLV=38 tboolean.sh --compute ========== RC 0
    scan-- : env PROXYLV=39 tboolean.sh --compute ========== RC 0


Duplicate that with the analysis RC as expected using all the events in /tmp/tboolean-proxy-0 etc..

::

    [blyth@localhost okg4]$ scan--
    scan-- : tboolean.py --pfx tboolean-proxy-0 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-1 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-2 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-3 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-4 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-5 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-6 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-7 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-8 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-9 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-10 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-11 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-12 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-13 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-14 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-15 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-16 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-17 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-18 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-19 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-20 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-21 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-22 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-23 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-24 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-25 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-26 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-27 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-28 ========== RC 77
    scan-- : tboolean.py --pfx tboolean-proxy-29 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-30 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-31 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-32 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-33 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-34 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-35 ========== RC 99
    scan-- : tboolean.py --pfx tboolean-proxy-36 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-37 ========== RC 88
    scan-- : tboolean.py --pfx tboolean-proxy-38 ========== RC 0
    scan-- : tboolean.py --pfx tboolean-proxy-39 ========== RC 0



Jump into ipython to check the those deviations and peruse the metadata with::

    tp(){  tboolean-;PROXYLV=18 tboolean-proxy-ip $* ; }



::

    [blyth@localhost ggeo]$ GMeshLibTest 
    ,,,
    2019-06-18 09:23:30.638 INFO  [416436] [test_dump1@103]  num_mesh 41
     0                       Upper_LS_tube0x5b2e9f0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  0
     1                    Upper_Steel_tube0x5b2eb10 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  1
     2                    Upper_Tyvek_tube0x5b2ec30 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  2
     3                       Upper_Chimney0x5b2e8e0 ce0           0.0000,0.0000,0.0000,1750.0000 ce1           0.0000,0.0000,0.0000,1750.0000  3
     4                                sBar0x5b34ab0 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  4
     5                                sBar0x5b34920 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  5
     6                         sModuleTape0x5b34790 ce0           0.0000,0.0000,0.0000,3430.0000 ce1           0.0000,0.0000,0.0000,3430.0000  6
     7                             sModule0x5b34600 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  7
     8                              sPlane0x5b34470 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  8
     9                               sWall0x5b342e0 ce0           0.0000,0.0000,0.0000,3430.6001 ce1           0.0000,0.0000,0.0000,3430.6001  9
    10                              sAirTT0x5b34000 ce0          0.0000,0.0000,0.0000,24000.0000 ce1          0.0000,0.0000,0.0000,24000.0000 10
    11                            sExpHall0x4bcd390 ce0          0.0000,0.0000,0.0000,24000.0000 ce1          0.0000,0.0000,0.0000,24000.0000 11
    12                            sTopRock0x4bccfc0 ce0          0.0000,0.0000,0.0000,27000.0000+ce1          0.0000,0.0000,0.0000,27000.0000 12
    13                             sTarget0x4bd4340 ce0         0.0000,0.0000,60.0000,17760.0000 ce1          0.0000,0.0000,0.0000,17760.0000 13
    14                            sAcrylic0x4bd3cd0 ce0          0.0000,0.0000,0.0000,17820.0000 ce1          0.0000,0.0000,0.0000,17820.0000 14
    15                              sStrut0x4bd4b80 ce0            0.0000,0.0000,0.0000,600.0000+ce1            0.0000,0.0000,0.0000,600.0000 15
    16                          sFasteners0x4c01080 ce0          0.0000,0.0000,-92.5000,150.0000+ce1            0.0000,0.0000,0.0000,150.0000 16
    17                               sMask0x4ca38d0 ce0          0.0000,0.0000,-78.9500,274.9500+ce1            0.0000,0.0000,0.0000,274.9500 17
    18             PMT_20inch_inner1_solid0x4cb3610 ce0           0.0000,0.0000,89.5000,249.0000-ce1            0.0000,0.0000,0.0000,249.0000 18
    19             PMT_20inch_inner2_solid0x4cb3870 ce0         0.0000,0.0000,-167.0050,249.0000-ce1            0.0000,0.0000,0.0000,249.0000 19
    20               PMT_20inch_body_solid0x4c90e50 ce0          0.0000,0.0000,-77.5050,261.5050-ce1            0.0000,0.0000,0.0000,261.5050 20
    21                PMT_20inch_pmt_solid0x4c81b40 ce0          0.0000,0.0000,-77.5050,261.5060-ce1           0.0000,0.0000,-0.0000,261.5060 21
    22                       sMask_virtual0x4c36e10 ce0          0.0000,0.0000,-79.0000,275.0500+ce1            0.0000,0.0000,0.0000,275.0500 22
    23   PMT_3inch_inner1_solid_ell_helper0x510ae30 ce0            0.0000,0.0000,14.5216,38.0000+ce1             0.0000,0.0000,0.0000,38.0000 23
    24   PMT_3inch_inner2_solid_ell_helper0x510af10 ce0            0.0000,0.0000,-4.4157,38.0000+ce1             0.0000,0.0000,0.0000,38.0000 24
    25 PMT_3inch_body_solid_ell_ell_helper0x510ada0 ce0             0.0000,0.0000,4.0627,40.0000+ce1             0.0000,0.0000,0.0000,40.0000 25
    26                PMT_3inch_cntr_solid0x510afa0 ce0           0.0000,0.0000,-45.8740,29.9995+ce1             0.0000,0.0000,0.0000,29.9995 26
    27                 PMT_3inch_pmt_solid0x510aae0 ce0           0.0000,0.0000,-17.9373,57.9383-ce1             0.0000,0.0000,0.0000,57.9383 27
    28                     sChimneyAcrylic0x5b310c0 ce0            0.0000,0.0000,0.0000,520.0000-ce1            0.0000,0.0000,0.0000,520.0000 28
    29                          sChimneyLS0x5b312e0 ce0           0.0000,0.0000,0.0000,1965.0000+ce1           0.0000,0.0000,0.0000,1965.0000 29
    30                       sChimneySteel0x5b314f0 ce0           0.0000,0.0000,0.0000,1665.0000-ce1           0.0000,0.0000,0.0000,1665.0000 30
    31                          sWaterTube0x5b30eb0 ce0           0.0000,0.0000,0.0000,1965.0000+ce1           0.0000,0.0000,0.0000,1965.0000 31
    32                        svacSurftube0x5b3bf50 ce0              0.0000,0.0000,0.0000,4.0000+ce1              0.0000,0.0000,0.0000,4.0000 32
    33                           sSurftube0x5b3ab80 ce0              0.0000,0.0000,0.0000,5.0000+ce1              0.0000,0.0000,0.0000,5.0000 33
    34                         sInnerWater0x4bd3660 ce0        0.0000,0.0000,850.0000,20900.0000-ce1          0.0000,0.0000,0.0000,20900.0000 34
    35                      sReflectorInCD0x4bd3040 ce0        0.0000,0.0000,849.0000,20901.0000-ce1          0.0000,0.0000,0.0000,20901.0000 35
    36                     sOuterWaterPool0x4bd2960 ce0          0.0000,0.0000,0.0000,21750.0000-ce1          0.0000,0.0000,0.0000,21750.0000 36
    37                         sPoolLining0x4bd1eb0 ce0         0.0000,0.0000,-1.5000,21753.0000-ce1          0.0000,0.0000,0.0000,21753.0000 37
    38                         sBottomRock0x4bcd770 ce0      0.0000,0.0000,-1500.0000,24750.0000+ce1          0.0000,0.0000,0.0000,24750.0000 38
    39                              sWorld0x4bc2350 ce0          0.0000,0.0000,0.0000,60000.0000+ce1          0.0000,0.0000,0.0000,60000.0000 39

    40                          sFasteners0x4c01080 ce0          0.0000,0.0000,-92.5000,150.0000 ce1            0.0000,0.0000,0.0000,150.0000 40





Non-aligned deviation checking reminder
---------------------------------------------


Take a look at tboolean-proxy-18::

    tp(){  tboolean-;PROXYLV=18 tboolean-proxy-ip $* ; }


Vague recollection, non-aligned deviation checking relies
on "accidental" history alignment. The problem with this is 
that you rapidly get very low statistics to compare.  

* :doc:



::

    In [22]: ab
    Out[22]: 
    AB(1,torch,tboolean-proxy-18)  None 0 
    A tboolean-proxy-18/tboolean-proxy-18/torch/  1 :  20190617-2331 maxbounce:9 maxrec:10 maxrng:3000000 /tmp/tboolean-proxy-18/evt/tboolean-proxy-18/torch/1/fdom.npy () 
    B tboolean-proxy-18/tboolean-proxy-18/torch/ -1 :  20190617-2331 maxbounce:9 maxrec:10 maxrng:3000000 /tmp/tboolean-proxy-18/evt/tboolean-proxy-18/torch/-1/fdom.npy (recstp) 
    tboolean-proxy-18



    In [16]: av,bv = ab.ox_dv.dvs[2].av, ab.ox_dv.dvs[2].bv

    In [17]: av.shape
    Out[17]: (27, 3, 4)

    In [18]: bv.shape
    Out[18]: (27, 3, 4)

    In [19]: av[0]
    Out[19]: 
    A()sliced
    A([[-451.8267, -183.0164, -747.0001,    7.0898],
       [  -0.5488,   -0.2223,   -0.8058,    1.    ],
       [   0.6132,   -0.7622,   -0.2074,  380.    ]], dtype=float32)

    In [20]: bv[0]
    Out[20]: 
    A()sliced
    A([[-451.8271, -183.0166, -747.    ,    7.0898],
       [  -0.5488,   -0.2223,   -0.8058,    1.    ],
       [   0.6132,   -0.7622,   -0.2074,  380.    ]], dtype=float32)

    In [21]: ab.ox_dv
    Out[21]: 
    ab.ox_dv maxdvmax: 0.00238 maxdv:0.0001221        0  0.00238  skip:SC AB RE
      idx        msg :                            sel :    lcu1     lcu2  :       nitem     nelem/    ndisc: fdisc  mx/mn/av        mx/       mn/      avg  eps:eps    
     0000            :                    TO BT BT SA :    8794     8794  :        7710     92520/        0: 0.000  mx/mn/av 0.0001221/        0/3.652e-06  eps:0.0002    
     0001            :                       TO BR SA :     580      617  :          33       396/        0: 0.000  mx/mn/av         0/        0/        0  eps:0.0002    
     0002            :                 TO BT BR BT SA :     561      527  :          27       324/       14: 0.043  mx/mn/av   0.00238/        0/3.823e-05  eps:0.0002    





