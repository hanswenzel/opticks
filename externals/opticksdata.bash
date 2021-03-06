##
## Copyright (c) 2019 Opticks Team. All Rights Reserved.
##
## This file is part of Opticks
## (see https://bitbucket.org/simoncblyth/opticks).
##
## Licensed under the Apache License, Version 2.0 (the "License"); 
## you may not use this file except in compliance with the License.  
## You may obtain a copy of the License at
##
##   http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software 
## distributed under the License is distributed on an "AS IS" BASIS, 
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
## See the License for the specific language governing permissions and 
## limitations under the License.
##

opticksdata-src(){      echo opticksdata.bash ; }
opticksdata-source(){   echo $BASH_SOURCE ; }
opticksdata-vi(){       vi $(opticksdata-source) ; }
opticksdata-usage(){ cat << EOU

Opticks Data
=============

From point of view of opticks regard this as another external ?


Locations
-----------

~/opticksdata 
       opticksdata bitbucket clone used to upload geometry and other opticks data

/usr/local/opticks/opticksdata
       readonly opticksdata bitbucket clone that only pulls, this is what users see


cleanup mess of untracked files
---------------------------------

See notes/issues/opticksdata-cleanup.rst

::

    [blyth@localhost juno1707]$ hg rm extras
    not removing extras: no tracked files
    [blyth@localhost juno1707]$ hg rm g4_00.a181a603769c1f98ad927e7367c7aa51.dae
    not removing g4_00.a181a603769c1f98ad927e7367c7aa51.dae: no tracked files
    [blyth@localhost juno1707]$ pwd
    /home/blyth/local/opticks/opticksdata/export/juno1707
    [blyth@localhost juno1707]$ 

    [blyth@localhost juno1707]$ rm -rf extras g4_00.a181a603769c1f98ad927e7367c7aa51.dae



big file issue
----------------

::

    epsilon:export blyth$ hg commit -m "juno1808" 
    epsilon:export blyth$ hg push 
    pushing to http://bitbucket.org/simoncblyth/opticksdata
    real URL is https://bitbucket.org/simoncblyth/opticksdata
    searching for changes
    http authorization required for https://bitbucket.org/simoncblyth/opticksdata                                                                                                                              
    realm: Bitbucket.org HTTP
    user: simoncblyth
    password: 
    abort: missing support for check:phases                                                                                                                                                                    
    epsilon:export blyth$ 

A few hours later from ~/opticksdata it works::

    epsilon:juno1808 blyth$ hg commit -m "j1808"
    epsilon:juno1808 blyth$ hg push 
    pushing to ssh://hg@bitbucket.org/simoncblyth/opticksdata
    Enter passphrase for key '/Users/blyth/.ssh/id_rsa': 
    searching for changes
    remote: adding changesets
    remote: adding manifests
    remote: adding file changes
    remote: added 1 changesets with 1 changes to 1 files
    epsilon:juno1808 blyth$ 
    epsilon:juno1808 blyth$ 


FUNCTIONS
-----------

::

    === opticksdata-export-ini : writing OPTICKS_DAEPATH_ environment to /usr/local/opticks/opticksdata/config/opticksdata.ini
    OPTICKSDATA_DAEPATH_DPIB=/usr/local/opticks/opticksdata/export/dpib/cfg4.dae
    OPTICKSDATA_DAEPATH_DYB=/usr/local/opticks/opticksdata/export/DayaBay_VGDX_20140414-1300/g4_00.dae
    OPTICKSDATA_DAEPATH_FAR=/usr/local/opticks/opticksdata/export/Far_VGDX_20140414-1256/g4_00.dae
    OPTICKSDATA_DAEPATH_JPMT=/usr/local/opticks/opticksdata/export/juno/test3.dae
    OPTICKSDATA_DAEPATH_LIN=/usr/local/opticks/opticksdata/export/Lingao_VGDX_20140414-1247/g4_00.dae
    OPTICKSDATA_DAEPATH_LXE=/usr/local/opticks/opticksdata/export/LXe/g4_00.dae


This .ini file is read by OpticksResource allowing opticks to access the .dae path 
at a higher level of just needing the tag.


opticks env
-------------

Most of the opticks environment comes from opticksdata-export::

    simon:boostrap blyth$ env | grep OPTICKS
    OPTICKS_HOME=/Users/blyth/opticks
    OPTICKS_INSTALL_PREFIX=/usr/local/opticks
    OPTICKS_ANA_DEFAULTS=det=concentric,src=torch,tag=1
    OPTICKS_DATA=/usr/local/opticks/opticksdata

    OPTICKSDATA_DAEPATH_DYB=/usr/local/opticks/opticksdata/export/DayaBay_VGDX_20140414-1300/g4_00.dae
    OPTICKSDATA_DAEPATH_DLIN=/usr/local/opticks/opticksdata/export/Lingao_VGDX_20140414-1247/g4_00.dae
    OPTICKSDATA_DAEPATH_DFAR=/usr/local/opticks/opticksdata/export/Far_VGDX_20140414-1256/g4_00.dae
    OPTICKSDATA_DAEPATH_JPMT=/usr/local/opticks/opticksdata/export/juno/test3.dae
    OPTICKSDATA_DAEPATH_LXE=/usr/local/opticks/opticksdata/export/LXe/g4_00.dae
    OPTICKSDATA_DAEPATH_DPIB=/usr/local/opticks/opticksdata/export/dpib/cfg4.dae


See Also
--------

* bitbucket- for notes on repo creation
* export- for the initial population

::

    simon:~ blyth$ export-;export-copy-
    mkdir -p /Users/blyth/opticksdata/export/DayaBay_VGDX_20140414-1300
    cp /usr/local/env/geant4/geometry/export/DayaBay_VGDX_20140414-1300/g4_00.dae /Users/blyth/opticksdata/export/DayaBay_VGDX_20140414-1300/g4_00.dae
    mkdir -p /Users/blyth/opticksdata/export/Far_VGDX_20140414-1256
    cp /usr/local/env/geant4/geometry/export/Far_VGDX_20140414-1256/g4_00.dae /Users/blyth/opticksdata/export/Far_VGDX_20140414-1256/g4_00.dae
    mkdir -p /Users/blyth/opticksdata/export/Lingao_VGDX_20140414-1247
    cp /usr/local/env/geant4/geometry/export/Lingao_VGDX_20140414-1247/g4_00.dae /Users/blyth/opticksdata/export/Lingao_VGDX_20140414-1247/g4_00.dae
    mkdir -p /Users/blyth/opticksdata/export/LXe
    cp /usr/local/env/geant4/geometry/export/LXe/g4_00.dae /Users/blyth/opticksdata/export/LXe/g4_00.dae
    simon:~ blyth$ 
    simon:~ blyth$ export-;export-copy- | sh 



EOU
}
opticksdata-env(){      olocal- ; opticks- ;  }
opticksdata-dir(){ echo $(opticks-prefix)/opticksdata ; }
opticksdata-c(){   cd $(opticksdata-dir)/$1 ; }
opticksdata-cd(){  cd $(opticksdata-dir)/$1 ; }

