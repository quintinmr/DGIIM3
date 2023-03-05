# PRÁCTICA 1. LECCIÓN 1.2

## Configuración de red

Cuando se configura la red se crea una tarjeta de red  tipo NAT (ventaja: drectamente a través del ordenador anfitrión podemos salir a internet porque virtual box implementa un servidor de tipo NAT). Vamos a crear una tarjeta de tipo "host-only" que solamente ermite conectarte con el host (viirtual box crea una de tipo host only que permite comunicarte desde el host hasta la máquina). Todas las máquinas que creemos han de tener las dos tarjetas de red, NAT y Host-Only. Otra ventaja que tine HO es que si hay varias máquinas virtuales conectadas, permite la comunicación simultánea entre dichas máquinas.

> Documentación tipos de tarjetas en "Ayuda > Virtual Networking > ...

1. Nos metemos en Configuración > Red > Adaptador 2 y lo ponemos en Solo Anfitrión.

2. Iniciamos la máquina.

3. Una vez dentro ejecutamos ip a

4. Nos metemos en el fichero /etc/neplan/00....yaml
   y lo editamo así:

   <image src="images/equi.png">

5. Hacemos ping a 192.168.56.10 desde VM

6. Hacemos ping a 192.168.56.10 desde nuestra terminal

7. Configuración del prompt de nuestro equipo; configuramos la variable $PS1. Para ello, vamos a irnos al fichero 
   

# PRÁCTICA 1. LECCIÓN 2

# Instalación de Rocky

1. Hacemos los mismos pasos que en Ubuntu para crear  la máquina pero sin crear el disco nuevo.

2. Inciar máquina Press tab para obtener toda la configuración.

3. Añadir la opción inst.text.

4. Modo texto.

5. Español, España, confirmar particionado automático, usar todo el espacio, usar LVM, crear user, meter en administrator, groups wheel., la hora (madrid 28).

6. Instalación ...

7. Hacer todo ahora con root (sudo bash).

8. Hacemos lsblk y vemos el árbol.
    <image src="images/1.png">

9. Hacemos pvdisplay para mostrar información sobre los volúmenes físicos del equipo.
    <image src="images/2.png">
10. Haciendo vgdisplay nos muestra información del volume group.
A lo que va con estos comandos es que si nos fijamos en el campo Free, vemos que está a 0 y por lo tanto lo que tendré que hacer :

    10.1 Añadirle un disco, añadirle más espacio. Para ello, apagamos la máquina y añadimos un nuevo disco (almacenamiento SATA). Encendemos de nuevo la máquina.

    <image src="images/3.png">

    10.2 Crear phisical volume y Añadir PV en VG: vamos a marcar el disco como phisical volume. Para ello, desde la terminal hacemos:

        pvcreate /dev/sdb

        pvdisplay

        vgdisplay (vemos que free está a 0)

        vgextend rl /dev/sdb

        vgdisplay y veremos un aumento de Gigas en Free

    <image src="images/4.png">

    10.4 Crear LV (nvar): como es un engorro, vamos a instalar el teclado en español (yum install kbd y luego localect set-keymap es)

        lvcreate -L 10G -n nvar rl

        vgdisplay (vemos que nos hemos comido 10G)

        lvdisplay (vemos que esos 10 G se han ido al LV que hemos creado)

    <image src="images/5.png">
    <image src="images/6.png">

11. Ahora lo que vamos a hacer es montar el /var en /mnt copiarl oy luego demonstarlo. Para darle formato a cualquier dispositivo:

        11.1 mkfs -t ext4 /dev/rl/nvar    (crea la tabla de inodos).

    <image src="images/7.png">

        11.2 Ahora ya la modemos montar: 
        mount /dev/rl/nvar /mnt/nvar
        Si ejecutamos mount a secas, veremos todo lo que está montado en ell sistema.

    <image src="images/8.png">

        11.3 cd /mnt/nvar

        11.4    ls -la

        11.5    cd /

        11.6 cd var/

        11.7 Para asegurar que no nos dejamos nada atras, pasamos el directorio a modo mantenimiento, hacemos la copia y luego recuperamos:

        systemctl isolate runlevel1.target
        systemctl status (y vemos que está en "maintance")

    <image src="images/9.png">


        cp -ar /var/* /mnt/nvar
        (r recursivamente)
        cd /mnt/nvar y comprobamos si se ha copiado
        cd ..
        cd ..
        mv var var_old
        mkdir var
        umount /mnt/nvar
        mount /dev/rl/nvar /var

    <image src="images/10.png">


    11.8 Si queremos que al apagar la máquina se guarde todo lo que hems montado manualmente, tenemos que editar el fichero fstab:

        nano /etc/fstab
        unmount /var
    Nos metemos en el fichero y escribimos en la última linea:

        /dev/rl/nvar  /var  ext4  defaults 0 0 
        :qw

12. Probar que todo está bien montado:
    dentro del /etc/:

        mount -a
        mount para ver lo montado
        lsblk y vemos si está ya fijo

    <image src="images/11.png">


13. Reiniciar

14.     df -h


15. dd (permite hace copia a nivel raw):

        sudo dd if=/dev/zero of=/var/basura.bin bs=1024M count=4096

<image src="images/12.png">


16.     df -h (vemos si hemos petado /var (avail = 0))


17. lvresize -L +5G /dev/rl/nvar

18. lvdisplay (15 G)
   si hago df -h ¡hay todavía 10G (hay inodos solo para 10 G)!

   <image src="images/13.png">

   Como lo arreglo:
   resize2fs /dev/rl/nvar
   df -h (ver si se ha recuperado)

<image src="images/14.png">

##  Tarjet de red rocky

1. Adaptador 2 solo anfitrión

2. Arrancar

3. El sistema de gestión de fedora, etc. se llama NetworkManager, gráfico y para configurar todo esto necesitamos un programa que se llama.

4. nmcli c show (conexiones activas)
   nmcli c delete "Wired Connection 1"
   nmcli c show para ver si la ha borrado
   nmcli c add type ethernet coon-name enp0s8 ifname enp0s8 ip4 192.168.56.20/24
   nmcli c show

   <image src="images/15.png">

   ip -a para ver si ha cogido la ip que le hemos dicho

   <image src="images/16.png">

5. ping 192.168.56.20 (desde VM y nuestro host)

    <image src="images/17.png">

