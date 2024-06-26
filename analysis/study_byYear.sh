#usage example sh study_byYear.sh dnc 2018

TAG=$1
N_WORKERS=64

if [[ "$TAG" == "" ]]; then
    echo "No tag provided"
    exit 1
fi

BASEDIR=/data/userdata/$USER/vbs_studies
YEAR=$2

./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=$TAG --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_nofix --year=$YEAR
# Copy nominal background to JER variations (wasteful, I know, but allows for the merges below to be done with the same script)
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jer_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jer_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jmr_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jmr_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jms_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jms_dn --year=$YEAR
# Copy nominal background to JEC variations (wasteful, I know, but allows for the merges below to be done with the same script)
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_1_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_1_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_2_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_2_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_3_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_3_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_4_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_4_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_5_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_5_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_6_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_6_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_7_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_7_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_8_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_8_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_9_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_9_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_10_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_10_dn --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_11_up --year=$YEAR
./bin/merge_vbsvvhjets_yearly vbsvvhjets --basedir=$BASEDIR --tag=${TAG}_jec_11_dn --year=$YEAR


JECS="
jec_2
jec_3
jec_5
jec_8
jec_11
"
VARS="
up
dn
"
YEARS="
2016postVFP
2016preVFP
2017
2018
"

#create symlink for the by year jecs so that you can later run the datacard script as usual
for JEC in $JECS; do
    for Y in $YEARS; do
        if [ ${YEAR} == ${Y} ]; then
             ln -s /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_jec_2_dn/Run2_${YEAR} /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_${JEC}_${Y}_dn/Run2_${YEAR}
             ln -s /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_jec_2_up/Run2_${YEAR} /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_${JEC}_${Y}_up/Run2_${YEAR}
        else
             ln -s /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc/Run2_${YEAR} /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_${JEC}_${Y}_dn/Run2_${YEAR}
             ln -s /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc/Run2_${YEAR} /data/userdata/legianni/vbs_studies/vbsvvhjets/output_dnc_${JEC}_${Y}_up/Run2_${YEAR}
        fi
    done
done

