../sclite -r ./tests.ref.utf8-2bytes -h ./tests.hyp.utf8-2bytes -i spu_id -o all sgml -O out -f 0 -n test7-utf8-2bytes -F -D -e utf-8
