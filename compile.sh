#! /bin/sh
clear
echo -e "ruta actual :"$(pwd)
echo -e "[!] Comenzando compilacion: \n"

make
make finally

echo -e "creando arbol de directorios"
mkdir -p iso
mkdir -p ./iso/boot
mkdir -p ./iso/boot/grub

echo -e "\nasignando sitio a los archivos del OS"
cp ./kernel ./iso/boot
cp ./grub.cfg ./iso/boot/grub

echo -e "\ncreando iso: "
grub-mkrescue --compress=xz --product-name=Desmon -o desmonOS.iso iso #--product-version=0.1.0
echo -e "\n[*] Os compilado, en la carpeta "$(pwd)
rm -rf *.o;
ls -a
echo -e "Empezando simulacion:\n"
qemu-system-x86_64 -cdrom desmonOS.iso

#tree
