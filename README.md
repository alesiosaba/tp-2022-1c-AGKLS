# TP SO 2022 1C - Grupo: AGKLS

## Links de descarga de vm's

https://www.utnso.com.ar/recursos/maquinas-virtuales/

## VM ubuntu-server

# Configurar network adapter en la vm

- Revisar si corresponde tildar la opcion "Cable Connected"

![image](https://user-images.githubusercontent.com/62452207/181865772-c5fca0cc-d5cf-45e2-a120-b103bf49cdf7.png)

# Una vez iniciada la vm comprobar la ip local asignada a ubuntu-server:

**Usar el comando** `ifconfig`

![image](https://user-images.githubusercontent.com/62452207/181865973-ed7442f3-d4de-4601-9e8f-297a115b7e61.png)

**Si no se configura una ip local automaticamente usar:**

`sudo systemctl restart networking`
