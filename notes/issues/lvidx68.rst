lvidx68 : in at position 5 : 320 mm : SstTopHub0xc2643d8 : TODO : difference out the inner cy
===============================================================================================

Summary
----------

A union of two polycones

* it has an *rmin* that didnt get translated : but my max.z 0 looks correct ?

* also top cylinder looks to extend all way up to the SST?  

* looks like g4poly bug... from coincident faces ?

  * or perhaps, my processing is failing to handle cleaved meshes ?

* TODO: difference out the inner cylinder



320 mm
-----------

::

           320                      SstTopHub0xc2643d8 lvidx  68 nsp    317 
           amn (   -220.500  -220.500  -340.000) 
           bmn (   -220.500  -220.500  -340.000) 
           dmn (      0.000     0.000     0.000) 
           amx (    220.500   220.500     0.000)   <--  parsurfbb.max.z is 320 above the g4poly.max.z that entirely missed the top cylinder
           bmx (    220.500   220.500  -320.000) 
           dmx (      0.000     0.000   320.000)



g4poly bbox vs parsurf bbox
---------------------------

::

    op --gltf 44           # dump the compare meshes table

    // :set nowrap
    2017-07-06 18:06:23.849 INFO  [3696358] [GScene::compareMeshes_GMeshBB@435] GScene::compareMeshes_GMeshBB num_meshes 249 cut 0.1 bbty CSG_BBOX_PARSURF parsurf_level 2 parsurf_target 200
       3869.75               RadialShieldUnit0xc3d7da8 lvidx  56 nsp    507 amn (  -2262.150 -2262.150  -498.500) bmn (   1607.600     0.000  -498.500) dmn (  -3869.750 -2262.150     0.000) amx (   2262.150  2262.150   498.500) bmx (   2262.150  1589.370   498.500) dmx (      0.000   672.780     0.000)
       3407.72               SstBotCirRibBase0xc26e2d0 lvidx  65 nsp    324 amn (  -2000.000 -2000.000  -215.000) bmn (   1407.720    12.467  -215.000) dmn (  -3407.720 -2012.468     0.000) amx (   1847.759  2000.000   215.000) bmx (   1998.360  1404.240   215.000) dmx (   -150.601   595.760     0.000)
       2074.65               SstTopCirRibBase0xc264f78 lvidx  69 nsp    352 amn (  -1220.000 -1220.000  -115.945) bmn (    854.653    10.020  -115.945) dmn (  -2074.653 -1230.020     0.000) amx (   1220.000  1220.000   115.945) bmx (   1218.680   854.688   115.945) dmx (      1.320   365.312     0.000)
        345.51                SstTopRadiusRib0xc271720 lvidx  66 nsp    408 amn (   -345.000   -10.000 -1114.250) bmn (   -345.510   -10.000 -1114.250) dmn (      0.510     0.000     0.000) amx (      0.000    10.000  1114.250) bmx (    345.510    10.000  1114.250) dmx (   -345.510     0.000     0.000)
           320                      SstTopHub0xc2643d8 lvidx  68 nsp    317 amn (   -220.500  -220.500  -340.000) bmn (   -220.500  -220.500  -340.000) dmn (      0.000     0.000     0.000) amx (    220.500   220.500     0.000) bmx (    220.500   220.500  -320.000) dmx (      0.000     0.000   320.000)
       28.0747              OcrGdsTfbInLsoOfl0xc2b5ba0 lvidx  83 nsp    243 amn (    -32.500   -32.500  -247.488) bmn (    -32.500   -32.500  -219.413) dmn (      0.000     0.000   -28.075) amx (     32.500    32.500   247.488) bmx (     32.500    32.500   247.488) dmx (      0.000     0.000    -0.000)
       26.2183                   OcrGdsLsoPrt0xc104978 lvidx  81 nsp    342 amn (    -98.000   -98.000     0.000) bmn (    -98.000   -98.000    26.218) dmn (      0.000     0.000   -26.218) amx (     98.000    98.000   214.596) bmx (     98.000    98.000   214.596) dmx (      0.000     0.000     0.000)
            20               headon-pmt-mount0xc2a7670 lvidx  55 nsp    365 amn (    -51.500   -51.500  -120.000) bmn (    -36.850   -36.850  -100.000) dmn (    -14.650   -14.650   -20.000) amx (     51.500    51.500   100.000) bmx (     36.850    36.850   100.000) dmx (     14.650    14.650     0.000)
            12           near_side_long_hbeam0xbf3b5d0 lvidx  17 nsp    450 amn (  -2000.000  -100.000  -147.000) bmn (  -2000.000   -99.876  -135.000) dmn (      0.000    -0.124   -12.000) amx (   2000.000   100.000   147.000) bmx (   2000.070   100.124   146.908) dmx (     -0.070    -0.124     0.092)
        10.035                   weight-shell0xc307920 lvidx 103 nsp    567 amn (    -10.035   -10.035   -28.510) bmn (    -10.035   -10.035   -18.475) dmn (      0.000     0.000   -10.035) amx (     10.035    10.035    28.510) bmx (     10.035    10.035    18.475) dmx (      0.000     0.000    10.035)
        10.035        AmCCo60AcrylicContainer0xc0b23b8 lvidx 131 nsp    219 amn (    -10.035   -10.035   -24.900) bmn (    -10.035   -10.035   -14.865) dmn (      0.000     0.000   -10.035) amx (     10.035    10.035    24.900) bmx (     10.035    10.036    24.899) dmx (     -0.000    -0.001     0.000)
        10.035                   source-shell0xc2d62d0 lvidx 111 nsp    567 amn (    -10.035   -10.035   -24.900) bmn (    -10.035   -10.035   -14.865) dmn (      0.000     0.000   -10.035) amx (     10.035    10.035    24.900) bmx (     10.035    10.035    14.865) dmx (      0.000     0.000    10.035)
        10.035               led-source-shell0xc3068f0 lvidx 100 nsp    567 amn (    -10.035   -10.035   -24.900) bmn (    -10.035   -10.035   -14.865) dmn (      0.000     0.000   -10.035) amx (     10.035    10.035    24.900) bmx (     10.035    10.035    14.865) dmx (      0.000     0.000    10.035)
       8.09241                    OcrGdsInLso0xbfa2190 lvidx  31 nsp    287 amn (    485.123 -1278.737  -242.962) bmn (    485.131 -1278.720  -251.054) dmn (     -0.008    -0.017     8.092) amx (    548.123 -1215.737   194.127) bmx (    548.131 -1215.720   195.139) dmx (     -0.008    -0.017    -1.012)


    2017-07-06 19:19:15.926 INFO  [3717808] [GScene::compareMeshes_GMeshBB@435] GScene::compareMeshes_GMeshBB num_meshes 249 cut 0.1 bbty CSG_BBOX_PARSURF parsurf_level 2 parsurf_target 200
       3869.75               RadialShieldUnit0xc3d7da8 lvidx  56 nsp    507                             intersection cylinder   nds[ 64]  4393 4394 4395 4396 4397 4398 4399 4400 4401 4402 ... 
       3407.72               SstBotCirRibBase0xc26e2d0 lvidx  65 nsp    324                          difference cylinder box3   nds[ 16]  4440 4441 4442 4443 4444 4445 4446 4447 6100 6101 ... 
       2074.65               SstTopCirRibBase0xc264f78 lvidx  69 nsp    352                        intersection cylinder box3   nds[ 16]  4465 4466 4467 4468 4469 4470 4471 4472 6125 6126 ... 
        345.51                SstTopRadiusRib0xc271720 lvidx  66 nsp    408                  difference box3 convexpolyhedron   nds[ 16]  4448 4449 4450 4451 4452 4453 4454 4455 6108 6109 ... 
           320                      SstTopHub0xc2643d8 lvidx  68 nsp    317                                    union cylinder   nds[  2]  4464 6124 . 
       28.0747              OcrGdsTfbInLsoOfl0xc2b5ba0 lvidx  83 nsp    243                          difference cylinder cone   nds[  2]  4515 6175 . 
       26.2183                   OcrGdsLsoPrt0xc104978 lvidx  81 nsp    342                    union difference cylinder cone   nds[  2]  4511 6171 . 
            20               headon-pmt-mount0xc2a7670 lvidx  55 nsp    365                         union difference cylinder   nds[ 12]  4357 4364 4371 4378 4385 4392 6017 6024 6031 6038 ... 
            12           near_side_long_hbeam0xbf3b5d0 lvidx  17 nsp    450                                        union box3   nds[  8]  2436 2437 2615 2616 2794 2795 2973 2974 . 
        10.035                   weight-shell0xc307920 lvidx 103 nsp    567                            union zsphere cylinder   nds[ 36]  4543 4547 4558 4562 4591 4595 4631 4635 4646 4650 ... 
        10.035        AmCCo60AcrylicContainer0xc0b23b8 lvidx 131 nsp    219                             union sphere cylinder   nds[  6]  4567 4655 4737 6227 6315 6397 . 
        10.035                   source-shell0xc2d62d0 lvidx 111 nsp    567                            union zsphere cylinder   nds[  6]  4552 4640 4722 6212 6300 6382 . 
        10.035               led-source-shell0xc3068f0 lvidx 100 nsp    567                            union zsphere cylinder   nds[  6]  4541 4629 4711 6201 6289 6371 . 
       8.09241                    OcrGdsInLso0xbfa2190 lvidx  31 nsp    287             intersection difference cylinder cone   nds[  2]  3168 4828 . 




