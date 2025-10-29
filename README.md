# Zooma

Joc simplu și rapid de strategie, de tip bubble-shooter, implementat momentan (v0.1) in consolă.

## Funcționalități utilizate

* Simularea mișcării șirului de bile pe un traseu virtual.
* Tragerea bilelor colorate din proiector.
* Detectarea și eliminarea potrivirilor de 3+ bile de aceeași culoare.
* Sistem de efecte speciale (`BOMBA`, `INVERSARE`) aplicate aleatoriu pe bile și activate la potrivire.
* Citirea secvenței de joc dintr-un fișier `tastatura.txt`.

## Cum se rulează

Proiectul este configurat cu CMake. După compilare, executabilul (`oop`) va rula și va citi automat comenzile din fișierul `tastatura.txt`, afișând starea jocului în consolă după fiecare pas.