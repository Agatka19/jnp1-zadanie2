#include "maptel.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cstring>
#include <cassert>
#include <regex>

using namespace std;

// == Ocena jakości kodu ==
//Zaczynamy od 4 punktów i odejmujemy po jednym punkcie za:
// użycie #ifdef __cplusplus, extern "C" w części kodu kompilowanej tylko w C++;

// ale bez tego mi się nie kompiluje -> trzeba coś z tym zrobić
extern "C" {

namespace jnp1 {

#ifndef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

using maptel_id_t = unsigned long;

//pytanie: czy nazwa dicts jest dobra? (jeśli nie to trzeba też zmienić
// powiązaną z nią nazwę zmiennej lokalnej w maptel_transform)
unordered_map<maptel_id_t, unordered_map<string, string>> dicts;

// Zakładamy, że poprawny telefon jest niepusty.
// (chyba że pusty jest poprawny to wtedy powinno być \\d*)
regex tel_nr("\\d+");

unsigned long maptel_create(void) {
    static maptel_id_t dict_idx = 0;
    static unordered_map<string, string> empty_map;

    // Wstawiamy pustą mapę, żeby zainicjalizować wartość pod kluczem dict_idx.
    dicts[dict_idx] = empty_map;

    return dict_idx++;
}

void maptel_delete(maptel_id_t id) {
    assert(dicts.count(id) != 0);
    dicts.erase(id);
}

void maptel_insert(maptel_id_t id, char const *tel_src, char const *tel_dst) {
    assert(dicts.count(id) != 0);
    assert(tel_src != nullptr);
    assert(tel_dst != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);
    assert(strlen(tel_dst) <= TEL_NUM_MAX_LEN);

    string src(tel_src), dst(tel_dst);

    assert(regex_match(src, tel_nr));
    assert(regex_match(dst, tel_nr));

    dicts[id][src] = dst;
}

void maptel_erase(maptel_id_t id, char const *tel_src) {
    assert(dicts.count(id) != 0);
    assert(tel_src != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    string src(tel_src);

    assert(regex_match(src, tel_nr));
    assert(dicts[id].count(src) != 0);

    dicts[id].erase(src);
}

void maptel_transform(maptel_id_t id, char const *tel_src, char *tel_dst,
                      size_t len) {
    assert(dicts.count(id) != 0);
    assert(tel_src != nullptr);
    assert(tel_dst != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    unordered_set<string> cycle;
    string src(tel_src);

    assert(regex_match(src, tel_nr));

    auto dict = dicts[id];
    unordered_map<string, string>::iterator it;

    //todo: zrobić tak żeby nie kopiować
    while ((it = dict.find(src)) != dict.end()) {
        src = it->second;
        if (cycle.count(src)) { // trafiliśmy na cykl
            // todo: wypisać error

            if (strlen(tel_src) >= len) {
                // Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to zapisuje w tel_dst
                // numer tel_src.
                // todo: wypisać kolejny error taki że tel_src nie zmieści się w tel_dst
            } else {
                strcpy(tel_dst, tel_src);
            }
            return;
        }
        cycle.insert(src);
    }

    if (len >= src.size()) {
        strcpy(tel_dst, &src[0]);
    }
}

} // namespace jnp1

} // extern "C"