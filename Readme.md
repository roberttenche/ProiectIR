# Proiect Introducere Robotică 2022
## Profesor Curs: Cosmin Bonchiș
## Profesor Laborator: Theodor Grumeza

Tema proiectului: Smart Car  
Studenți: Andrei Pleian, Țenche Robert, Maciovan Răzvan, Vlad Petcu, Cristian Toth  

Descriere sumară:  
O mașinuță controlată prin telecomandă, care se oprește dacă urmează coliziunea cu un obstacol ( Emergency Braking System ).

Componentele mașinii:
- un șasiu modular
- 4 roți + modulul de control
- 2 arduino nano ( Master + EBS )
- 2 senzori ultrasonici ( EBS )
- 2 module SPI/CAN ( MCP2515 )
- 4 baterii alcaline AA 1.5v
- 2 baterii alcaline PP3 9v

Descriere detaliată:  
Pe șasiu sunt montate toate componentele.  
Master-ul controlează modulul roților.  
EBS-ul controlează cei 2 senzori ultrasonici montați în față și în spatele mașinii.  
EBS-ul trimite constat mesaje pe CAN în care se raportează dacă urmează o coliziune sau nu.  
Mesajul constă în 2 bytes  
Primul byte raportează dacă urmează o coliziune.  
Al doilea byte este direcția posibilei coliziuni.  
Master-ul va citi constant primul primul byte, dacă urmează o coliziune se va opri mașina pe loc, altfel continuă execuția normal.  
Dacă la pasul anterior s-a primit un raport de coliziune, se va citi direcția din cel de al doilea byte, și se va bloca direcția respectivă.