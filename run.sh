#!/bin/bash
for ((i=512;i<=2048;i+=512))
 do for((j=0;j<50;j++)) 
     do
       time openssl genrsa -out private.pem $i
     done
done

