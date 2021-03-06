Material Code Mapping Generalization
=======================================

Persisted gensteps contain material indices, in order to 
map these to actual materials it is necessary to have 
a code to material name mapping. 



env/chroma/G4DAEChroma
------------------------- 

* gdc-
* ~/env/chroma/G4DAEChroma

NB the ChromaMaterialMap.json is an input to the generation of gensteps, 
the actual values of the codes are immaterial... so long as the map covers
all materials and the codea are unique.



Mapping Codes ?
-----------------

::

    delta:ggeo blyth$ op --j1707 --bnd
    === op-cmdline-binary-match : finds 1st argument with associated binary : --bnd
    ubin /usr/local/opticks/lib/GBndLibTest cfm --bnd cmdline --j1707 --bnd
    === op-export : OPTICKS_BINARY /usr/local/opticks/lib/GBndLibTest
    248 -rwxr-xr-x  1 blyth  staff  126532 Sep 11 15:35 /usr/local/opticks/lib/GBndLibTest
    proceeding.. : /usr/local/opticks/lib/GBndLibTest --j1707 --bnd
    2017-09-11 20:13:15.746 INFO  [1207704] [main@28] /usr/local/opticks/lib/GBndLibTest
    2017-09-11 20:13:15.748 INFO  [1207704] [main@32]  ok 
    2017-09-11 20:13:15.748 INFO  [1207704] [main@36]  loaded blib 
    2017-09-11 20:13:15.749 INFO  [1207704] [GMaterialLib::postLoadFromCache@67] GMaterialLib::postLoadFromCache  nore 0 noab 0 nosc 0 xxre 0 xxab 0 xxsc 0 fxre 0 fxab 0 fxsc 0 groupvel 1
    2017-09-11 20:13:15.749 INFO  [1207704] [GMaterialLib::replaceGROUPVEL@552] GMaterialLib::replaceGROUPVEL  ni 15
    2017-09-11 20:13:15.749 INFO  [1207704] [GPropertyLib::getIndex@338] GPropertyLib::getIndex type GMaterialLib TRIGGERED A CLOSE  shortname [Acrylic]
    2017-09-11 20:13:15.749 INFO  [1207704] [GPropertyLib::close@384] GPropertyLib::close type GMaterialLib buf 15,2,39,4
    2017-09-11 20:13:15.750 INFO  [1207704] [main@40]  loaded all  blib 0x7fc7bb404a30 mlib 0x7fc7bb4055f0 slib 0x7fc7bb427200
    2017-09-11 20:13:15.750 INFO  [1207704] [GBndLib::dump@830] GBndLib::dump
    2017-09-11 20:13:15.750 INFO  [1207704] [GBndLib::dump@832] GBndLib::dump ni 27
     (  0) om:                 Galactic os:                                is:                                im:                 Galactic ( 3,-1,-1, 3)
     (  1) om:                 Galactic os:                                is:                                im:                     Rock ( 3,-1,-1, 6)
     (  2) om:                     Rock os:                                is:                                im:                      Air ( 6,-1,-1, 1)
     (  3) om:                      Air os:                                is:                                im:                      Air ( 1,-1,-1, 1)
     (  4) om:                      Air os:                                is:                                im:                       LS ( 1,-1,-1, 4)
     (  5) om:                      Air os:                                is:                                im:                    Steel ( 1,-1,-1, 7)
     (  6) om:                      Air os:                                is:                                im:                    Tyvek ( 1,-1,-1, 9)
     (  7) om:                     Rock os:                                is:                                im:                    Tyvek ( 6,-1,-1, 9)
     (  8) om:                    Tyvek os:                                is:                                im:                vetoWater ( 9,-1,-1,12)
     (  9) om:                vetoWater os:                 CDTyvekSurface is:                                im:                    Tyvek (12, 0,-1, 9)
     ( 10) om:                    Tyvek os:                                is:                                im:                    Water ( 9,-1,-1,11)
     ( 11) om:                    Water os:                                is:                                im:                  Acrylic (11,-1,-1, 0)
     ( 12) om:                  Acrylic os:                                is:                                im:                       LS ( 0,-1,-1, 4)
     ( 13) om:                    Water os:                                is:                                im:                    Steel (11,-1,-1, 7)
     ( 14) om:                    Water os:                                is:                                im:                   Copper (11,-1,-1, 2)
     ( 15) om:                    Water os:                                is:                                im:                    Water (11,-1,-1,11)
     ( 16) om:                    Water os:                                is:                                im:                    Pyrex (11,-1,-1, 5)
     ( 17) om:                    Pyrex os:                                is:                                im:                    Pyrex ( 5,-1,-1, 5)
     ( 18) om:                    Pyrex os:PMT_20inch_photocathode_logsurf2 is:                                im:                   Vacuum ( 5, 4,-1,10)
     ( 19) om:                    Pyrex os:                                is:     PMT_20inch_mirror_logsurf1 im:                   Vacuum ( 5,-1, 2,10)
     ( 20) om:                    Pyrex os:PMT_3inch_photocathode_logsurf2 is:                                im:                   Vacuum ( 5, 8,-1,10)
     ( 21) om:                    Pyrex os:                                is:      PMT_3inch_absorb_logsurf1 im:                   Vacuum ( 5,-1, 5,10)
     ( 22) om:                    Water os:                                is:                                im:                    Tyvek (11,-1,-1, 9)
     ( 23) om:                    Water os:                                is:                                im:                       LS (11,-1,-1, 4)
     ( 24) om:                       LS os:                                is:                                im:                    Steel ( 4,-1,-1, 7)
     ( 25) om:                    Water os:                      Tube_surf is:                                im:                   Teflon (11, 9,-1, 8)
     ( 26) om:                   Teflon os:                                is:                                im:                   Vacuum ( 8,-1,-1,10)
    2017-09-11 20:13:15.751 INFO  [1207704] [int>::save@633] NPYBase::save creating directories [/tmp/blyth/opticks/GBndLib]/tmp/blyth/opticks/GBndLib/GBndLibIndex.npy
    2017-09-11 20:13:15.751 INFO  [1207704] [int>::save@636] NPYBase::save created directories [/tmp/blyth/opticks/GBndLib]
    2017-09-11 20:13:15.751 INFO  [1207704] [GPropertyLib::close@384] GPropertyLib::close type GBndLib buf 27,4,2,39,4
    2017-09-11 20:13:15.752 INFO  [1207704] [BFile::preparePath@462] preparePath : created directory /tmp/blyth/opticks/GItemList
    2017-09-11 20:13:15.752 INFO  [1207704] [main@59]  after blib saveToCache 
    2017-09-11 20:13:15.752 INFO  [1207704] [main@61]  after blib saveOpticalBuffer 
    /Users/blyth/opticks/bin/op.sh RC 0
    delta:ggeo blyth$ 



    delta:ggeo blyth$ op --j1707 --mat | grep material
    256 -rwxr-xr-x  1 blyth  staff  129960 Sep 11 15:35 /usr/local/opticks/lib/GMaterialLibTest
    proceeding.. : /usr/local/opticks/lib/GMaterialLibTest --j1707 --mat
    the required argument for option '--materialprefix' is missing
    2017-09-11 20:18:46.595 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  0       material m:Acrylic k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Acrylic
    2017-09-11 20:18:46.595 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  1       material m:Air k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Air
    2017-09-11 20:18:46.596 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  2       material m:Copper k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Copper
    2017-09-11 20:18:46.596 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  3       material m:Galactic k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Galactic
    2017-09-11 20:18:46.597 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  4       material m:LS k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w LS
    2017-09-11 20:18:46.597 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  5       material m:Pyrex k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Pyrex
    2017-09-11 20:18:46.598 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  6       material m:Rock k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Rock
    2017-09-11 20:18:46.598 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  7       material m:Steel k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Steel
    2017-09-11 20:18:46.599 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  8       material m:Teflon k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Teflon
    2017-09-11 20:18:46.599 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>::  9       material m:Tyvek k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Tyvek
    2017-09-11 20:18:46.600 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 10       material m:Vacuum k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Vacuum
    2017-09-11 20:18:46.600 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 11       material m:Water k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w Water
    2017-09-11 20:18:46.601 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 12       material m:vetoWater k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w vetoWater
    2017-09-11 20:18:46.601 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 13       material m:GlassSchottF2 k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w GlassSchottF2
    2017-09-11 20:18:46.602 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 14       material m:MainH2OHale k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w MainH2OHale
    2017-09-11 20:18:46.602 WARN  [1209203] [GMaterialLib::dump@701] GPropertyMap<T>:: 15       material m:GlassSchottF2 k:refractive_index absorption_length scattering_length reemission_prob group_velocity extra_y extra_z extra_w GlassSchottF2






