Am creat o structura RGB pentru imagine

	Task 1:
EDIT
Am dechis fisierul binar cu modul "rb" pentru a putea citi din el.
Am citit structurile header si info si am mutat cursorul cu offsetul
pentru a putea citi matricea de pixeli.
Am alocat memorie liniilor matricei si dupa cu un for a alocat memorie
pentru coloanele corespunzatoare liniei.
Am calculat paddingul matricei.
Am citit matricea, iar la final de fiecare rand am mutat cursorul
pentru a sari paddingul.

SAVE
Am deschis fisierul binar cu modul "wb" pentru a putea scrie in el.
Am scris headerul si info in fisier.
Am scris matricea de pixeli, iar la final de rand am adaugat paddingul.

QUIT
Am golit matricea.

	Task 2:
INSERT
Am verificat daca coordonatele punctului dat apartin matricei.
Am citit a doua matrice de inserat.
Am parcurs matricea initiala si am adaugat matricea de inserat
de la pozitia (x, y).
Astfel, se va adauga prima oara linia 0 din matricea de inserat,
dupa linia 1, ..., linia x

	Task 3:
SET DRAW COLOR
Am citit RGB-ul culorii noi.

SET LINE WIDTH
Am citit grosimea linii noi.

DRAW LINE:
Am calculat minimul, respectiv maximul dintre x-uri si y-uri pentru
a vedea care este intervalul mai mare.
Am luat 4 cazuri: linie dreapta verticala, linie dreapta orizontala,
linie oblica cu intervalul x-urilor mai mare, linie oblica cu
intervalul y-urilor mai mare.
Linii drepte: Parcurg intervalul si iau 2 for-uri care merg de la
jumatatea grosimii in stanga bitului pana la jumatatea grosimii
in dreapta bitului.
Verific daca coordonatele punctului care urmeaza sa fie colorat
apartin matricei si colorez punctul.
Linii oblice: Calculez x in functie de y pentru intervalul y-urilor
mai mare, respectiv y in functie de x pentru intervalul x-urilor.
Se repeta acelasi algoritm ca la linii drepte.

DRAW RECTANGLE
Apelez functia draw_line pentru cele 4 linii ale dreptunghiului.

DRAW TRIANGLE
Apelez functia draw_line pentru cele 3 linii ale triunghiului.

	Task 4:
FILL
Verific daca coordonatele punctului la care am ajuns sunt bune.
Verific daca pixelul la care am ajuns are culoarea initiala
a punctului dat.
Colorez pixelul si apelez functia pentru a trece la vecinii lui.

	MAIN:
Am citit inputul si in functie de cuvintele cheie am apelat
funtiile specifice.