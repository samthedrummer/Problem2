savedcmd_/home/ubuntu/problem2/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/ubuntu/problem2/"$$0) }' > /home/ubuntu/problem2/hello.mod
