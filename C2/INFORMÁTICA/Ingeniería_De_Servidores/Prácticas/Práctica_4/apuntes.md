# PRÁCTICA 4

## Ejercicio 

1. Añadimos llave GPG para validar el repositorio:
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

2. Añadimos repositorio:
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

3. Actualizamos lista de repositorios:
sudo apt update

4. Buscamos el repositorio de docker (community edition) y lo instalamos:
apt search docker-ce
sudo apt install docker-ce

5. Comprobar estado del servicio (ver que está iniciado y activado...)

     systemctl status docker

    sudo systemctl start docker

    Añadimos el usuario al grupo docker 2
    sudo usermod -aG docker su_nombre_de_usu

6. Podemos probar los comandos:

    docker info ; docker run hello-world

7. Instalar docker compose:

    sudo apt install docker-compose

8. Levantar el servicio iseJMeter:

    git clone https://github.com/davidPalomar-ugr/iseP4JMeter.git

9. Levantamos el servicio docker-compose:

    cd iseP4JMeter

    docker-compose up

10. Tras terminar el despliegue podemos ya crear el test de comprobación.

11. Accedemos al servicio con el navegador (todo esto sin cortar el docker compose up):

    http://192.168.56.15:3000/

12. 