Where is genstep path configured ?
----------------------------------


::

    delta:optickscore blyth$ grep cerenkov *.*
    Opticks.cc:    else if(m_cfg->hasOpt("cerenkov"))      code = CERENKOV ;
    OpticksCfg.cc:       ("cerenkov,c",  "load cerenkov gensteps") ;
    OpticksCfg.cc:       ("natural,n",  "load natural gensteps containing a mixture of scintillation and cerenkov steps") ;
    OpticksEvent.cc:    m_abbrev[genstep_] = "gs" ;    // input gs are named: cerenkov, scintillation but for posterity need common output tag
    OpticksEvent.hh:      scintillation or cerenkov
    OpticksEvent.hh:      //    typ: cerenkov/scintillaton/torch/g4gun
    OpticksFlags.cc:const char* OpticksFlags::cerenkov_          = "cerenkov" ;
    OpticksFlags.cc:       case CERENKOV     :name = cerenkov_      ;break;
    OpticksFlags.cc:    else if(strcmp(type,cerenkov_)==0)      code = CERENKOV ;
    OpticksFlags.hh:       static const char* cerenkov_ ;
    delta:optickscore blyth$ 

::

     933 unsigned int Opticks::getSourceCode()
     934 {
     935     unsigned int code ;
     936     if(     m_cfg->hasOpt("natural"))       code = NATURAL ;     // doing (CERENKOV | SCINTILLATION) would entail too many changes 
     937     else if(m_cfg->hasOpt("cerenkov"))      code = CERENKOV ;
     938     else if(m_cfg->hasOpt("scintillation")) code = SCINTILLATION ;
     939     else if(m_cfg->hasOpt("torch"))         code = TORCH ;
     940     else if(m_cfg->hasOpt("machinery"))     code = MACHINERY ;
     941     else if(m_cfg->hasOpt("g4gun"))         code = G4GUN ;
     942     else                                    code = TORCH ;
     943     return code ;
     944 }



