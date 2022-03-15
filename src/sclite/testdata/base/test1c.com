../sclite -r ./csrnab.ref -h ./csrnab.hyp -i wsj -w ./csrnab_r.wwl -o wws prf -O out -f 0 -n test1c
