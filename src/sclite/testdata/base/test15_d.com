../sclite -r ./test.ukranian.ref trn -h ./test.ukranian.hyp -o all prf -O out -f 0 -n test15_d -e utf-8 ukrainian -i spu_id
