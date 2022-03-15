../sclite -r ./lvc_ref.stm stm -h ./lvc_hypc.ctm ctm -o sum -O out -f 0 -n test3b