::

    054 void OpticksGen::initInputGensteps()
     55 {
     56     if(m_ok->isNoInputGensteps())
     57     {
     58         LOG(warning) << "OpticksGen::initInputGensteps SKIP as isNoInputGensteps " ;
     59         return ;
     60     }
     61 
     62     const char* type = m_ok->getSourceType();
     63     unsigned code = m_ok->getSourceCode();
     64     LOG(debug) << "OpticksGen::initInputGensteps"
     65                << " code " << code
     66                << " type " << type
     67                ;
     68 
     69 
     70     NPY<float>* gs = NULL ;
     71 
     72     if( code == FABRICATED || code == MACHINERY  )
     73     {
     74         m_fabstep = makeFabstep();
     75         gs = m_fabstep->getNPY();
     76     }
     77     else if(code == TORCH)
     78     {
     79         m_torchstep = makeTorchstep() ;
     80         gs = m_torchstep->getNPY();
     81     }
     82     else if( code == CERENKOV || code == SCINTILLATION || code == NATURAL )
     83     {
     84         gs = loadGenstepFile("GS_LOADED,GS_LEGACY");
     85     }
     86     else if( code == G4GUN  )
     87     {
     88         if(m_ok->existsGenstepPath())
     89         {
     90              gs = loadGenstepFile("GS_LOADED");
     91         }
     92         else
     93         {
     94              std::string path = m_ok->getGenstepPath();
     95              LOG(warning) <<  "G4GUN running, but no gensteps at " << path
     96                           << " LIVE G4 is required to provide the gensteps "
     97                           ;
     98         }
     99     }
    100     setInputGensteps(gs);
    101 }


::

    200 NPY<float>* OpticksGen::loadGenstepFile(const char* label)
    201 {
    202     NPY<float>* gs = m_ok->loadGenstep();
    203     if(gs == NULL)
    204     {
    205         LOG(fatal) << "OpticksGen::loadGenstepFile FAILED" ;
    206         m_ok->setExit(true);
    207         return NULL ;
    208     }
    209     gs->setLookup(m_lookup);
    210 
    211     int modulo = m_cfg->getModulo();
    212 
    213     NParameters* parameters = gs->getParameters();
    214     parameters->add<int>("Modulo", modulo );
    215     if(modulo > 0)
    216     {
    217         parameters->add<std::string>("genstepOriginal",   gs->getDigestString()  );
    218         LOG(warning) << "OptickGen::loadGenstepFile applying modulo scaledown " << modulo ;
    219         gs = NPY<float>::make_modulo(gs, modulo);
    220         parameters->add<std::string>("genstepModulo",   gs->getDigestString()  );
    221     }
    222     gs->addActionControl(OpticksActionControl::Parse(label));
    223     return gs ;
    224 }

::

    1221 std::string Opticks::getGenstepPath()
    1222 {
    1223     const char* det = m_spec->getDet();
    1224     const char* typ = m_spec->getTyp();
    1225     const char* tag = m_spec->getTag();
    1226 
    1227     std::string path = NLoad::GenstepsPath(det, typ, tag);
    1228     return path ;
    1229 }
    1230 
    1231 bool Opticks::existsGenstepPath()
    1232 {
    1233     std::string path = getGenstepPath();
    1234     return BFile::ExistsFile(path.c_str());
    1235 }
    1236 
    1237 
    1238 NPY<float>* Opticks::loadGenstep()
    1239 {
    1240     std::string path = getGenstepPath();
    1241     NPY<float>* gs = NPY<float>::load(path.c_str());
    1242     if(!gs)
    1243     {
    1244         LOG(warning) << "Opticks::loadGenstep"
    1245                      << " FAILED TO LOAD GENSTEPS FROM "
    1246                      << " path " << path
    1247                      ;
    1248         return NULL ;
    1249     }
    1250     return gs ;
    1251 }


     08 std::string NLoad::GenstepsPath(const char* det, const char* typ, const char* tag)
      9 {
     10     const char* gensteps_dir = BOpticksResource::GenstepsDir();  // eg /usr/local/opticks/opticksdata/gensteps
     11     BOpticksEvent::SetOverrideEventBase(gensteps_dir) ;
     12     BOpticksEvent::SetLayoutVersion(1) ;
     13 
     14     const char* stem = "" ; // backward compat stem of gensteps
     15     std::string path = BOpticksEvent::path(det, typ, tag, stem, ".npy");
     16 
     17     BOpticksEvent::SetOverrideEventBase(NULL) ;
     18     BOpticksEvent::SetLayoutVersionDefault() ;
     19 
     20     return path ;
     21 }
     22 
     23 NPY<float>* NLoad::Gensteps(const char* det, const char* typ, const char* tag)
     24 {
     25     std::string path = GenstepsPath(det, typ, tag);
     26     NPY<float>* gs = NPY<float>::load(path.c_str()) ;
     27     return gs ;
     28 }


     302 void Opticks::init()
     303 {
     304     m_mode = new OpticksMode(hasArg(COMPUTE_ARG_)) ;
     305 
     306     m_cfg = new OpticksCfg<Opticks>("opticks", this,false);
     307 
     308     m_timer = new Timer("Opticks::");
     309 
     310     m_timer->setVerbose(true);
     311 
     312     m_timer->start();
     313 
     314     m_parameters = new NParameters ;
     315 
     316     m_lastarg = m_argc > 1 ? strdup(m_argv[m_argc-1]) : NULL ;
     317 
     318     m_resource = new OpticksResource(this, m_envprefix, m_lastarg);
     319 
     320     setDetector( m_resource->getDetector() );
     321 
     322     LOG(info) << "Opticks::init DONE " << m_resource->desc()  ;
     323 }




Review Genstep
----------------

