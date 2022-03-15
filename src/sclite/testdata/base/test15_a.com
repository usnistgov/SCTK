../sclite -r ./test.cantonese.stm stm -h ./test.cantonese.ctm ctm -o all prf -O out -f 0 -n test15_a -e utf-8
