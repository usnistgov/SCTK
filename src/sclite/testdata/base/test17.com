../sclite -r ./test.vietnamese.ref.trn trn -h test.vietnamese.hyp.trn trn -i spu_id -o all prf -O out -f 0 -n test17 -e utf-8 babel_vietnamese
