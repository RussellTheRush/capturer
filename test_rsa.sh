#! /bin/bash

a="25276A6CA9E17361F0EA6AAAA28492625B1E2E0FDE1705FECF496652F4D776E45F21356CDFAE8B9DBA6925D0762CBE3842B2B44CF814EA413BEEACCA60E9DE81"
b="9628686253532F6A07852BE7744E20B1880737AA4FACB5B44E6EB91749CB64D99879724308C928449633238D5F245E10AC01A6D57D5513D7EC67EC726B41C3E7"
N="CF496652F4D776E45F21356CDFAE8B9DBA6"
M="AF4966524D776325F21356CDFAE8B9DBA7"
ctags -R .
gcc -g rsa.c -o rsa
./rsa $a $b $N
#./rsa $N $M
