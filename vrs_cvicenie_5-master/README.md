# Náplň cvičenia
##### - s pomocou CubeMX vytvoriť nový projekt a nastaviť počiatočnú konfiguráciu mikrokontrolera
##### - V názve adresy kam vytvárame projektu sa nesmie nachádzať diakritika ani medzery (može to sposobovať problemy)!

# CubeMX a nový projekt
### 1. vytvorenie nového STM32 projektu

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/novy_projekt.png" width="500">
</p>

### 2. zvolenie typu MCU, ktorý chceme programovať (STM32F303K8)

<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/vyber_MCU.png" width="500">
</p>

### 3. zvolenie názvu projektu (všetko ostatné je už dobre nastavené od začiatku)

- projekt je týmto krokom vytvorený a prechádza sa do prostredia CubeMX, kde sa konfigurujú periférie MCU
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/vyber_nazvu.png" width="350">
</p>

### 4. konfigurácia periférii MCU

- nastavenie funkcie GPIO alebo ich priradenie k periférii (podľa potreby)
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/konfiguracia_periferii.png" width="300">
</p>

- konfigurácia konkrétnych GPIO, ktoré boli zvolené v predošlom kroku
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/konfiguracia_periferii_2.PNG" width="750">
</p>

- v nastaveniach hodín (clock configuration) nie je nutná žiadna zmena, pretože nám stačí počiatočná konfigurácia

### 5. nastavenia súvisiace s generovaním kódu

- nastavenia, ktoré cheme nastaviť ešte pred samotným generovaním kódu

- pre každú použitú perifériu bude vygenerovaný ".c" a ".h" súbor
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/nastavenie_generovania_3.PNG" width="450">
</p>

- zvolenie "LL - low level" knižnice
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/nastavenie_generovania_2.PNG" width="650">
</p>

### 6. generovanie kódu

- vygenerovanie kódu
<p align="center">
    <img src="https://github.com/VRS-Predmet/vrs_cvicenie_5/blob/master/images/code_generation.png" width="500">
</p>

# Zadanie
- nastaviť pin PA3 tak, aby bol zdrojom vonkajšieho perušenia (EXTI), ak je na neho privedená nábežná hrana 
- ak nastane prerušenie, LED na vývojovej doske zmení svoj aktuálny stav
