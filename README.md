## treść zadania
Biblioteka standardowa języka C++ udostępnia bardzo przydatny typ
`unordered_map`. Jest to typ generyczny, pozwalający tworzyć słowniki
o różnych kluczach i wartościach, np. `unordered_map<int, string>`,
`unordered_map<string, string>`, a nawet
`unordered_map<string, unordered_map<int, string>>` i inne, jeszcze bardziej
złożone. Biblioteka standardowa języka C nie udostępnia podobnego typu.
W praktyce programistycznej często pojawia się też konieczność łączenia kodu
napisanego w językach C i C++.

Napisz w C++ moduł maptel udostępniający w języku C słowniki obsługujące zmiany
numerów telefonów. **W języku C numer telefonu jest przechowywany w tablicy znaków
typu char jako ciąg maksymalnie 22 cyfr zapisanych w ASCII i jest zakończony
znakiem o wartości zero.**

Moduł maptel powinien składać się z pliku nagłówkowego `maptel.h` deklarującego
jego interfejs oraz pliku `maptel.cc` zawierającego jego implementację.

Moduł maptel powinien udostępniać następujący interfejs:

```C
// Tworzy słownik i zwraca liczbę naturalną będącą jego identyfikatorem.
unsigned long maptel_create(void);

// Usuwa słownik o identyfikatorze id.
void maptel_delete(unsigned long id)

// Wstawia do słownika o identyfikatorze id informację o zmianie numeru
// tel_src na numer tel_dst. Nadpisuje ewentualną istniejącą informację.
void maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst);

// Jeśli w słowniku o identyfikatorze id jest informacja o zmianie numeru
// tel_src, to ją usuwa. W przeciwnym przypadku nic nie robi.
void maptel_erase(unsigned long id, char const *tel_src);

// Sprawdza, czy w słowniku o identyfikatorze id jest zapisana zmiana numeru
// tel_src. Podąża ciągiem kolejnych zmian. Zapisuje zmieniony numer w tel_dst.
// Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to zapisuje w tel_dst
// numer tel_src. Wartość len to rozmiar przydzielonej pamięci wskazywanej
// przez tel_dst.
void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len);
```

Nagłówek modułu `maptel` powinien udostępniać stałą `TEL_NUM_MAX_LEN` typu `size_t`
o wartości `22`.

Przykład użycia znajduje się w pliku `maptel_test1.c`.

Ludzie bywają przewrotni i mając kod, który można używać w języku C, chcą go też
używać w C++. Należy zapewnić możliwość użycia pliku nagłówkowego `maptel.h`
w języku C++ w taki sposób, aby interfejs modułu maptel został umieszczony
w przestrzeni nazw `jnp1` i nie był widoczny w globalnej przestrzeni nazw.
Przykład użycia znajduje się w pliku `maptel_test2.c`c.

### kompilacja
Kompilowanie przykładów:

```g++ -c -Wall -Wextra -O2 -std=c++17 maptel.cc -o maptel.o
gcc -c -Wall -Wextra -O2 -std=c11 maptel_test1.c -o maptel_test1.o
g++ -c -Wall -Wextra -O2 -std=c++17 maptel_test2.cc -o maptel_test2.o
g++ maptel_test1.o maptel.o -o maptel_test1
g++ maptel_test2.o maptel.o -o maptel_test21
g++ maptel.o maptel_test2.o -o maptel_test22
```

### błędy
**Moduł maptel powinien sprawdzać poprawność parametrów i poprawność wykonania
funkcji za pomocą asercji i wypisywać na standardowy strumień błędów informacje
diagnostyczne. Należy sprawdzać przynajmniej, czy identyfikator słownika jest
poprawny, czy przekazany wskaźnik nie jest NULL-em, czy przekazany napis nie
jest za długi, czy zawiera poprawne znaki i czy kończy się znakiem o wartości
zero. Kompilowanie z parametrem `-DNDEBUG` powinno wyłączać sprawdzanie
i wypisywanie.**

Przykłady informacji diagnostycznych wypisywanych przez programy `maptel_test1`,
`maptel_test21` i `maptel_test22` znajdują się odpowiednio w plikach
`maptel_test1.err` i `maptel_test2.err`.

