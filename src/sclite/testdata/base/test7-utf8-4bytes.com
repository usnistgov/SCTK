../sclite -r ./tests.ref.utf8-4bytes -h ./tests.hyp.utf8-4bytes -i spu_id -o all sgml -O out -f 0 -n test7-utf8-4bytes -F -D -e utf-8
