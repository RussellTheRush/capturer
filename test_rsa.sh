#! /bin/bash

ctags -R .
gcc -g rsa.c -o rsa
a="25276A6CA9917361FeEA6AAAA28492625B1E2E0FDE1705FECF496652F4D776E45F21356CDFAE8B9DBA6925D0762CBE3842B2B44CF814EA413BEEACCA60E9DE81"
b="962868625c532F6A07852BE7744E20B1880737AA4FACB5B44E6EB91749CB64D99879724308C928449633238D5F245E10AC01A6D57D5513D7EC67EC726B41C3E7"
N="CF4966f2F4D776E45F21356CDFAE8B9DBA6"
M="AF4966524D776325F21356CDFAE8B9DBA7"
./rsa $a $b $N
#./rsa $N $M

#a="1a9bb6cf82bd5e06b86c24501e09a6ca7"
#b="1a9bb6cf82bd5e06b86c24501e09a6ca7"
#N="CF4966f2F4D776E45F21356CDFAE8B9DBA6"
#./rsa $a $b $N