### inne
Oczekiwane rozwiązanie powinno korzystać z kontenerów i funkcji udostępnianych
przez standardową bibliotekę C++. Nie należy definiować własnych struktur ani
klas. W szczególności nie należy przechowywać przekazanych przez użytkownika
wskaźników `const char*` bezpośrednio, bowiem użytkownik może po wykonaniu
operacji modyfikować dane pod uprzednio przekazanym wskaźnikiem lub zwolnić
pamięć. W rozwiązaniu nie należy nadużywać kompilacji warunkowej. Fragmenty
tekstu źródłowego realizujące wyspecyfikowane operacje modułu maptel nie powinny
zależeć od sposobu kompilowania - parametr `-DNDEBUG` lub jego brak (inaczej
wypisywanie informacji diagnostycznych nie miałoby sensu).

Pliki z rozwiązaniem `maptel.h` i `maptel.cc` należy umieścić w repozytorium
w katalogu

`grupaN/zadanie2/ab123456+cd123456`

lub

`grupaN/zadanie2/ab123456+cd123456+ef123456`

gdzie `N` jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami
członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie
powinien zawierać innych plików. **Nie wolno umieszczać w repozytorium plików
dużych, binarnych, tymczasowych (np. *.o) ani innych zbędnych.**

### kryteria oceny stylu
=== Proponowane kryteria oceniania drugiego zadania ===

== Cele dydaktyczne ==

Studenci powinni:
- poznać kolejne elementy o STL-a (insert, erase, iteratory, ...),
- ugruntować w sobie konieczność weryfikacji poprawności danych wejściowych,
- nauczyć się łączenia kodu C i C++,
- dowiedzieć się, jak inicjuje się obiekty globalne i statyczne,
- wiedzieć, dlaczego i jak należy ograniczać widoczność zmiennych
  globalnych i funkcji pomocniczych modułu (jednostki translacji)
  niebędących częścią interfejsu tego modułu;
- dowiedzieć się, jak poprawnie korzystać z dyrektyw preprocesora.

== Ocena automatyczna ==

Za testy automatyczne jest do zdobycia 6 punktów, po jednym punkcie za przejście
każdej z sześciu grup testów.

== Ocena jakości kodu ==

Zaczynamy od 4 punktów i odejmujemy po jednym punkcie za:
- niezgodność zawartości repozytorium ze specyfikacją zadania;
- brzydki styl (niepoprawne wstawianie spacji, wcięć, odstępów, brak komentarzy,
  magiczne stałe itd.);
- ostrzeżenia wypisywane przez kompilator (opcje -Wall -Wextra);
- błędy w komunikatach diagnostycznych;
- dłubanie własnych klas lub struktur zamiast użycia kontenerów STL-owych;
- rozwlekłą lub nieelegancką strukturę programu, rozpatrywanie zbyt wielu
  warunków brzegowych, powtarzanie kodu;
- nieskuteczną obsługę (lub jej brak) problemu "static initialization
  order fiasco" - o ile test automatyczny tego nie wykrył;
- niepoprawne pliki nagłówkowe, czyli brak header guard
  (#ifndef #define ... #endif) lub #ifdef __cplusplus);
- użycie #ifdef __cplusplus, extern "C" w części kodu kompilowanej tylko w C++;
- wstawienie using do pliku nagłówkowego;
- nieukrycie przed światem zewnętrznym zmiennych globalnych i funkcji
  niebędących częścią interfejsu modułu;
- uzależnienie fragmentów kodu źródłowego realizujące wyspecyfikowane operacje
  na mapach od opcji -DNDEBUG;
- wielokrotne wyszukiwanie w mapie wg tego samego klucza;
- zły dobór typów, np. używanie int zamiast size_t itp.
- potencjalne wycieki pamięci;
- inne, niewymienione w tych kryteriach błędy w szczególności te niewykryte
  przez testy automatyczne.
  Przy czym ocena nie może być ujemna.

Ponadto piętnujemy:
- przekazywanie funkcjom dużych argumentów (np. typu string) przez
  wartość, takie obiekty przekazujemy przez stałą referencję (jeszcze
  bym za to nie odejmował punktów, bo to jest temat zadania 3).

Jednym z rozwiązań problemu initialization order fiasco proponowanym
w czytankach jest użycie pamięci dynamicznej, której się potem nie zwalnia
(zakłada się, że system operacyjny odzyska tę pamięć po zakończeniu procesu).
W tym zadaniu można zastosować takie rozwiązanie, w którym nie trzeba używać
pamięci dynamicznej, ale ogólnie takie rozwiązanie też nie jest bez wad (patrz
czytanki), więc można dopuścić rozwiązanie z niezwalnianą pamięcią dynamiczną
globalnego słownika.

