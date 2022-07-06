../sclite -r ./lvc_ref.stm stm -h ./lvc_hyp.ctm ctm -o all lur prf -O out -f 0 -n test3