opticksdata-url(){     echo https://hwenzel@bitbucket.org/simoncblyth/opticksdata ; }
opticksdata-url-ssh(){ echo ssh://hg@bitbucket.org/simoncblyth/opticksdata ; }

opticksdata-j(){ echo $(opticksdata-xpath j1808).gdml ; }

opticksdata-jv2(){ echo $(opticksdata-xpath j1808)_v2.gdml ; }  ## manually remove guide tube torus
opticksdata-jv2-vi(){ vi $(opticksdata-jv2) ; }

opticksdata-jv3(){ echo $(opticksdata-xpath j1808)_v3.gdml ; }  
opticksdata-jv3-vi(){ vi $(opticksdata-jv3) ; }

opticksdata-jv4(){ echo $(opticksdata-xpath j1808)_v4.gdml ; }  
opticksdata-jv4-vi(){ vi $(opticksdata-jv4) ; }

opticksdata-jv5(){ echo $(opticksdata-xpath j1808)_v5.gdml ; }  
opticksdata-jv5-vi(){ vi $(opticksdata-jv5) ; }


 
opticksdata-d0-gdml(){   echo $(opticksdata-xpath dyb).gdml ; }
opticksdata-d0-dae(){    echo $(opticksdata-xpath dyb).dae ; }
opticksdata-d0-gdml-vi(){ vi $(opticksdata-d0-gdml) ; }
opticksdata-d0-dae-vi(){  vi $(opticksdata-d0-dae) ; }

opticksdata-dxtmp(){    echo $(opticks-dir)/tmp/CGeometry/CGeometry.gdml ; }
opticksdata-dxtmp-vi(){ vi $(opticksdata-dxtmp) ; }

opticksdata-dx(){  echo $(opticksdata-xpath dybexport).gdml ; }
opticksdata-dx-vi(){ vi $(opticksdata-dx) ; }


opticksdata-j-info(){ cat << EOI

   opticksdata-j   : $(opticksdata-j)
   opticksdata-jv2 : $(opticksdata-jv2)
   opticksdata-jv3 : $(opticksdata-jv3)     
   opticksdata-jv4 : $(opticksdata-jv4)     opticksdata-jv4-vi
   opticksdata-jv5 : $(opticksdata-jv5)     opticksdata-jv5-vi 

EOI
}



opticksdata-info(){ cat << EOM

   opticksdata-url       : $(opticksdata-url)
   opticksdata-url-ssh   : $(opticksdata-url-ssh)

EOM
}

opticksdata-hgrc-vi(){
   opticksdata-cd 

   opticksdata-info 

   echo $msg SWITCH TO SSH BY UPDATING URLS 

   vi .hg/hgrc

}



opticksdata-get(){
   local msg="$FUNCNAME :"
   local dir=$(dirname $(opticksdata-dir)) &&  mkdir -p $dir && cd $dir

   local url=$(opticksdata-url)
   local nam=$(basename $url)
   if [ ! -d "$nam/.git" ]; then
        local cmd="git clone $url "
        echo $msg proceeding with \"$cmd\" from $dir 
        eval $cmd
   else
        echo $msg ALREADY CLONED from $url to $(opticksdata-dir) 
   fi

}

opticksdata--()
{
   opticksdata-get
   opticksdata-export-ini
}

opticksdata-pull()
{
   local msg="$FUNCNAME :"
   local iwd=$PWD
   opticksdata-cd
   echo $msg PWD $PWD
   git  pull 
   git up
   cd $iwd  
}


opticksdata-path(){ echo $(opticksdata-xpath $1).dae ; }
opticksdata-xpath(){
  local base=$(opticksdata-dir)/export
  case $1 in 
       dyb) echo $base/DayaBay_VGDX_20140414-1300/g4_00 ;;
       dybexport) echo $base/DayaBay_VGDX_20140414-1300/g4_00_CGeometry_export ;;
      dlin) echo $base/Lingao_VGDX_20140414-1247/g4_00 ;;
      dfar) echo $base/Far_VGDX_20140414-1256/g4_00 ;;
      dpib) echo $base/dpib/cfg4 ;; 
       lxe) echo $base/LXe/g4_00 ;;
      jpmt) echo $base/juno/test3 ;;
      j1707) echo $base/juno1707/g4_00 ;; 
      j1808) echo $base/juno1808/g4_00 ;; 
  esac

