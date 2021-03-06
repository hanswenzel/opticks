tcfg4 bash level option
==========================

op.sh level executable switches, okg4/tcfg4/* -> OKG4Test/CG4Test/OKTest 
-----------------------------------------------------------------------------

The tcfg4 bash level option was formerly used to 
create Opticks format "G4" events, using 
a bash level convention of negating the tag.


::

    169 op-binary-name-default(){ echo OKTest ; }
    170 
    171 op-binary-names(){ type op-binary-name | perl -ne 'm,--(\w*)\), && print "$1\n" ' - ; }
    172 op-binary-name()
    173 {
    174    case $1 in
    175          --idpath) echo OpticksIDPATH ;;
    176            --keys) echo InteractorKeys ;;
    177           --tcfg4) echo CG4Test ;;
    178            --okg4) echo OKG4Test ;;
    179          --tracer) echo OTracerTest ;;
    180       --gdml2gltf) echo gdml2gltf.py ;;


Current approach for comparisons is bi-simulation with okg4
--------------------------------------------------------------

This was replaced on moving to bi-simulation 
whereby Opticks and G4 events are standardly 
both created at once. 

::

     34 void OpticksRun::createEvent(unsigned tagoffset)
     35 {
     36     m_ok->setTagOffset(tagoffset);
     37     // tagoffset is recorded with Opticks::setTagOffset within the makeEvent, but need it here before that 
     38 
     39     OK_PROFILE("OpticksRun::createEvent.BEG");
     40 
     41     m_g4evt = m_ok->makeEvent(false, tagoffset) ;
     42     m_evt = m_ok->makeEvent(true, tagoffset) ;
     43 
     44     LOG(trace) << m_g4evt->brief() << " " << m_g4evt->getShapeString() ;
     45     LOG(trace) << m_evt->brief() << " " << m_evt->getShapeString() ;
     46 

     76 OpticksEvent* OpticksRun::getG4Event()
     77 {
     78     return m_g4evt ;
     79 }
     80 OpticksEvent* OpticksRun::getEvent()
     81 {
     82     return m_evt ;
     83 }
     84 OpticksEvent* OpticksRun::getCurrentEvent()
     85 {
     86     bool g4 = m_ok->hasOpt("vizg4|evtg4") ;
     87     return g4 ? m_g4evt : m_evt ;
     88 }
     89 



Former way of producing G4 and Opticks events for comparison 
--------------------------------------------------------------

The two launches are now replaced with a single --okg4 one.

::

    238 #tpmt-gen()
    239 #{
    240 #    tpmt--  --compute 
    241 #    tpmt--  --tcfg4
    242 #}


Convention of tag negation with tcfg4 was required.  This
same convention is now automatic.

::

    184 tpmt--(){
    185    type $FUNCNAME
    186 
    187     local msg="=== $FUNCNAME :"
    188 
    189     local cmdline=$*
    190     local tag=$(tpmt-tag)
    191 
    192     [ -z "$OPTICKS_INSTALL_PREFIX" ] && echo missing envvar OPTICKS_INSTALL_PREFIX && return
    193 
    194     #if [ "${cmdline/--tcfg4}" != "${cmdline}" ]; then
    195     #    tag=-$tag  
    196     #fi 
    197     ## hmm suspect tag negation no longer needed, as doing both at once ???
    198 
    199     local anakey
    200     if [ "${cmdline/--okg4}" != "${cmdline}" ]; then
    201         anakey=tpmt   ## compare OK and G4 evt histories
    202     else
    203         anakey=tevt    ## just dump OK history table
    204     fi


Ancient tcfg4 should be replaced with okg4 ?
------------------------------------------------

* YES: almost all use of tcfg4 is now a bug 


::

    simon:tests blyth$ grep tcfg4 *.*
    tbox.bash:`tbox-- --tcfg4` 
    tbox.bash:`tbox-- --tcfg4 --load`
    tg4gun.bash:#       --tcfg4 \
    tnewton.bash:Running with --tcfg4 option to use geant4 simulation runs into 
    tpmt.bash:`tpmt-- --tcfg4` 
    tpmt.bash:`tpmt-- --tcfg4 --load`
    tpmt.bash:    #if [ "${cmdline/--tcfg4}" != "${cmdline}" ]; then
    tpmt.bash:#    tpmt--  --tcfg4
    tpmt.bash:tpmt-v-g4() { tpmt-- --load --tcfg4 ; } 
    trainbow.bash:    if [ "${cmdline/--tcfg4}" != "${cmdline}" ]; then
    trainbow.bash:   trainbow-- --${pol}pol --tcfg4
    trainbow.bash:trainbow-v-g4(){  trainbow-- $* --load --tcfg4 ; } 
    ttemplate.bash:`ttemplate-- --tcfg4` 
    ttemplate.bash:`ttemplate-- --tcfg4 --load`
    ttemplate.bash:    if [ "${cmdline/--tcfg4}" != "${cmdline}" ]; then
    ttemplate.bash:    ttemplate--  --tcfg4
    ttemplate.bash:ttemplate-v-g4() { ttemplate-- --load --tcfg4 ; } 
    simon:tests blyth$ 


