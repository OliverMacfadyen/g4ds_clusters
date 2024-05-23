#!/bin/bash

sim_name="neutrons_Hybrid"
Njobs=400
beamOn=500000

#bgd=("bgd_titanium_vessel")
#bgd=( "bgd_sipm" "plasticscintillator" "bgd_veto_sipm" "bgd_titanium_vessel" "TPCbarrel")
bgd=("bgd_sipm" "bgd_veto_sipm" "bgd_gridSupport" "steel_structure" "anode" "cathode" "OPsAcrylic" "TPCbarrel" "bgd_titanium_vessel")
#bgd=("bgd_gridSupport" "steel_structure")

spectra=("U238StainlessSteel")

declare -i starting_seed=123777

###########################
build_dir=$(pwd)
rooter_dir="$build_dir/../tools"
rooter="$rooter_dir/g4rooter_nDS20k_update"
###########################

for ((n_loc=0; n_loc < ${#bgd[@]}; n_loc++)); do

	sim_path="${build_dir}/${sim_name}_${bgd[$n_loc]}"
	
	cd "${build_dir}"
	
	mkdir "${sim_name}_${bgd[$n_loc]}"
	cd "${sim_path}"
	
	cat << EOF > submitter.sub
universe = vanilla
request_memory = 2047 MB
arguments = \$(ProcId) \$(ClusterId)

executable = job.sh
error = err_\$(ProcId).sh
log = log_\$(ProcId).sh

ShouldTransferFiles = YES
WhenToTransferOutput = ON_EXIT

queue ${Njobs}
EOF

	chmod +x submitter.sub

	cat << EOF > job.sh
#!/bin/bash
PROCID=\${1}
CLUSID=\${2}
cd ${build_dir}
. /storage/gpfs_ds50/darkside/software/conda/envs/ds/etc/profile.d/conda.sh
conda activate base

./g4ds ${sim_path}/mac\${PROCID}.mac

if [ \${PROCID} -ge 1 ]; then
    rm "${sim_path}/mac\${PROCID}.mac"
fi

${rooter} ${sim_path}/outrun\${PROCID}.fil  ${sim_path}/outrun\${PROCID}.root NRcut=1

#if [   "\$(( \$(condor_q -format "%d\\n" ClusterId -constraint "ClusterId==\${CLUSID} && JobStatus==4" -name sn-02.cr.cnaf.infn.it | wc -l) - 0 ))" -eq "\$(condor_q -format "%d\\n" ClusterId -constraint "ClusterId==\${CLUSID}" -name sn-02.cr.cnaf.infn.it | wc -l)"   ]; then
    #root -l 'analyser.C("${sim_path}", "${sim_name}_${bgd[$n_loc]}.output")'
#fi

EOF

	chmod +x job.sh

	for ((n_mac=0; n_mac < ${Njobs}; n_mac++)); do	

		cat << EOF > mac${n_mac}.mac
/ds/manager/log routine
/ds/manager/verbosity 0
/ds/manager/checkoverlap 0
/ds/manager/eventcounter 10000
/ds/manager/writephotons 0
/ds/manager/writedeposits 1
/ds/manager/writedaughters 1

/ds/detector/configuration ds20k
/ds/detector/ds20k_SiPMsNumber 100
/ds/detector/vetoyieldfactor 0
/ds/detector/ds20k_SiPMs 1

/ds/detector/TPCconfiguration Hybrid

/run/filename ${sim_path}/outrun${n_mac}

/run/heprandomseed $((${starting_seed} + ${n_mac}))

/ds/physics/hadronic_list QGSP_BIC_HP
/ds/physics/em_list livermore

/ds/physics/killS1S2 1
/ds/physics/tuned200V 1


/run/initialize


# Define the number of events

/ds/generator/select G4Gun
/ds/generator/particle neutron
/ds/generator/${bgd[$n_loc]} 1

/ds/generator/energyfile ../data/physics/${spectra[0]}.dat

/run/beamOn ${beamOn}

EOF
	done

	condor_submit -spool -name sn-02.cr.cnaf.infn.it submitter.sub 
	
done 
