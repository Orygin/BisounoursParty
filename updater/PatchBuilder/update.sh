#!/bin/bash

echo "Updating"

rm -rf bpr_old
rm -rf bpr_new
rm -rf output

cp bpr bpr_old -r

echo "Copied bpr to bpr_old"

cd bpr

#git reset --hard
expect -c "
spawn git pull origin master
expect -nocase \"Enter passphrase for key '/home/bisounoursparty/.ssh/id_rsa':\" {send \"*\r\"; interact}
"
cd ..

echo "Updated repo"

cp bpr bpr_new -r

echo "Copied bpr to bpr_new"

rm -rf bpr_new/.git
echo "removed git from bpr_new"
rm -rf bpr_old/.git
echo "removed git from bpr_old"

mkdir output
mkdir output/bpr

cd bpr_new

for D in $(find . -mindepth 1 -type d) ; do
   mkdir ../output/$D -p
done
cd ..

echo "Created output directories"

./PatchBuilder bpr_old bpr_new output

find output -type d -empty -delete

echo "deleted empty folders from output"

mkdir patch/bpr -p
mv output/* patch/bpr/

cd patch
../bin/7z a patch.7z bpr -y
mv patch.7z ../patch.7z
cd ..

echo "Built patch file"

rm -rf output
rm -rf patch
rm -rf bpr_new
rm -rf bpr_old

VERSION=$(head -1 bpr/bin/version)

cp patch.7z /var/www/bisounoursparty.com/updates/$VERSION.update
rm patch.7z

