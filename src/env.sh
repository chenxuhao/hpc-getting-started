# on Delta
module load papi
export PAPI_HOME=/opt/cray/pe/papi/7.0.1.2

# on Rockfish
#export PAPI_HOME=/scratch4/xhchen/papi/src/install

# On Anvil
#export PAPI_HOME=/anvil/projects/x-cis220095/papi/src/install

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PAPI_HOME/lib
