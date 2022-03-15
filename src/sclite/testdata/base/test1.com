../sclite -r ./csrnab.ref -h ./csrnab.hyp -i wsj -o all snt spk dtl prf sgml nl.sgml -O out -f 0 -n test1
