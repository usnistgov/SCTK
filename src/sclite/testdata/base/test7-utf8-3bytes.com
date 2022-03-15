../sclite -r ./tests.ref.utf8-3bytes -h ./tests.hyp.utf8-3bytes -i spu_id -o all sgml -O out -f 0 -n test7-utf8-3bytes -F -D -e utf-8
