../sclite -r ./test.turkish.ref trn -h ./test.turkish.hyp -o all prf -O out -f 0 -n test15_c -e utf-8 babel_turkish -i spu_id
