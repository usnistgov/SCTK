../sclite -r ./stm2ctm_missing.ref.stm stm -h ./stm2ctm_missing.hyp-missfile1chanA.ctm ctm -o all prf -O out -f 0 -n test16_4