opticks-tbool-vi 68
----------------------

* cylinder on a pedestal 

::

     78 # generated by tboolean.py : 20170706-1446 
     79 # opticks-;opticks-tbool 68 
     80 # opticks-;opticks-tbool-vi 68 
     81 
     82 
     83 a = CSG("cylinder", param = [0.000,0.000,0.000,220.500],param1 = [-340.000,-320.000,0.000,0.000])
               ## wider base 
     84 b = CSG("cylinder", param = [0.000,0.000,0.000,170.500],param1 = [-320.000,0.000,0.000,0.000])
               ## thinner top : note coincident face at z=-320

     85 b.transform = [[1.000,0.000,0.000,0.000],[0.000,1.000,0.000,0.000],[0.000,0.000,1.000,0.000],[0.000,0.000,0.000,1.000]]
     86 ab = CSG("union", left=a, right=b)
     87 
     88 
     89 
     90 obj = ab
     91 
     92 con = CSG("sphere",  param=[0,0,0,10], container="1", containerscale="2", boundary=args.container , poly="IM", resolution="20" )
     93 CSG.Serialize([con, obj], args.csgpath )



A union of two polycones

* ahha it has an *rmin* that didnt get translated : but my max.z 0 looks correct ?

::

     1062     <polycone aunit="deg" deltaphi="360" lunit="mm" name="SstTopHubBot0xc2635b8" startphi="0">
     1063       <zplane rmax="220.5" rmin="150.5" z="-340"/>
     1064       <zplane rmax="220.5" rmin="150.5" z="-320"/>
     1065     </polycone>
     1066     <polycone aunit="deg" deltaphi="360" lunit="mm" name="SstTopHubMain0xc263d80" startphi="0">
     1067       <zplane rmax="170.5" rmin="150.5" z="-320"/>
     1068       <zplane rmax="170.5" rmin="150.5" z="0"/>
     1069     </polycone>
     1070     <union name="SstTopHub0xc2643d8">
     1071       <first ref="SstTopHubBot0xc2635b8"/>
     1072       <second ref="SstTopHubMain0xc263d80"/>
     1073     </union>



op --dlv68 --gltf 3
-----------------------

::

    op --dlv68 --gltf 3  

    ~/opticks_refs/lvidx68_gltf_3_g4poly_misses_top_cylinder.png




