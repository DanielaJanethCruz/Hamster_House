# Hamster_House- GDS0452
## Integrantes:
  - Cruz Breña Daniela Janeth (1221100295) <br>
  - Garay García Omar Ricardo (1221100307)<br>
  - Palacios Rangel Maria Fernanda (1221100171)<br>
  - Rincon Torres Juan Antornio (1221100684)
## Objetivos:
### Objetivo Generales:
El objetivo de este proyecto es agrantizar un estilo de vida apto, de las ascotas de muestros clientes. Evitando que los clientes tengan perdidas, brindando atencion a los aspectos vitales de un hámster como su alimentacion, temperatira de su entorno, manteniendo un registro de las actividades del Hamster como es su alimentación.
### Objetivos Especificos:
- Mantener un ambiente de calidad
- Mantener un control de alimentacion del hamster
- Asegurar una temperatura apta para el desarrollo del hamster.
## Requerimientos:
Los requerimiwentos fueron planteados por el docente Rodriguez García Anastacio para la Unidad 3: Desarrollo de Interfaces Gráficas, para la acreditación de la materia de Aplicaciones de IoT, los cuales fueron:
- Firma de profesor o empresario
- Repositorio con código y Documentación
- Uso de Rasperry Pi como Broker y comunicación MQTT
- Uso de 4 o 5 sensores
- Uso de 4 o 5 actuadores
- Uso de Dashboard para monitoreo y control
- Uso de Base de Datos relacional o NoSQL
- Uso de ESP32
## Materiales:
### Tabla de Software utilizado
| Id | Software | Version | Tipo |
|----|----------|---------|------|
|  1  |    Arduino      |    2.0.3     |   Gratuito   |
|  2  |    JavaScript      |    8u361     |  Gratuito    |
|  2  |       C   |    c18     |  Gratuito    |

### Tabla con el hardware utilizado
| Id | Componente | Descripción | Imagen | Cantidad | Costo total |
|----|------------|-------------|--------|----------|-------------|
|  1  |     ESP32       |      Es un microprocesador utilizado para el intercambio de informacion via wifi y bluetooth       | ![image](https://user-images.githubusercontent.com/115411941/233493935-640a50f0-6426-4ed0-b638-8917e14e34cb.png)|    1      |       $160      |
|  2  |    Rasperry pi        | Ordenador compacto de bajo coste que se utiliza para aprender sobre programación y electrónica, y para crear una amplia variedad de proyectos informáticos.            |    ![image](https://user-images.githubusercontent.com/115411941/233494213-0e5129a3-c47f-494d-b66c-ed926f1162bd.png)|    1      |       $4035      |
|  3  |      KY-026      |Módulo de sensor de detección de llama de infrarrojos. Puede detectar llamas o fuentes de calor y enviar una señal de salida digital al microcontrolador.|   ![image](https://user-images.githubusercontent.com/115411941/233494957-ffc53956-8c69-4dd1-bfa6-7d56f83bbb3c.png)|1| $75|
|  4  |      DHT11      |Sensor de temperatura y humedad relativa que proporciona lecturas precisas y confiables. Este sensor es utilizado comúnmente en proyectos de automatización del hogar, monitoreo ambiental y otros sistemas electrónicos que requieren medición de temperatura y humedad. |![image](https://user-images.githubusercontent.com/115411941/233495039-6c90b390-1362-49ac-9812-f1a1020702f1.png)|     1     |      $40       |
|  5  |      DHT11      |Sensor de temperatura y humedad relativa que proporciona lecturas precisas y confiables. Este sensor es utilizado comúnmente en proyectos de automatización del hogar, monitoreo ambiental y otros sistemas electrónicos que requieren medición de temperatura y humedad. |![image](https://user-images.githubusercontent.com/115411941/233495090-bf4aec89-52be-4216-94b2-ca61fab07a93.png)|     1     |      $40       |
|  6  |      Analog Temp      |Dispositivo que mide la temperatura en un rango de valores continuo, en lugar de señales discretas como los sensores digitales.|![image](https://user-images.githubusercontent.com/115411941/233495371-0006c5cc-0a06-450e-90d6-0c0325fff9a0.png)|     1     |       $25      |
|  7  |      Relay      |Es un dispositivo que se utiliza para conectar sensores a otros dispositivos, la función principal del sensor relay es convertir la señal eléctrica del sensor en una señal que pueda ser utilizada por otro dispositivo|![image](https://user-images.githubusercontent.com/115411941/233495184-cf149c5d-5b24-4ca4-9529-93b617a5ba77.png)|    1      |      $75       |
|  8  |      Servomotor      |Dispositivo electromecánico que convierte una señal de control en un movimiento preciso y controlado de un eje.|![image](https://user-images.githubusercontent.com/115411941/233495253-ba6961b1-a414-48ef-a575-1524250888ca.png)|3|$100|
|  9  |      Buzzer     |Dispositivo que emite un sonido agudo y fuerte para llamar la atención o para indicar algo.|![image](https://user-images.githubusercontent.com/115411941/233495316-df14d17a-e2a5-4249-addf-6276ffbee478.png)|2|$30|
#### Sensores:
- Sensor Ky-026
- Sensor DHT11
- Sensor Analog Temp
- Sensor DHT22
#### Actuadores:
- 2 Buzzer en uso con alarmas
- 3 servos
- 1 Relay on foco
## Tabla de historias de usuario
| Id | Historia de usuario | Prioridad | Estimación | Como probarlo | Responsable |
|----|---------------------|-----------|------------|---------------|-------------|
|  1  | Realizar detección de Flamas|Alta| 1 dia |Prueba de Fuergo|Daniela|
|  2  | Detección de Alarmas temperatura y apertura de puerta superior de jaula|Alta|2 horas|Código prueba de Servo, sensores y buzzer|Fernanda|
|  3  | Conección de Servo 1 y 2 para apertura de la puerta inferior con señales de Sensores|Alta| 2 horas|Codigo prueba de prueba|Ricardo|
|  4  | Endendido del foco a partir de la señal del Relay|Alta|1/2 dia|Código prueba|Daniela|
|  5  | Envio de datos a travez de la Rasperry Pi|Alta|3 dias|Prueba de códigos fuente a la Base de Datos|Antonio|
## Video:
[Video Esxplicativo Hamster House]()
## Resultado Final
![image](https://user-images.githubusercontent.com/115411941/233499465-27c5dec6-3e29-4ef2-867a-bd5c6829f22c.png)

