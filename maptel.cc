#include "maptel.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cstring>

using namespace std;

#ifndef NDEBUG
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

using maptel_id_t = unsigned long;

//todo: nazwac to lepiej
unordered_map<maptel_id_t, unordered_map<string, string>> dicts;
maptel_id_t dict_idx = 0;

unsigned long maptel_create(void) {
    return dict_idx++;
}

void maptel_delete(maptel_id_t id) {
    dicts.erase(id);
}

void maptel_insert(maptel_id_t id, char const *tel_src, char const *tel_dst) {
    //todo: spr czy id istnieje w mapie
    //todo: spr czy długość nie przekracza limitu 22
    string src(tel_src), dst(tel_dst);
    dicts[id][src] = dst;
}

void maptel_erase(maptel_id_t id, char const *tel_src) {
    //todo: spr czy id istnieje w mapie
    //todo: spr czy długość nie przekracza limitu 22
    string src(tel_src);
    dicts[id].erase(src);
}

void maptel_transform(maptel_id_t id, char const *tel_src, char *tel_dst,
                      size_t len) {
    unordered_set<string> cycle;
    //todo: spr czy id istnieje w mapie
    //todo: spr czy długość nie przekracza limitu 22
    string src(tel_src), dst(tel_dst);
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