::

     07 struct CerenkovStep
      8 {
      9     int Id    ;
     10     int ParentId ;
     11     int MaterialIndex  ;
     12     int NumPhotons ;

     07 struct ScintillationStep
      8 {
      9     int Id    ;
     10     int ParentId ;
     11     int MaterialIndex  ;
     12     int NumPhotons ;
     13 

     07 struct TorchStep
      8 {
      9     
     10     // (0) m_ctrl
     11     int Id    ;
     12     int ParentId ; 
     13     int MaterialIndex  ;
     14     int NumPhotons ;



::

     38 unsigned G4StepNPY::getNumSteps()
     39 {
     40     return m_npy->getShape(0);
     41 }
     42 unsigned G4StepNPY::getNumPhotons(unsigned i)
     43 {
     44     unsigned ni = getNumSteps();
     45     assert(i < ni);
     46     int numPhotons = m_npy->getInt(i,0u,3u);
     47     return numPhotons ;
     48 }
     49 unsigned G4StepNPY::getGencode(unsigned i)
     50 {
     51     unsigned ni = getNumSteps();
     52     assert(i < ni);
     53     unsigned gencode = m_npy->getInt(i,0u,0u);
     54     return gencode  ;
     55 }



Issue : Sep 2017 : Still getting lookup fails 
-----------------------------------------------

Older gensteps from /usr/local/opticks/opticksdata/gensteps/juno/ using 
material indices 48,24,42.

::

    In [89]: c1 = np.load("/usr/local/opticks/opticksdata/gensteps/juno/cerenkov/1.npy")

    In [90]: c1.shape
    Out[90]: (3840, 6, 4)

    In [91]: c1.view(np.int32)[:,0]
    Out[91]: 
    array([[    -1,      1,     48,    322],
           [   -11,      1,     48,    300],
           [   -21,      1,     48,    294],
           ..., 
           [-38371,     12,     48,     11],
           [-38381,      9,     48,     40],
           [-38391,      4,     48,     47]], dtype=int32)

    In [96]: count_unique_sorted( c1[:,0,2].view(np.int32) )
    Out[96]: 
    array([[  48, 3038],
           [  24,  750],
           [  42,   52]], dtype=uint64)


    In [97]: s1 = np.load("/usr/local/opticks/opticksdata/gensteps/juno/scintillation/1.npy")

    In [98]: s1.shape
    Out[98]: (1774, 6, 4)

    In [99]: s1.view(np.int32)[:,0]
    Out[99]: 
    array([[    1,     1,    48,  1032],
           [   51,     1,    48,  1057],
           [  101,     1,    48,   949],
           ..., 
           [88551, 11849,    48,   172],
           [88601, 12296,    48,   176],
           [88651, 12363,    48,    84]], dtype=int32)

    In [100]: count_unique_sorted( s1[:,0,2].view(np.int32) )
    Out[100]: array([[  48, 1774]], dtype=uint64)



Looks to match the dumped $TMP/G4StepNPY_applyLookup_FAIL.npy::

    a = np.load(os.path.expandvars("$TMP/G4StepNPY_applyLookup_FAIL.npy"))

    b = a[:,0,2].view(np.uint32)

    from opticks.ana.nbase import count_unique_sorted

    In [22]: count_unique_sorted(b)
    Out[22]: 
    array([[  48, 3038],
           [  24,  750],
           [  42,   52]], dtype=uint64)

    In [25]: a.shape
    Out[25]: (3840, 6, 4)

    In [26]: a
    Out[26]: 
    array([[[   0.    ,    0.    ,    0.    ,    0.    ],
            [   0.    ,    0.    ,    0.    ,    0.    ],
            [  -0.8609,   -0.1562,   -0.5302,    1.023 ],
            [   0.    ,   -1.    ,    1.    ,  299.7923],
            [   1.    ,    0.    ,    0.    ,    0.6879],
            [   0.5267,  293.2452,  293.2452,    0.    ]],

           [[   0.    ,    0.    ,    0.    ,    0.    ],
            [  -8.4068,   -1.5249,   -5.1779,    0.0333],
            [  -0.8609,   -0.1562,   -0.5302,    1.023 ],
            [   0.    ,   -1.    ,    1.    ,  299.7923],
            [   1.    ,    0.    ,    0.    ,    0.6879],
            [   0.5267,  293.2452,  293.2452,    0.    ]],


    In [56]: a.view(np.uint32)[1800:1850,0]
    Out[56]: 
    array([[  1,   1,  48,  39],
           [  1,   1,  48, 302],
           [  1,   1,  48, 303],
           [  1,   1,  48, 298],
           [  1,   1,  48, 324],
           [  1,   1,  48,  60],
           [  1,   1,  48, 316],
           [  1,   1,  48,  20],
           [  1,   1,  48, 293],
           [  1,   1,  48, 322],
           [  1,   1,  48, 298],
           [  1,   1,  48, 261],
           [  1,   1,  48, 324],
           [  1,   1,  48, 287],
           [  1,   1,  48, 321],
           [  1,   1,  48, 328],
           [  1,   1,  48, 288],
           [  1,   1,  42, 283],
           [  1,   1,  42, 292],
           [  1,   1,  42, 307],
           [  1,   1,  42, 124],
           [  1,   1,  42, 317],
           [  1,   1,  42,  45],
           [  1,   1,  42,  69],
           [  1,   1,  42, 291],
           [  1,   1,  42, 304],
           [  1,   1,  42, 276],
           [  1,   1,  42, 318],
           [  1,   1,  42, 278],
           [  1,   1,  42, 334],
           [  1,   1,  24, 293],
           [  1,   1,  24, 287],
           [  1,   1,  24, 320],
           [  1,   1,  24, 290],
           [  1,   1,  24, 306],
           [  1,   1,  24, 278],
           [  1,   1,  24, 293],
           [  1,   1,  24, 334],
           [  1,   1,  24, 301],
           [  1,   1,  24, 299],
           [  1,   1,  24, 269],
           [  1,   1,  24, 280],
           [  1,   1,  24, 298],
           [  1,   1,  24, 283],
           [  1,   1,  24, 300],
           [  1,   1,  24, 270],
           [  1,   1,  24, 309],
           [  1,   1,  24, 317],
           [  1,   1,  24, 134],
           [  1,   1,  24,  34]], dtype=uint32)
     

    In [60]: a.view(np.uint32)[:,0,0].min()
    Out[60]: 1

    In [61]: a.view(np.uint32)[:,0,0].max()
    Out[61]: 1

    In [62]: 

    In [62]: 

    In [62]: a.view(np.uint32)[:,0,1].max()
    Out[62]: 7780

    In [63]: a.view(np.uint32)[:,0,1].min()
    Out[63]: 1




Newer cerenkov gensteps using different material indices 1,28,27



::

    In [71]: e0 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/cerenkov/csl-evt0.npy")

    In [73]: e0.shape
    Out[73]: (43, 6, 4)

    In [74]: e0.view(np.int32)[:,0]
    Out[74]: 
    array([[    -1,      1,      1,    322],
           [ -1001,    134,      1,    137],
           [ -2001,    268,      1,      2],
           [ -3001,      1,      1,    296],
           [ -4001,      1,      1,    304],
           [ -5001,      1,      1,    312],
           [ -6001,    567,      1,    214],
           [ -7001,    645,      1,    297],
           [ -8001,    993,      1,    228],
           [ -9001,      1,      1,    278],
           [-10001,      1,      1,    291],
           [-11001,      1,      1,    285],
           [-12001,      1,      1,    322],
           [-13001,      1,      1,    301],
           [-14001,      1,      1,    308],
           [-15001,   1914,      1,      1],
           [-16001,      1,      1,    327],
           [-17001,   2098,      1,    281],
           [-18001,   2096,      1,    324],
           [-19001,   2526,      1,    319],
           [-20001,   3183,      1,     18],
           [-21001,   3514,      1,    299],
           [-22001,   3949,      1,    313],
           [-23001,   4399,      1,    303],
           [-24001,      1,      1,    314],
           [-25001,      1,      1,    316],
           [-26001,      1,      1,    286],
           [-27001,   4966,      1,     33],
           [-28001,      1,      1,    331],
           [-29001,      1,      1,    293],
           [-30001,   5216,      1,    304],
           [-31001,   5537,      1,     11],
           [-32001,      1,      1,    318],
           [-33001,      1,     28,    263],
           [-34001,      1,     28,    279],
           [-35001,      1,     27,    317],
           [-36001,      1,     27,    266],
           [-37001,      1,     27,    301],
           [-38001,      1,     27,    294],
           [-39001,   6315,     27,     62],
           [-40001,   5975,     27,    275],
           [-41001,   5903,     28,    317],
           [-42001,   9479,     28,    326]], dtype=int32)


    In [75]: e1 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/cerenkov/csl-evt1.npy")

    In [76]: e1.shape
    Out[76]: (36, 6, 4)

    In [77]: e1.view(np.int32)[:,0]
    Out[77]: 
    array([[    -1,      1,      1,    299],
           [ -1001,      1,      1,    329],
           [ -2001,    185,      1,    286],
           [ -3001,      1,      1,    308],
           [ -4001,      1,      1,    299],
           [ -5001,      1,      1,    347],
           [ -6001,      1,      1,    301],
           [ -7001,      1,      1,    222],
           [ -8001,   1014,      1,    291],
           [ -9001,      1,      1,    313],
           [-10001,      1,      1,    272],
           [-11001,      1,      1,    293],
           [-12001,      1,      1,    297],
           [-13001,      1,      1,    292],
           [-14001,   1792,      1,     93],
           [-15001,      1,      1,    300],
           [-16001,      1,      1,    293],
           [-17001,   2223,      1,    282],
           [-18001,   2150,      1,    315],
           [-19001,      1,      1,    313],
           [-20001,      1,      1,    321],
           [-21001,      1,      1,    259],
           [-22001,   3293,      1,     16],
           [-23001,      1,      1,    309],
           [-24001,      1,      1,    338],
           [-25001,      1,      1,    318],
           [-26001,      1,      1,    308],
           [-27001,      1,      1,    333],
           [-28001,      1,     28,    173],
           [-29001,      1,     28,    302],
           [-30001,      1,     27,    259],
           [-31001,      1,     27,    297],
           [-32001,      1,     27,    306],
           [-33001,      1,     27,    312],
           [-34001,   4270,     27,    314],
           [-35001,   3880,     28,     13]], dtype=int32)


    In [78]: e2 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/cerenkov/csl-evt2.npy")

    In [79]: e2.shape
    Out[79]: (49, 6, 4)

    In [80]: e2.view(np.int32)[:,0]
    Out[80]: 
    array([[    -1,      1,      1,    284],
           [ -1001,      1,      1,    318],
           [ -2001,      1,      1,    302],
           [ -3001,    344,      1,    299],
           [ -4001,    659,      1,      6],
           [ -5001,    879,      1,    299],
           [ -6001,    379,      1,    319],
           [ -7001,    344,      1,    320],
           [ -8001,    344,      1,    282],
           [ -9001,   2196,      1,    282],
           [-10001,   2696,      1,    320],
           [-11001,   3160,      1,     50],
           [-12001,   3393,      1,    347],
           [-13001,   3881,      1,    307],
           [-14001,   4144,      1,     90],
           [-15001,   4384,      1,    309],
           [-16001,    344,      1,    317],
           [-17001,    344,      1,    282],
           [-18001,      1,      1,    173],
           [-19001,      1,      1,    302],
           [-20001,      1,      1,    295],
           [-21001,      1,      1,    324],
           [-22001,   5516,      1,    134],
           [-23001,      1,      1,     27],
           [-24001,      1,      1,    290],
           [-25001,      1,      1,    321],
           [-26001,      1,      1,    307],
           [-27001,      1,      1,    301],
           [-28001,      1,      1,    278],
           [-29001,      1,      1,    291],
           [-30001,      1,      1,    291],
           [-31001,   6390,      1,    279],
           [-32001,   6497,      1,    338],
           [-33001,   6585,      1,    299],
           [-34001,      1,      1,    290],
           [-35001,      1,      1,    289],
           [-36001,      1,      1,    288],
           [-37001,      1,      1,    277],
           [-38001,      1,      1,    276],
           [-39001,      1,     28,    237],
           [-40001,      1,     28,    300],
           [-41001,      1,     27,    320],
           [-42001,      1,     27,    266],
           [-43001,      1,     27,    270],
           [-44001,      1,     27,    285],
           [-45001,      1,     27,    305],
           [-46001,   8163,     27,    312],
           [-47001,   8002,     27,    304],
           [-48001,   7449,     28,    139]], dtype=int32)



Newer scintillation all with material 48::


    In [81]: s0 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/scintillation/ssl-evt0.npy")

    In [82]: s0.shape
    Out[82]: (89, 6, 4)

    In [83]: s0.view(np.int32)[:,0]
    Out[83]: 
    array([[    1,     1,    48,  1032],
           [ 1001,     1,    48,   569],
           [ 2001,     1,    48,   842],
           [ 3001,     1,    48,  1165],
           [ 4001,     1,    48,  1224],
           [ 5001,     1,    48,  1481],
           [ 6001,   290,    48,   362],
           [ 7001,     1,    48,  1105],
           [ 8001,   348,    48,   543],
           [ 9001,   383,    48,  1325],
           [10001,     1,    48,  1019],
           [11001,     1,    48,   840],
           [12001,   474,    48,   547],
    ...

    In [84]: s1 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/scintillation/ssl-evt1.npy")

    In [85]: s1.shape
    Out[85]: (73, 6, 4)

    In [86]: s1.view(np.int32)[:,0]
    Out[86]: 
    array([[    1,     1,    48,   927],
           [ 1001,     1,    48,  1289],
           [ 2001,    78,    48,   572],
           [ 3001,     1,    48,   964],
           [ 4001,   150,    48,   475],
           [ 5001,   185,    48,  1157],
           [ 6001,     1,    48,  1273],
           [ 7001,   273,    48,   543],
           [ 8001,   307,    48,   923],
           [ 9001,     1,    48,  1080],
           [10001,     1,    48,   996],
           [11001,   446,    48,    49],
           [12001,   404,    48,  1233],
           [13001,     1,    48,  1036],
           [14001,     1,    48,   912],
           [15001,     1,    48,   923],
           [16001,   770,    48,   586],
           [17001,   820,    48,  1294],
           [18001,   853,    48,   604],
           [19001,   875,    48,  1219],
           [20001,     1,    48,  1018],
           [21001,  1014,    48,   463],
           [22001,  1092,    48,  1191],
           [23001,  1050,    48,  1104],
           [24001,     1,    48,   880],

    In [87]: s2 = np.load("/usr/local/opticks/opticksdata/gensteps/juno1707/scintillation/ssl-evt2.npy")

    In [88]: s2.view(np.int32)[:,0]
    Out[88]: 
    array([[     1,      1,     48,   1070],
           [  1001,     47,     48,    192],
           [  2001,      1,     48,    864],
           [  3001,      1,     48,    864],
           [  4001,      1,     48,   1127],
           [  5001,    259,     48,   1067],
           [  6001,      1,     48,   1117],
           [  7001,      1,     48,    824],
           [  8001,    380,     48,   1399],
           [  9001,    503,     48,    575],
           [ 10001,    562,     48,   1221],
           [ 11001,    692,     48,     40],
           [ 12001,    816,     48,    347],
           [ 13001,    893,     48,    547],
           [ 14001,    966,     48,    555],
           [ 15001,   1064,     48,     46],
           [ 16001,   1157,     48,    241],
           [ 17001,   1295,     48,      2],
           [ 18001,   1394,     48,   1242],
           [ 19001,    379,     48,   1115],
           [ 20001,   1600,     48,    585],
           [ 21001,   1656,     48,      5],
           [ 22001,   1827,     48,      1],
           [ 23001,   1941,     48,   1418],



Issue : Dec 2016 : Lookup fails with live g4gun
-------------------------------------------------

* HUH did not do anything substantial but it seems not to be happening anymore


::

    delta:opticksgeo blyth$ opticks-find applyLookup
    ./ok/ok.bash:G4StepNPY::applyLookup does a to b mapping between lingo which is invoked 
    ./ok/ok.bash:     553         genstep.applyLookup(0, 2);   // translate materialIndex (1st quad, 3rd number) from chroma to GGeo 

    ./optickscore/OpticksEvent.cc:    m_g4step->applyLookup(0, 2);  // jj, kk [1st quad, third value] is materialIndex
    ./optickscore/OpticksEvent.cc:    idx->applyLookup<unsigned char>(phosel_values);
    ./optickscore/tests/IndexerTest.cc:    idx->applyLookup<unsigned char>(phosel->getValues());
    ./opticksnpy/tests/NLookupTest.cc:    cs.applyLookup(0, 2); // materialIndex  (1st quad, 3rd number)

    ./opticksnpy/G4StepNPY.cpp:bool G4StepNPY::applyLookup(unsigned int index)
    ./opticksnpy/G4StepNPY.cpp:        printf(" G4StepNPY::applyLookup  %3u -> %3d  a[%s] b[%s] \n", acode, bcode, aname.c_str(), bname.c_str() );
    ./opticksnpy/G4StepNPY.cpp:        printf("G4StepNPY::applyLookup failed to translate acode %u : %s \n", acode, aname.c_str() );
    ./opticksnpy/G4StepNPY.cpp:void G4StepNPY::applyLookup(unsigned int jj, unsigned int kk)
    ./opticksnpy/G4StepNPY.cpp:            bool ok = applyLookup(index);
    ./opticksnpy/G4StepNPY.cpp:       LOG(fatal) << "G4StepNPY::applyLookup"
    ./opticksnpy/G4StepNPY.cpp:       m_npy->save("$TMP/G4StepNPY_applyLookup_FAIL.npy");
    ./opticksnpy/G4StepNPY.cpp:       dumpLookupFails("G4StepNPY::applyLookup");
    ./opticksnpy/G4StepNPY.hpp:       void applyLookup(unsigned int jj, unsigned int kk);
    ./opticksnpy/G4StepNPY.hpp:       bool applyLookup(unsigned int index);



::

    075 void OpticksRun::setGensteps(NPY<float>* gensteps)
     76 {
     77     LOG(info) << "OpticksRun::setGensteps " << gensteps->getShapeString() ;
     78 
     79     assert(m_evt && m_g4evt && "must OpticksRun::createEvent prior to OpticksRun::setGensteps");
     80 
     81     m_g4evt->setGenstepData(gensteps);
     82 
     83     passBaton(); 
     84 }
     85 
     86 void OpticksRun::passBaton()
     87 {
     88     NPY<float>* nopstep = m_g4evt->getNopstepData() ;
     89     NPY<float>* genstep = m_g4evt->getGenstepData() ;
     90 
     91     LOG(info) << "OpticksRun::passBaton"
     92               << " nopstep " << nopstep
     93               << " genstep " << genstep
     94               ;
     95 
     96 
     97    //
     98    // Not-cloning as these buffers are not actually distinct 
     99    // between G4 and OK.
    100    //
    101    // Nopstep and Genstep should be treated as owned 
    102    // by the m_g4evt not the Opticks m_evt 
    103    // where the m_evt pointers are just weak reference guests 
    104    //
    105 
    106     m_evt->setNopstepData(nopstep);
    107     m_evt->setGenstepData(genstep);
    108 }


::

    0938 void OpticksEvent::setGenstepData(NPY<float>* genstep_data, bool progenitor, const char* oac_label)
     939 {
     940     int nitems = NPYBase::checkNumItems(genstep_data);
     941     if(nitems < 1)
     942     {
     943          LOG(warning) << "OpticksEvent::setGenstepData SKIP "
     944                       << " nitems " << nitems
     945                       ;
     946          return ;
     947     }
     948 
     949     importGenstepData(genstep_data, oac_label );
     950 
     951     setBufferControl(genstep_data);
     952 
     953     m_genstep_data = genstep_data  ;
     954     m_parameters->add<std::string>("genstepDigest",   m_genstep_data->getDigestString()  );
     955 
     956     //                                                j k l sz   type        norm   iatt  item_from_dim
     957     ViewNPY* vpos = new ViewNPY("vpos",m_genstep_data,1,0,0,4,ViewNPY::FLOAT,false,false, 1);    // (x0, t0)                     2nd GenStep quad 
     958     ViewNPY* vdir = new ViewNPY("vdir",m_genstep_data,2,0,0,4,ViewNPY::FLOAT,false,false, 1);    // (DeltaPosition, step_length) 3rd GenStep quad
     959 
     960     m_genstep_vpos = vpos ;
     961 
     962     m_genstep_attr = new MultiViewNPY("genstep_attr");
     963     m_genstep_attr->add(vpos);
     964     m_genstep_attr->add(vdir);
     965 
     966     {
     967         m_num_gensteps = m_genstep_data->getShape(0) ;
     968         unsigned int num_photons = m_genstep_data->getUSum(0,3);
     969         bool resize = progenitor ;
     970         setNumPhotons(num_photons, resize); // triggers a resize   <<<<<<<<<<<<< SPECIAL HANDLING OF GENSTEP <<<<<<<<<<<<<<
     971     }
     972 }




    1046 void OpticksEvent::importGenstepData(NPY<float>* gs, const char* oac_label)
    1047 {
    1048     Parameters* gsp = gs->getParameters();
    1049     m_parameters->append(gsp);
    1050 
    1051     gs->setBufferSpec(OpticksEvent::GenstepSpec(isCompute()));
    1052 
    1053     assert(m_g4step == NULL && "OpticksEvent::importGenstepData can only do this once ");
    1054     m_g4step = new G4StepNPY(gs);
    1055 
    1056     OpticksActionControl oac(gs->getActionControlPtr());
    1057     if(oac_label)
    1058     {
    1059         LOG(debug) << "OpticksEvent::importGenstepData adding oac_label " << oac_label ;
    1060         oac.add(oac_label);
    1061     }
    1062 
    1063 
    1064     LOG(debug) << "OpticksEvent::importGenstepData"
    1065                << brief()
    1066                << " shape " << gs->getShapeString()
    1067                << " " << oac.description("oac")
    1068                ;
    1069 
    1070     if(oac("GS_LEGACY"))
    1071     {
    1072         translateLegacyGensteps(gs);
    1073     }
    1074     else if(oac("GS_TORCH"))
    1075     {
    1076         LOG(debug) << " checklabel of torch steps  " << oac.description("oac") ;
    1077         m_g4step->checklabel(TORCH);
    1078     }
    1079     else if(oac("GS_FABRICATED"))
    1080     {
    1081         m_g4step->checklabel(FABRICATED);
    1082     }
    1083     else
    1084     {
    1085         LOG(debug) << " checklabel of non-legacy (collected direct) gensteps  " << oac.description("oac") ;
    1086         m_g4step->checklabel(CERENKOV, SCINTILLATION);
    1087     }
    1088 
    1089     m_g4step->countPhotons();
    .... 
    1105 }
    1106 



    0986 void OpticksEvent::translateLegacyGensteps(NPY<float>* gs)
     987 {
     988     OpticksActionControl oac(gs->getActionControlPtr());
     989     bool gs_torch = oac.isSet("GS_TORCH") ;
     990     bool gs_legacy = oac.isSet("GS_LEGACY") ;
     991 
     992     if(!gs_legacy) return ;
     993     assert(!gs_torch); // there are no legacy torch files ?
     994 
     995     if(gs->isGenstepTranslated())
     996     {
     997         LOG(warning) << "OpticksEvent::translateLegacyGensteps already translated " ;
     998         return ;
     999     }
    1000 
    1001     gs->setGenstepTranslated();
    1002 
    1003     NLookup* lookup = gs->getLookup();
    1004     if(!lookup)
    1005             LOG(fatal) << "OpticksEvent::translateLegacyGensteps"
    1006                        << " IMPORT OF LEGACY GENSTEPS REQUIRES gs->setLookup(NLookup*) "
    1007                        << " PRIOR TO OpticksEvent::setGenstepData(gs) "
    1008                        ;
    1009 
    1010     assert(lookup);
    1011 
    1012     m_g4step->relabel(CERENKOV, SCINTILLATION);
    1013 
    1014     // CERENKOV or SCINTILLATION codes are used depending on 
    1015     // the sign of the pre-label 
    1016     // this becomes the ghead.i.x used in cu/generate.cu
    1017     // which dictates what to generate
    1018 
    1019     lookup->close("OpticksEvent::translateLegacyGensteps GS_LEGACY");
    1020 
    1021     m_g4step->setLookup(lookup);
    1022     m_g4step->applyLookup(0, 2);  // jj, kk [1st quad, third value] is materialIndex
    1023     // replaces original material indices with material lines
    1024     // for easy access to properties using boundary_lookup GPU side
    1025 
    1026 }





Legacy Approach
----------------

Translate on load
~~~~~~~~~~~~~~~~~~~

Genstep material indices are translated into GPU material lines on loading the file,
to keep things simple GPU side.

`NPY<float>* OpticksHub::loadGenstepFile()`::

    389     G4StepNPY* g4step = new G4StepNPY(gs);
    390     g4step->relabel(CERENKOV, SCINTILLATION);
    391     // which code is used depends in the sign of the pre-label 
    392     // becomes the ghead.i.x used in cu/generate.cu
    393 
    394     if(m_opticks->isDayabay())
    395     {
    396         // within GGeo this depends on GBndLib
    397         NLookup* lookup = m_ggeo ? m_ggeo->getLookup() : NULL ;
    398         if(lookup)
    399         {
    400             g4step->setLookup(lookup);
    401             g4step->applyLookup(0, 2);  // jj, kk [1st quad, third value] is materialIndex
    402             //
    403             // replaces original material indices with material lines
    404             // for easy access to properties using boundary_lookup GPU side
    405             //
    406         }
    407         else
    408         {
    409             LOG(warning) << "OpticksHub::loadGenstepFile not applying lookup" ;
    410         }
    411     }
    412     return gs ;
         

* with in memory gensteps direct from G4, need to do the 
  same thing but with the lookup will need to be different


Lookups
~~~~~~~~~

* npy-/NLookup does the mapping

::

     /// setupLookup is invoked by GGeo::loadGeometry

     620 void GGeo::setupLookup()
     621 {
     622     //  maybe this belongs in GBndLib ?
     623     //
     624     m_lookup = new NLookup() ;
     625 
     626     const char* cmmd = m_opticks->getDetectorBase() ;
     627 
     628     m_lookup->loadA( cmmd, "ChromaMaterialMap.json", "/dd/Materials/") ;
     629 
     630     std::map<std::string, unsigned int>& msu  = m_lookup->getB() ;
     631 
     632     m_bndlib->fillMaterialLineMap( msu ) ;
     633 
     634     m_lookup->crossReference();
     635 
     636     //m_lookup->dump("GGeo::setupLookup");  
     637 }



ggeo-/tests/NLookupTest.cc::

    GBndLib* blib = GBndLib::load(m_opticks, true );

    NLookup* m_lookup = new NLookup();

    const char* cmmd = m_opticks->getDetectorBase() ;

    m_lookup->loadA( cmmd , "ChromaMaterialMap.json", "/dd/Materials/") ;

    std::map<std::string, unsigned int>& msu = m_lookup->getB() ;

    blib->fillMaterialLineMap( msu ) ;     // shortname eg "GdDopedLS" to material line mapping 

    m_lookup->crossReference();

    m_lookup->dump("ggeo-/NLookupTest");



ChromaMaterialMap.json contains name to code mappings used 
for a some very old gensteps that were produced by G4DAEChroma
and which are still in use.
As the assumption of all gensteps being produced the same
way and with the same material mappings will soon become 
incorrect, need a more flexible way.

Perhaps a sidecar file to the gensteps .npy should
contain material mappings, and if it doesnt exist then 
defaults are used ?

::

    simon:DayaBay blyth$ cat ChromaMaterialMap.json | tr "," "\n"
    {"/dd/Materials/OpaqueVacuum": 18
     "/dd/Materials/Pyrex": 21
     "/dd/Materials/PVC": 20
     "/dd/Materials/NitrogenGas": 16
     "/dd/Materials/Teflon": 24
     "/dd/Materials/ESR": 9
     "/dd/Materials/MineralOil": 14


Changes
---------

* move NLookup to live up in OpticksHub in order to 
  configure it from the hub prior to geometry loading 
  when the lookup cross referencing is done
 


