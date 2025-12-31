# Zooma (Proiect POO) v0.5.0

Un joc de strategie rapid, de tip bubble-shooter, inspirat din Zuma, implementat în C++ folosind biblioteca grafică SFML (versiunea 3.0). Această versiune pune accent pe o arhitectură robustă Orientată pe Obiecte și pe gestionarea eficientă a memoriei.

! Versiunea v0.5.1 include toate cerințele din tema 2

## Funcționalități Gameplay

* **Fizică Avansată a Șarpelui:**
    * Simularea mișcării bilelor pe un traseu predefinit.
    * **Gaps & Push Logic:** Bilele se pot despărți (creând spații goale) când sunt distruse elemente din interiorul șirului.
    * **Mecanică Magnetică:** Dacă segmentele separate de un gol au capete de aceeași culoare și pot forma un combo, segmentul din față este atras magnetic înapoi pentru a închide golul.
* **Proiector Interactiv:**
    * Tragerea bilelor colorate.
    * Previzualizarea următoarei bile și comutarea între bila curentă/următoare (Click Dreapta).
    * Animație de "pop-in" la inserarea bilelor în șir.
* **Sistem de Efecte Speciale (Power-ups):**
    * Efectele apar aleatoriu pe bilele din șir sau la generare.
    * Efectele au un timer de viață (dispar după 15s dacă nu sunt folosite).
    * **Explozie:** Distruge bilele pe o rază mare.
    * **Retro (Time Back):** Împinge întregul șir înapoi timp de câteva secunde.
    * **Accuracy:** Afișează o linie de ghidare pentru țintire precisă.
    * **Îngheț (Freeze):** Oprește complet înaintarea șarpelui timp de 3 secunde.
* **Logica Jocului:**
    * Detectarea și eliminarea potrivirilor de 3+ bile (Match-3).
    * Condiții de victorie (șir gol) și înfrângere (șirul ajunge la final).

## Concepte Tehnice Implementate (POO & C++)

Proiectul demonstrează utilizarea conceptelor avansate de C++ și OOP:

1.  **Programare Orientată pe Obiecte (OOP):**
    * **Moștenire & Polimorfism:** Clasă de bază abstractă `EfectBila` cu metode virtuale pure (`doAplica`, `clone`), derivată în clase concrete (`EfectExplozie`, `EfectInghet`, etc.).
    * **Encapsulare:** Toate clasele (`Bila`, `SirDeBile`, `Nivel`, `Proiector`) își protejează datele membre.
2.  **Managementul Memoriei:**
    * **Smart Pointers:** Utilizare extensivă a `std::unique_ptr` pentru gestionarea polimorfă a efectelor, eliminând riscul de memory leaks.
    * **Idiomul Copy-and-Swap:** Implementare corectă a Constructorului de Copiere, Operatorului de Atribuire și Destructorului în clasa `Bila` (Rule of Three/Five) pentru a gestiona resursele dinamice.
3.  **Gestionarea Erorilor:**
    * Ierarhie proprie de excepții: `ZoomaException` (bază) -> `ResursaLipsaException`, `LogicaJocException`, etc.
    * Blocuri `try-catch` în `main` pentru o execuție sigură.
4.  **STL & Algoritmi:**
    * Utilizarea containerelor: `std::vector`, `std::list` (pentru inserări/ștergeri eficiente în șarpe).
    * Generare de numere aleatoare modernă folosind `<random>` (std::mt19937).
5.  **Elemente Statice:**
    * Membri și metode `static` pentru statistici globale (ex: contorizarea bilelor distruse).