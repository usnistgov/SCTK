../sclite -r ./csrnab.ref -h ./csrnab.hyp -i wsj -L ./csrnab_r.blm -o sum wws prf -O out -f 0 -n test1b
