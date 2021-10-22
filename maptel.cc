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

#ifdef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

using maptel_id_t = unsigned long;

// static jest dlatego że extern "C" wywala namespace i funkcja byłaby widzoczna
// w C. Jeżeli wywalimy extern "C" to można wywalić to static
static unordered_map<maptel_id_t, unordered_map<string, string>> &dicts() {
    static unordered_map<maptel_id_t, unordered_map<string, string>> d;
    return d;
};

// Zakładamy, że poprawny telefon może być pusty.
static regex &tel_nr() {
    static regex r("\\d*");
    return r;
}

unsigned long maptel_create(void) {
    if (debug) cerr << "maptel: maptel_create()\n";
    static maptel_id_t dict_idx = 0;
    static unordered_map<string, string> empty_map;

    // Wstawiamy pustą mapę, żeby zainicjalizować wartość pod kluczem dict_idx.
    dicts()[dict_idx] = empty_map;

    if (debug) cerr << "maptel: maptel_create: new map id = " << dict_idx << "\n";

    return dict_idx++;
}

void maptel_delete(maptel_id_t id) {
    if (debug) cerr << "maptel: maptel_delete(" << id << ")\n";
    assert(dicts().count(id) != 0);
    dicts().erase(id);
    if (debug) cerr << "maptel: maptel_delete: map " << id << " deleted";
}

void maptel_insert(maptel_id_t id, char const *tel_src, char const *tel_dst) {
    if (debug) cerr << "maptel: maptel_insert(" << id << ", " << tel_src << ", " << tel_dst << ")\n";
    assert(dicts().count(id) != 0);
    assert(tel_src != nullptr);
    assert(tel_dst != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);
    assert(strlen(tel_dst) <= TEL_NUM_MAX_LEN);

    string src(tel_src), dst(tel_dst);

    assert(regex_match(src, tel_nr()));
    assert(regex_match(dst, tel_nr()));

    dicts()[id][src] = dst;
    if (debug) cerr << "maptel: maptel_insert: inserted\n";

}

void maptel_erase(maptel_id_t id, char const *tel_src) {
    if (debug) cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")\n";
    assert(dicts().count(id) != 0);
    assert(tel_src != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    string src(tel_src);

    assert(regex_match(src, tel_nr()));
    assert(dicts()[id].count(src) != 0);

    dicts()[id].erase(src);
    if (debug) cerr << "maptel: maptel_erase: erased\n";
}

void maptel_transform(maptel_id_t id, char const *tel_src, char *tel_dst,
                      size_t len) {
    if (debug) cerr << "maptel: maptel_transform(" << id << ", " << tel_src << ", " << tel_dst << ", " << len << ")\n";
    assert(dicts().count(id) != 0);
    assert(tel_src != nullptr);
    assert(tel_dst != nullptr);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    unordered_set<string> cycle;
    string src(tel_src);

    assert(regex_match(src, tel_nr()));

    auto dict = dicts()[id];
    unordered_map<string, string>::iterator it;

    while ((it = dict.find(src)) != dict.end()) {
        src = it->second;
        if (cycle.count(src)) { // Trafiliśmy na cykl.
            if (debug) cerr << "maptel: maptel_transform: cycle detected\n";

            if (strlen(tel_src) >= len) {
                // Jeśli nie ma zmiany numeru lub zmiany tworzą cykl, to zapisuje w tel_dst
                // numer tel_src.
                if (debug) cerr << "maptel: maptel_transform: telephone number is too long\n";
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
    if (debug) cerr << "maptel: maptel_transform:" << tel_src << " -> " << tel_dst <<"\n";
}

} // namespace jnp1

} // extern "C"