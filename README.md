# Grandcentrix-Test-Automation

(Project not completed)

1. Created a Device simulator which can be turned ON/OFF, and sends random energy consumption data upon request.
2. Created a Application simulator which can be used to link to devices, turn ON/OFF, get energy consumption data.
3. Established a connection between both simulator via MQTT.
4. Each instance will have a publisher and subscriber client.
5. They can communicate via subscribing and publishing under topics.
6. So, Application can send ON/OFF requests via MQTT.
7. Upon receive the command via MQTT, the device turns ON/OFF.
8. Device responds back via MQTT, that it is turned ON/OFF.
9. Application recieves this data and updates it to output.
