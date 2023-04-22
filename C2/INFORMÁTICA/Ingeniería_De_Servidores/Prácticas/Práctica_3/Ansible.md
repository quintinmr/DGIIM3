# Ansible

Permite ejecutar comandos sobre máquinas remotas. Dado un equipo A, instalamos Ansible. Luego tendremos varios equipos diferentes (distintas máquinas virtuales con sistemas operativos los que sean). Desde la máquina A, ejecutaremos órdenes a las VM (crea varias hebras; hay concurrenca real). Ansible trabaja con comandos **Ad-Hoc** (solo trabajaremos con estas) y **PlayBooks** (no lo veremos, pero interesa; juega el papel de un scrip). Ansible es un lenguaje declarativo; decimos el estado final que queremos que alcance la máquina.

## Instalación

Recomendación: tener instalado Ansible en el ordenador anfitrión (para no tener lanzadas 3 máquinas virtuales al mismo tiempo, de cara al examen)

> sudo apt install ansible

Configuración en:

> /etc/ansible

Fichero de inventario:

> /etc/ansible/hosts

    Un concepto muy importante es [webservers]: lo tenemos que ver como un conjunto de servidores. De tal forma que si ejecutamos una orden sobre [webservers], se ejecutará dicha orden en cada uno de los servidores del conjunto.

Desde el ordenador anfitrión, editamos el archivo de inventario:

Por ejemplo:

>   correo1 ansible_hosts=192.168.56.10 ansible_user=usuario_que_sea

Desde la terminal:

> correo1 -m ping

Cuando mandamos un comando por ansible, lo que hacemos es mandar un archivo de python via ssh.

> Único requisito para que una máquina pueda utilizar Ansible, es suficiente que tenga **SSH** y **Python**

