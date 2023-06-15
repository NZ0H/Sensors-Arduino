# Sensors-Arduino

Projet de fin d'année à l'IUT, l'objectif du programme est d'obtenir le résultat des 3 capteurs (pression, température, humidité) au format 32 bits, ainsi que le résultat d'un boîtier TOR, également sur 32 bits. Ensuite, nous effectuons un checksum sous la forme d'un XOR entre les résultats, puis nous inverserons les bits de rang pair à l'aide d'un masque. Enfin, nous enverrons le résultat via une communication en série à une Raspberry.


End of year project at the IUT, the objective of the program is to obtain the results of the 3 sensors (pressure, temperature, humidity) in 32-bit format, along with the result from a TOR box, also in 32 bits. Then, we perform a checksum in the form of an XOR operation between the results, and finally, we invert the bits of even rank using a mask. The result is then sent via serial communication to a Raspberry Pi.