#      dybf) echo $base/DayaBay_VGDX_20140414-1300/g4_00 ;;
#      jtst) echo $base/juno/test ;;
#      juno) echo $base/juno/nopmt ;;

}


opticksdata-tags-(){ cat << EOT
DYB
DFAR
DLIN
DPIB
JPMT
LXE
J1707
J1808
EOT
}

opticksdata-export(){
   local utag
   local ltag
   local daepath
   local gdmlpath
   for utag in $(opticksdata-tags-) 
   do
      ltag=$(echo $utag | tr "A-Z" "a-z")
      daepath=$(opticksdata-xpath $ltag).dae 
      gdmlpath=$(opticksdata-xpath $ltag).gdml 
      #printf "%5s %5s %s \n"  $utag $ltag $daepath

      if [ -f "$daepath" ]; then 
          export OPTICKSDATA_DAEPATH_$utag=$daepath
      elif [ -f "$gdmlpath" ]; then 
          export OPTICKSDATA_DAEPATH_$utag=$daepath    ## yes really daepath : gdmlpath can be derived from it 
      else
          [ -n "$VERBOSE" ] && echo $msg SKIP MISSING PATH for $utag 
      fi
   done
}

opticksdata-dump(){
   env | grep OPTICKSDATA_DAEPATH_ | sort 
}

opticksdata-find(){    find ~/opticksdata -name '*.dae' -type f ; }
opticksdata-find-ls(){ find ~/opticksdata -name '*.dae' -type f -exec ls -l {} \; ; }
opticksdata-find-du(){ find ~/opticksdata -name '*.dae' -type f -exec du -h {} \; ; }


opticksdata-ini(){ echo $(opticks-prefix)/opticksdata/config/opticksdata.ini ; }
opticksdata-export-ini()
{
   local msg="=== $FUNCNAME :"

   opticksdata-export 

   local ini=$(opticksdata-ini)
   local dir=$(dirname $ini)
   mkdir -p $dir 

   echo $msg writing OPTICKS_DAEPATH_ environment to $ini
   env | grep OPTICKSDATA_DAEPATH_ | sort > $ini

   cat $ini
}



opticksdata-export-ps(){  

   vs- 
   cat << EOP

# paste into powershell profile


\$env:DAE_NAME = "$(vs-wp $(opticksdata-name dyb))"
\$env:DAE_NAME_DYB = "$(vs-wp $(opticksdata-name dyb))"
\$env:DAE_NAME_DPIB = "$(vs-wp $(opticksdata-name dpib))"


EOP
}


opticksdata-export-du()
{
   opticksdata-cd export/$1

   du -sh * 

}



opticksdata-cherries-(){ cat << EOC
refractiveindex/tmp/glass/schott/F2.npy
refractiveindex/tmp/main/H2O/Hale.npy
gensteps/dayabay/cerenkov/1.npy
gensteps/dayabay/scintillation/1.npy
gensteps/juno/cerenkov/1.npy
gensteps/juno/scintillation/1.npy
export/juno1808/g4_00_v5.gdml
EOC
}

opticksdata-migrate-to-opticksaux()
{
   local src=$(opticksdata-dir)
   local dst=$HOME/opticksaux
   [ ! -d "$src" ] && return 
   [ ! -d "$dst" ] && return 

    
   local path
   local fold
   opticksdata-cherries- | while read path ; do

       fold=$(dirname $path)

       cd $src
       [ ! -f $path ] && echo $msg src path $path missing from $src && return 
       cd $dst 
       [ -f $path ] && echo $msg dst path $path already exists at $dst && continue 
 
       mkdir -p $fold
       echo $msg proceed to copy $path 
       cp $src/$path $fold/
   done

}






