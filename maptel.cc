#include "maptel.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cstring>
#include <cassert>
#include <regex>

using namespace std;

extern "C" {

namespace jnp1 {

#ifndef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

using maptel_id_t = unsigned long;

//pytanie: czy nazwa dicts jest dobra?
unordered_map<maptel_id_t, unordered_map<string, string>> dicts;

// Zakładamy, że poprawny telefon jest niepusty.
// (chyba że pusty jest poprawny to wtedy powinno być \\d*)
regex tel_nr("\\d+");

unsigned long maptel_create(void) {
    static maptel_id_t dict_idx = 0;
    return dict_idx++;
}

void maptel_delete(maptel_id_t id) {
    assert(dicts.count(id) != 0);
    dicts.erase(id);
}

void maptel_insert(maptel_id_t id, char const *tel_src, char const *tel_dst) {
    assert(dicts.count(id) != 0);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);
    assert(strlen(tel_dst) <= TEL_NUM_MAX_LEN);

    string src(tel_src), dst(tel_dst);

    assert(regex_match(src, tel_nr));
    assert(regex_match(dst, tel_nr));

    dicts[id][src] = dst;
}

void maptel_erase(maptel_id_t id, char const *tel_src) {
    assert(dicts.count(id) != 0);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    string src(tel_src);

    assert(regex_match(src, tel_nr));
    assert(dicts[id].count(src) != 0);

    dicts[id].erase(src);
}

void maptel_transform(maptel_id_t id, char const *tel_src, char *tel_dst,
                      size_t len) {
    assert(dicts.count(id) != 0);
    assert(strlen(tel_src) <= TEL_NUM_MAX_LEN);

    unordered_set<string> cycle;
    string src(tel_src), dst;

    assert(regex_match(src, tel_nr));

    //todo: zrobić tak żeby nie kopiować
    while (dicts[id].count(dst)) {
        src = dst;
        // todo: nie szukać w mapie 2 razy po tym samym kluczu
        dst = dicts[id][src];
        if (cycle.count(dst)) {
            // todo: error
            if (strlen(tel_src) >= len) {
                // todo: kolejby error (nie wyspecyfikowany w treści)
            } else {
                strcpy(tel_dst, tel_src);
            }
            return;
        }
        cycle.insert(dst);
    }
    if (len >= dst.size()) {
        //todo: czy tak można?
        strcpy(tel_dst, &dst[0]);
    }
}

} // namespace jnp1

} // extern "C"