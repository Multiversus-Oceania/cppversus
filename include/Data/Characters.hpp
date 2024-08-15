#ifndef __CHARACTERS_HPP__
#define __CHARACTERS_HPP__

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <algorithm>

#include <spdlog/spdlog.h>

namespace CPPVersus {

struct Character {
    uint64_t id;
    std::string name;
    std::string slug;
};

const std::vector<Character> characters = {
    { 0,  "Shaggy",             "character_shaggy"        },
    { 1,  "Jake",               "character_jake"          },
    { 2,  "Wonder Woman",       "character_wonder_woman"  },
    { 3,  "Jason",              "character_jason"         },
    { 4,  "Taz",                "character_taz"           },
    { 5,  "Steven Universe",    "character_steven"        },
    { 6,  "Rick",               "character_c020"          },
    { 7,  "Banana Guard",       "character_bananaguard"   },
    { 8,  "Harley Quinn",       "character_harleyquinn"   },
    { 9,  "Arya Stark",         "character_arya"          },
    { 10, "Gizmo",              "character_c023a"         },
    { 11, "Batman",             "character_batman"        },
    { 12, "Velma",              "character_velma"         },
    { 13, "Joker",              "character_c028"          },
    { 14, "LeBron James",       "character_c16"           },
    { 15, "Iron Giant",         "character_c017"          },
    { 16, "Bugs Bunny",         "character_bugs_bunny"    },
    { 17, "Black Adam",         "character_c021"          },
    { 18, "Garnet",             "character_garnet"        },
    { 19, "Finn",               "character_finn"          },
    { 20, "Reindog",            "character_creature"      },
    { 21, "Morty",              "character_c019"          },
    { 22, "Marvin the Martian", "character_c018"          },
    { 23, "Stripe",             "character_c023b"         },
    { 24, "Tom and Jerry",      "character_tom_and_jerry" },
    { 25, "Superman",           "character_superman"      },
    { 26, "Agent Smith",        "character_c036"          },
    { 27, "Samurai Jack",       "character_c035"          }
};

inline std::optional<Character> characterFromSlug(std::string slug) {
    // convert name to lowercase
    for(char& c : slug) {
        c = tolower(c);
    }

    auto it = std::find_if(characters.begin(), characters.end(), [slug](Character character) {
        return character.slug == slug;
    });

    if(it == characters.end()) return std::optional<Character>();
    return std::optional<Character>(*it);
}

};

#endif