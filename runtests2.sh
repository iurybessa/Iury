#!/bin/bash
# runtests.sh
# Script to automate running filter verification

# TEST TYPES: overflow, limitcycle, timing, error, magnitude, phase, stability.
testtypes=( overflow limitcycle timing error magnitude phase stability )
for testtype in ${testtypes[@]} 
do
echo "============== Running filter verification - $testtype - Cases 12 to 20 =============="

#testtype="error";
#if [ $# -eq 0 ]; then
#	echo "No arguments supplied. Using default test: $testtype"
#	echo "You can set a verification type, eg.: ./runtests.sh --limitcycle"
#	echo -e "  --overflow \n  --limitcycle \n  --timing \n  --error \n  --magnitude \n  --stability"
#
#elif [ $# -ne 0 ] && [ $1 != "--overflow" ] && [ $1 != "--limitcycle" ] && [ $1 != "--timing" ] && \
#     [ $1 != "--error" ] && [ $1 != "--magnitude" ] && [ $1 != "--stability" ]; then
#	echo -e "Test $1 not supported! Use some of the options below in \n./runtests.sh [option]"
#	echo -e "  --overflow \n  --limitcycle \n  --timing \n  --error \n  --magnitude \n  --stability"
#	exit
#else
#	#Line below set testtype and removes the "--"
#	testtype=${1:2};
#fi

mkdir logs_"$testtype"
	
tcmin=12
tcmax=20

# Loop on test cases
for i in `seq $tcmin $tcmax`
do
		if [ "$testtype" == "overflow" ]; then
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_overflow.c \
	    		-DTESTCASE=$i \
	    		--no-bounds-check --no-pointer-check --no-div-by-zero-check --z3-bv \
	    		--timeout 1h \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
			} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt
	    	
		elif [ "$testtype" == "limitcycle" ]; then
			echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_limitcycle.c \
	    		-DTESTCASE=$i \
	    		--no-bounds-check --no-pointer-check --no-div-by-zero-check --z3-bv \
	    		--timeout 1h \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ; 
			} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt
		
		elif [ "$testtype" == "timing" ]; then
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_timing.c \
	    		-DTESTCASE=$i \
	    		--no-bounds-check --no-pointer-check --no-div-by-zero-check --z3-bv \
	    		--timeout 1h \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
	    		} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt

		elif [ "$testtype" == "error" ]; then
			#Error test is not supported for complex filters cases now! 
			if [ $i -gt 15 ]; then	continue; fi
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_error.c \
	    		-DTESTCASE=$i \
	    		--no-bounds-check --no-pointer-check --no-div-by-zero-check --z3-bv \
	    		--timeout 1h \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
			} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt

		elif [ "$testtype" == "magnitude" ]; then
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_freqz.c \
	    		-DTESTCASE=$i \
	    		--timeout 1h \
	    		--fixedbv \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
	    		} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt

		elif [ "$testtype" == "phase" ]; then
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc verify_freqz.c --function phase\
	    		-DTESTCASE=$i \
	    		--timeout 1h \
	    		--fixedbv \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
	    		} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt

		elif [ "$testtype" == "stability" ]; then
			#Stability test is not supported for complex filters cases now! 
			if [ $i -gt 15 ]; then	continue; fi
	    		echo "=============== VERIFY: $testtype - CASE $i ==============="
	    		{ time ./esbmc_stability verify_stability.c \
	    		-DTESTCASE=$i \
	    		--timeout 1h \
	    		--fixedbv \
	    		> logs_"$testtype"/log_"$testtype"_c"$i".txt ;
			} 2>> logs_"$testtype"/log_"$testtype"_c"$i".txt

		else
			echo "FAIL! Test $testtype not supported."
		fi
done
done

exitcode=$?
echo "Exit code = $exitcode"
exit $exitcode
