#/bin/bash
#PATH=/bin:/sbin/:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
#export PATH

step=1000
for ((i=1; i<=25; i++))
do
    file=$(($i*$step)) 
	./ND ./run/neutron/"neutron_beam_$file"keV.in
done
